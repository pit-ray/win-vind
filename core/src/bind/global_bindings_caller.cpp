#include "bind/global_bindings_caller.hpp"

#include <iostream>

#include "bind/base/key_logger_base.hpp"
#include "bind/base/ntype_logger.hpp"
#include "key/keycode_def.hpp"
#include "opt/virtual_cmd_line.hpp"

#include "bind/base/binded_func.hpp"
#include "bind/bind.hpp"

#include "bind/func_finder.hpp"
#include "bind/logger_parser.hpp"
#include "bind/mode/change_mode.hpp"

//internal linkage
namespace
{
    vind::NTypeLogger g_ntlgr{} ;
    vind::FuncFinder g_funcfinder{} ;
    vind::BindedFunc::SPtr g_active_func = nullptr ;
}

namespace vind
{
    namespace gbindcaller {

        void initialize() {
            g_ntlgr.clear() ;
            g_active_func = nullptr ;
        }

        void load_config() {
            funcfinder::load_global_bindings() ;
            g_funcfinder.reconstruct_funcset() ;
        }

        void call_matched_func() {
            auto result = g_ntlgr.logging_state() ;
            if(NTYPE_EMPTY(result)) {
                return ;
            }
            if(NTYPE_HEAD_NUM(result)) {
                VirtualCmdLine::cout(std::to_string(g_ntlgr.get_head_num())) ;
                return ;
            }

            if(g_ntlgr.is_long_pressing()) {
                if(g_active_func) {
                    g_active_func->process(g_ntlgr) ;
                }
                return ;
            }

            auto actid = g_active_func ? g_active_func->id() : 0 ;
            auto parser = g_funcfinder.find_parser_with_transition(g_ntlgr.latest(), actid) ;
            g_active_func = nullptr ;

            if(parser) {
                if(parser->is_accepted()) {
                    VirtualCmdLine::reset() ;

                    g_active_func = parser->get_func() ;
                    g_active_func->process(g_ntlgr) ;

                    g_ntlgr.accept() ;
                    g_funcfinder.reset_parser_states() ;
                }
                else if(parser->is_rejected_with_ready()) {
                    g_funcfinder.backward_parser_states(1) ;
                    g_ntlgr.remove_from_back(1) ;
                }
            }
            else {
                g_ntlgr.ignore() ;
                g_funcfinder.reset_parser_states() ;

                if(g_ntlgr.get_head_num() == 0) {
                    VirtualCmdLine::refresh() ;
                }
            }
        }
    }
}
