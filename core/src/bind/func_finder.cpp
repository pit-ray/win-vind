#include "bind/func_finder.hpp"

#include "bind/binded_func.hpp"
#include "bind/bindings_lists.hpp"
#include "bind/logger_parser.hpp"
#include "entry.hpp"
#include "g_maps.hpp"
#include "key/key_logger_base.hpp"
#include "mapdefs.hpp"
#include "mode.hpp"
#include "parser/bindings_json_parser.hpp"
#include "parser/bindings_parser.hpp"
#include "path.hpp"
#include "util/def.hpp"

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>


//inner linkage
namespace
{
    using namespace vind ;
    const auto& g_all_func_list = bindingslists::get() ;

    ParsedBindingLists g_parsed_bindlists ;
}

namespace vind
{
    using LoggerParserList = std::vector<LoggerParser::SPtr> ;
    struct FuncFinder::Impl {
        ModeArray<LoggerParserList> parser_ar_{} ;
    } ;

    FuncFinder::FuncFinder()
    : pimpl(std::make_unique<Impl>())
    {}

    FuncFinder::~FuncFinder() noexcept = default ;
    FuncFinder::FuncFinder(const FuncFinder& rhs)
    : pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
    {}
    FuncFinder& FuncFinder::operator=(const FuncFinder& rhs) {
        if(rhs.pimpl) {
            *pimpl = *(rhs.pimpl) ;
        }
        return *this ;
    }
    FuncFinder::FuncFinder(FuncFinder&& rhs)        = default ;
    FuncFinder& FuncFinder::operator=(FuncFinder&&) = default ;

    void FuncFinder::reconstruct_funcset() {
        //clear logger parser list by swapping
        ModeArray<LoggerParserList>().swap(pimpl->parser_ar_) ;

        for(std::size_t i = 0 ; i < mode::mode_num() ; i ++) {
            for(const auto& func : g_all_func_list) {
                try {
                    const auto& list = g_parsed_bindlists[i].at(func->name()) ;
                    auto p_parser = std::make_shared<LoggerParser>(func) ;

                    p_parser->share_parsed_binding_list(list) ;
                    pimpl->parser_ar_[i].push_back(std::move(p_parser)) ;
                }
                catch(const std::out_of_range&) {
                    continue ;
                }
            }
        }
    }

    LoggerParser::SPtr FuncFinder::transition_parser_states_in_batch(
            const KeyLoggerBase& lgr,
            mode::Mode mode) {
        LoggerParser::SPtr ptr = nullptr ;
        for(const auto& log : lgr) {
            ptr = find_parser_with_transition(log, 0, mode) ;
        }
        return ptr ;
    }

    void FuncFinder::search_unrejected_parser(
            std::vector<LoggerParser::SPtr>& results,
            mode::Mode mode) {

        if(!results.empty()) {
            results.clear() ;
        }

        for(auto& parser : pimpl->parser_ar_[static_cast<std::size_t>(mode)]) {
            if(!parser->is_rejected()) {
                results.push_back(parser) ;
            }
        }
    }

    LoggerParser::SPtr FuncFinder::find_rejected_with_ready_parser(mode::Mode mode) {
        for(auto& parser : pimpl->parser_ar_[static_cast<std::size_t>(mode)]) {
            if(parser->is_rejected_with_ready()) {
                return parser ;
            }
        }
        return nullptr ;
    }

    LoggerParser::SPtr FuncFinder::find_waiting_parser(mode::Mode mode) {
        for(auto& parser : pimpl->parser_ar_[static_cast<std::size_t>(mode)]) {
            if(parser->is_waiting()) {
                return parser ;
            }
        }
        return nullptr ;
    }

    LoggerParser::SPtr FuncFinder::find_accepted_parser(mode::Mode mode) {
        for(auto& parser : pimpl->parser_ar_[static_cast<std::size_t>(mode)]) {
            if(parser->is_accepted()) {
                return parser ;
            }
        }
        return nullptr ;
    }

    LoggerParser::SPtr FuncFinder::find_parser_with_transition(
            const KeyLog& log,
            std::size_t low_priority_func_id,
            mode::Mode mode) {
        LoggerParser::SPtr ptr = nullptr ;
        LoggerParser::SPtr low_priority_parser = nullptr ;
        unsigned char mostnum = 0 ;

        for(auto& parser : pimpl->parser_ar_[static_cast<std::size_t>(mode)]) {
            auto num = parser->validate_if_match(log) ;

            if(parser->is_rejected()) {
                continue ;
            }

            if(mostnum < num) {
                if(parser->get_func()->id() == low_priority_func_id) {
                    low_priority_parser = parser ;
                }
                else {
                    ptr = parser ;
                    mostnum = num ;
                }
            }
            else if(mostnum == num && !ptr->is_accepted()) {
                if(parser->is_accepted()) {
                    ptr = parser ;
                }
                else if(ptr->is_rejected_with_ready() && parser->is_waiting()) {
                    ptr = parser ;
                }
            }
        }

        if(ptr) {
            return ptr ;
        }
        if(low_priority_parser && low_priority_parser->is_accepted()) {
            return low_priority_parser ;
        }
        return nullptr ;
    }

    void FuncFinder::backward_parser_states(std::size_t n, mode::Mode mode) {
        for(auto& parser : pimpl->parser_ar_[static_cast<std::size_t>(mode)]) {
            parser->backward_state(n) ;
        }
    }

    void FuncFinder::reset_parser_states(mode::Mode mode) {
        for(auto& parser : pimpl->parser_ar_[static_cast<std::size_t>(mode)]) {
            parser->reset_state() ;
        }
    }

    BindedFunc::SPtr FuncFinder::find_func_byname(const std::string& name) {
        auto id = BindedFunc::name_to_id(name) ;
        for(const auto& func : g_all_func_list) {
            if(func->id() == id) return func ;
        }
        return nullptr ;
    }

    void FuncFinder::load_global_bindings() {
        ParsedBindingLists().swap(g_parsed_bindlists) ; //clear by empty swapping

        std::vector<gmaps::UniqueMap> maps{} ;

        for(size_t i = 0 ; i < mode::mode_num() ; i ++) {
            auto& funcmap = g_parsed_bindlists[i] ;

            maps.clear() ;
            gmaps::get_maps(static_cast<mode::Mode>(i), maps) ;
            for(const auto& map : maps) {
                if(!map.is_noremap_function()) {
                    continue ;
                }

                auto& shared_cmd_list = funcmap[map.func_name()] ;
                if(!shared_cmd_list) {
                    shared_cmd_list = std::make_shared<CommandList>() ;
                }
                shared_cmd_list->push_back(map.trigger_command()) ;
            }
        }

        // load configs of all function and give a chance to reconstruct FuncFinder automatically.
        for(auto& func : g_all_func_list) {
            func->reconstruct() ;
        }
    }

}
