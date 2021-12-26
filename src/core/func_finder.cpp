#include "func_finder.hpp"

#include "bind/binded_func.hpp"
#include "bind/bindings_lists.hpp"
#include "bindings_parser.hpp"
#include "core/maptable.hpp"
#include "defs.hpp"
#include "entry.hpp"
#include "key_logger_base.hpp"
#include "logger_parser.hpp"
#include "logger_parser_mgr.hpp"
#include "mode.hpp"
#include "path.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"

#include <array>
#include <memory>
#include <stdexcept>
#include <unordered_map>


namespace vind
{
    namespace core
    {
        struct FuncFinder::Impl {
            ModeArray<LoggerParserManager> mgrs_{} ;
            std::vector<bind::BindedFunc::SPtr> funcs_{} ;
        } ;

        FuncFinder::FuncFinder()
        : pimpl(std::make_unique<Impl>())
        {}

        FuncFinder::~FuncFinder() noexcept = default ;
        FuncFinder::FuncFinder(FuncFinder&& rhs) = default ;
        FuncFinder& FuncFinder::operator=(FuncFinder&&) = default ;

        LoggerParser::SPtr FuncFinder::transition_parser_states_in_batch(
                const KeyLoggerBase& lgr,
                Mode mode) {
            return pimpl->mgrs_[static_cast<int>(mode)].transition_parser_states_in_batch(lgr) ;
        }

        void FuncFinder::search_unrejected_parser(
                std::vector<LoggerParser::SPtr>& results,
                Mode mode) {
            pimpl->mgrs_[static_cast<int>(mode)].search_unrejected_parser(results) ;
        }

        LoggerParser::SPtr
        FuncFinder::find_rejected_with_ready_parser(Mode mode) {
            return pimpl->mgrs_[static_cast<int>(mode)].find_rejected_with_ready_parser() ;
        }

        LoggerParser::SPtr FuncFinder::find_waiting_parser(Mode mode) {
            return pimpl->mgrs_[static_cast<int>(mode)].find_waiting_parser() ;
        }

        LoggerParser::SPtr FuncFinder::find_accepted_parser(Mode mode) {
            return pimpl->mgrs_[static_cast<int>(mode)].find_accepted_parser() ;
        }

        LoggerParser::SPtr FuncFinder::find_parser_with_transition(
                const KeyLog& log,
                std::size_t low_priority_func_id,
                Mode mode) {
            return pimpl->mgrs_[static_cast<int>(mode)].find_parser_with_transition(
                    log,
                    low_priority_func_id) ;
        }

        void FuncFinder::backward_parser_states(std::size_t n, Mode mode) {
            return pimpl->mgrs_[static_cast<int>(mode)].backward_parser_states(n) ;
        }

        void FuncFinder::reset_parser_states(Mode mode) {
            return pimpl->mgrs_[static_cast<int>(mode)].reset_parser_states() ;
        }

        bind::BindedFunc::SPtr FuncFinder::find_func_byname(const std::string& name) const {
            auto id = bind::BindedFunc::name_to_id(name) ;
            for(const auto& func : pimpl->funcs_) {
                if(func->id() == id) {
                    return func ;
                }
            }

            return nullptr ;
        }

        void FuncFinder::reconstruct() {
            pimpl->funcs_ = bind::all_global_binded_funcs() ;
            do_reconstruct() ;
        }

        void FuncFinder::reconstruct(const std::vector<bind::BindedFunc::SPtr>& funcs) {
            pimpl->funcs_ = funcs ;
            do_reconstruct() ;
        }
        void FuncFinder::reconstruct(std::vector<bind::BindedFunc::SPtr>&& funcs) {
            pimpl->funcs_ = std::move(funcs) ;
            do_reconstruct() ;
        }

        void FuncFinder::do_reconstruct() {
            auto& maptable = MapTable::get_instance() ;

            std::unordered_set<std::size_t> funcid_set ;
            for(const auto& func : pimpl->funcs_) {
                funcid_set.insert(func->id()) ;
            }

            for(std::size_t i = 0 ; i < mode_num() ; i ++) {
                FuncMap<std::shared_ptr<CommandList>> funcmap ;

                auto maps = maptable.get_noremaps(static_cast<Mode>(i)) ;
                for(const auto& map : maps) {
                    const auto& func_name = map.target_command_string() ;
                    auto func_id = bind::BindedFunc::name_to_id(func_name) ;

                    if(funcid_set.find(func_id) == funcid_set.end()) {
                        continue ;
                    }

                    auto& shared_cmd_list = funcmap[func_name] ;
                    if(!shared_cmd_list) {
                        shared_cmd_list = std::make_shared<CommandList>() ;
                    }
                    shared_cmd_list->push_back(map.trigger_command()) ;
                }

                std::vector<LoggerParser::SPtr> parsers ;
                for(const auto& func : pimpl->funcs_) {
                    try {
                        auto p_parser = std::make_shared<LoggerParser>(func) ;
                        p_parser->share_parsed_binding_list(funcmap.at(func->name())) ;
                        parsers.push_back(std::move(p_parser)) ;
                    }
                    catch(const std::out_of_range&) {
                        continue ;
                    }
                }
                pimpl->mgrs_[i] = LoggerParserManager(std::move(parsers)) ;
            }
        }
    }
}
