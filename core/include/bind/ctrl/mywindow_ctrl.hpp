#ifndef _MYWINDOW_CTRL_HPP
#define _MYWINDOW_CTRL_HPP

#include "bind/binded_func_creator.hpp"
#include <functional>

namespace vind
{
    struct ShowConfigGUI : public BindedFuncCreator<ShowConfigGUI> {
        explicit ShowConfigGUI() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;

        static void register_show_func(std::function<void()> func) noexcept ;
    } ;


    struct ExitConfigGUI : public BindedFuncCreator<ExitConfigGUI> {
        explicit ExitConfigGUI() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;

        static void register_exit_func(std::function<void()> func) noexcept ;
    } ;
}

#endif
