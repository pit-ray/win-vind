#ifndef _MYWINDOW_CTRL_HPP
#define _MYWINDOW_CTRL_HPP

#include "bind/base/binded_func_with_creator.hpp"
#include <functional>

namespace vind
{
    struct ShowConfigWindow : public BindedFuncCreator<ShowConfigWindow> {
        explicit ShowConfigWindow() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;

        static void register_show_func(std::function<void()> func) noexcept ;
    } ;


    struct ExitConfigWindow : public BindedFuncCreator<ExitConfigWindow> {
        explicit ExitConfigWindow() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;

        static void register_exit_func(std::function<void()> func) noexcept ;
    } ;
}

#endif
