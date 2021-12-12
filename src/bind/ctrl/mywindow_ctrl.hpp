#ifndef _MYWINDOW_CTRL_HPP
#define _MYWINDOW_CTRL_HPP

#include "bind/binded_func.hpp"
#include <functional>

namespace vind
{
    namespace bind
    {
        struct ShowConfigGUI : public BindedFuncVoid<ShowConfigGUI> {
            explicit ShowConfigGUI() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ExitConfigGUI : public BindedFuncFlex<ExitConfigGUI> {
            explicit ExitConfigGUI() ;
            static SystemCall sprocess() ;
            static SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
