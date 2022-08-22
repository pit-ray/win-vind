#ifndef _MYWINDOW_CTRL_HPP
#define _MYWINDOW_CTRL_HPP

#include "bind/bindedfunc.hpp"
#include <functional>

namespace vind
{
    namespace bind
    {
        struct ShowConfigGUI : public BindedFuncVoid<ShowConfigGUI> {
            explicit ShowConfigGUI() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct ExitConfigGUI : public BindedFuncFlex<ExitConfigGUI> {
            explicit ExitConfigGUI() ;
            static SystemCall sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
