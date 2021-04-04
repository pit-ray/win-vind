#include "global_bindings_caller.hpp"

#include <iostream>

#include "key/key_logger_base.hpp"
#include "key/keycode_def.hpp"
#include "key/ntype_logger.hpp"
#include "opt/virtual_cmd_line.hpp"

#include "bind.hpp"
#include "binded_func.hpp"

//internal linkage
namespace
{
    vind::BindedFunc::shp_t g_running_func = nullptr ;
    vind::NTypeLogger ntlgr{} ;
}

namespace vind
{
    namespace gbindcaller {

        void initialize() {
            ntlgr.clear() ;

            g_running_func = nullptr ;
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

            if(auto func = vind::keybind::find_func(ntlgr, g_running_func, true)) {
                if(func->is_callable()) {
                    if(!ntlgr.is_pressing()) {
                        VirtualCmdLine::reset() ;
                        g_running_func = func ;
                        ntlgr.accept() ;
                    }

                    // If the keys more than two are long pressing,
                    // continue calling a first selected function to prohibit calling the functions alternately.
                    g_running_func->process(!ntlgr.is_pressing(), ntlgr.get_head_num(), nullptr, nullptr) ;
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
