#include "func_finder.hpp"

#include "bindings_parser.hpp"
#include "entry.hpp"
#include "g_maps.hpp"
#include "key_logger_base.hpp"
#include "logger_parser.hpp"
#include "mapdefs.hpp"
#include "mode.hpp"
#include "path.hpp"

#include "bind/binded_func.hpp"
#include "bind/bindings_lists.hpp"
#include "logger_parser_mgr.hpp"
#include "util/def.hpp"

#include <array>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{
    namespace core
    {
        struct FuncFinder::Impl {
            ModeArray<LoggerParserManager> mgrs_ {} ;
            std::vector<bind::BindedFunc::SPtr> funcs_ = bind::all_global_binded_funcs() ;
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
            return pimpl->mgrs_[static_cast<int>(mode)].find_parser_with_transition(log, 0) ;
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

        bind::BindedFunc::SPtr FuncFinder::find_func_byname(const std::string& name) {
            static auto funcs = bind::all_global_binded_funcs() ;

            auto id = bind::BindedFunc::name_to_id(name) ;
            for(const auto& func : funcs) {
                if(func->id() == id) {
                    return func ;
                }
            }

            return nullptr ;
        }

        void FuncFinder::reconstruct() {
            // load configs of all function and give a chance to reconstruct FuncFinder automatically.
            for(auto& func : pimpl->funcs_) {
                func->reconstruct() ;
            }

            //clear by empty swapping
            ParsedBindingLists cmds ;

            std::vector<UniqueMap> maps{} ;
            for(size_t i = 0 ; i < mode_num() ; i ++) {
                auto& funcmap = cmds[i] ;

                maps.clear() ;
                get_maps(static_cast<Mode>(i), maps) ;
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

                std::vector<LoggerParser> parsers ;
                for(const auto& func : pimpl->funcs_) {
                    try {
                        const auto& list = cmds[i].at(func->name()) ;
                        auto p_parser = std::make_shared<LoggerParser>(func) ;

                        p_parser->share_parsed_binding_list(list) ;
                        parsers.push_back(std::move(p_parser)) ;
                    }
                    catch(const std::out_of_range&) {
                        continue ;
                    }
                }
                pimpl->mgrs_[i] = std::move(parsers) ;
            }
        }
    }
}