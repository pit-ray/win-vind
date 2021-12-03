#include "main_loop.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

#include "func_finder.hpp"
#include "key_logger_base.hpp"
#include "keycode_def.hpp"
#include "logger_parser.hpp"
#include "logpooler.hpp"
#include "mapgate.hpp"
#include "ntype_logger.hpp"

#include "opt/vcmdline.hpp"

#include "bind/binded_func.hpp"
#include "bind/bindings_lists.hpp"
#include "bind/mode/change_mode.hpp"

//internal linkage
namespace
{
    using namespace vind ;
    core::NTypeLogger g_ntlgr{} ;
    core::FuncFinder g_funcfinder{} ;
    bind::BindedFunc::SPtr g_active_func = nullptr ;
}

namespace vind
{
    namespace core {
        void initialize_mainloop() {
            g_ntlgr.clear() ;
            g_active_func = nullptr ;
        }

        void reconstruct_mainloop() {
            for(auto& func : bind::all_global_binded_funcs()) {
                func->reconstruct() ;
            }
            g_funcfinder.reconstruct() ;

            MapGate::get_instance().reconstruct() ;
        }

        void update_mainloop() {
            auto log = LogPooler::get_instance().pop_log() ;

            auto result = g_ntlgr.logging_state(log) ;

            if(NTYPE_EMPTY(result)) {
                return ;
            }
            if(NTYPE_HEAD_NUM(result)) {
                opt::VCmdLine::print(opt::StaticMessage(
                            std::to_string(g_ntlgr.get_head_num()))) ;
                return ;
            }

            // std::cout << print(g_ntlgr) << std::endl ;

            if(g_ntlgr.is_long_pressing()) {
                if(g_active_func) {
                    g_active_func->process(g_ntlgr) ;
                }
                return ;
            }

            auto actid = g_active_func ? g_active_func->id() : 0 ;
            auto parser = g_funcfinder.find_parser_with_transition(
                    g_ntlgr.latest(), actid) ;
            g_active_func = nullptr ;

            if(parser) {
                if(parser->is_accepted()) {
                    if(g_ntlgr.has_head_num()) {
                        opt::VCmdLine::reset() ;
                    }

                    g_active_func = parser->get_func() ;
                    g_active_func->process(g_ntlgr) ;

                    g_ntlgr.accept() ;
                    g_funcfinder.reset_parser_states() ;
                }
                else if(parser->is_rejected_with_ready()) {
                    // It did not accepted, but only matched subsets.
                    // For example, bindings <ctrl> in <ctrl-f>
                    g_funcfinder.backward_parser_states(1) ;
                    g_ntlgr.remove_from_back(1) ;
                }
            }
            else {
                if(g_ntlgr.has_head_num()) {
                    opt::VCmdLine::refresh() ;
                }
                g_ntlgr.reject() ;
                g_funcfinder.reset_parser_states() ;
            }
        }
    }
}
