#include "bind/global_bindings_caller.hpp"

#include <iostream>

#include "bind/base/key_logger_base.hpp"
#include "key/keycode_def.hpp"
#include "bind/base/ntype_logger.hpp"
#include "opt/virtual_cmd_line.hpp"

#include "bind/bind.hpp"
#include "bind/base/binded_func.hpp"

#include "bind/logger_parser.hpp"
#include "bind/mode/change_mode.hpp"

//internal linkage
namespace
{
    vind::BindedFunc::shp_t g_running_func = nullptr ;
    vind::NTypeLogger ntlgr{} ;

    vind::LoggerParser g_parser ;
}

namespace vind
{
    namespace gbindcaller {

        void initialize() {
            ntlgr.clear() ;

            g_running_func = nullptr ;

            g_parser.append_binding("a<any-a>bb") ;
        }

        void call_matched_func() {
            auto result = ntlgr.logging_state() ;
            if(NTYPE_EMPTY(result)) {
                g_running_func = nullptr ;
                return ;
            }
            if(NTYPE_HEAD_NUM(result)) {
                VirtualCmdLine::cout(std::to_string(ntlgr.get_head_num())) ;
                return ;
            }
            /*
            std::cout << "######################################################\n" ;


            if(ntlgr.latest().is_containing(KEYCODE_BKSPACE)) {
                g_parser.undo_state(1) ;
                ntlgr.remove_from_back(2) ;
                std::cout << "\tundo\n" ;
            }
            else if(ntlgr.latest().is_containing(KEYCODE_ESC)) {
                g_parser.reset_state() ;
                ntlgr.clear() ;
                std::cout << "\tclear\n" ;
                return ;
            }
            else {

                // test
                auto res = g_parser.validate_if_match(ntlgr.latest()) ;
                std::cout << "\tres : " << static_cast<int>(res) << std::endl ;
            }
            std::cout << keyloggerutil::debug::print_log(ntlgr) ;
            std::cout << "\tsize   : " << g_parser.state_stack_size() << std::endl ;

            if(g_parser.is_accepted()) {
                std::cout << "\taccept\n" ;
            }
            else if(g_parser.is_rejected()) {
                std::cout << "\treject\n" ;
            }
            else if(g_parser.is_waiting()) {
                std::cout << "\twaiting\n" ;
            }
            else if(g_parser.is_rejected_with_headsys_ready()) {
                std::cout << "\treject with head\n" ;
                g_parser.undo_state(1) ;
                ntlgr.remove_from_back(1) ;
            }
            */

            //-----------------------

            if(auto func = vind::keybind::find_func(ntlgr, g_running_func, true)) {
                if(func->is_callable()) {
                    if(!ntlgr.is_long_pressing()) {
                        VirtualCmdLine::reset() ;
                        g_running_func = func ;
                        ntlgr.accept() ;
                    }

                    // If the keys more than two are long pressing,
                    // continue calling a first selected function to prohibit calling the functions alternately.
                    g_running_func->process(!ntlgr.is_long_pressing(), ntlgr.get_head_num(), nullptr, nullptr) ;
                    return ;
                }

                if(func->is_matched_syskey_in_combined_bindings()) {
                    ntlgr.remove_from_back(1) ;
                    g_running_func = nullptr ;
                }
            }
            else {
                ntlgr.ignore() ;
                if(ntlgr.get_head_num() == 0) {
                    VirtualCmdLine::refresh() ;
                }
                g_running_func = nullptr ;
            }
        }
    }
}
