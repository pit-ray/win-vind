#include "bind/ctrl/mywindow_ctrl.hpp"

#include <windows.h>

#include "bind/mouse/jump_actwin.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"

namespace
{
    std::function<void()> show_func = [] {return ;} ;
    std::function<void()> exit_func = [] {return ;} ;
}

namespace vind
{
    //ShowConfigWindow
    ShowConfigWindow::ShowConfigWindow()
    : BindedFuncCreator("show_config_window")
    {}
    void ShowConfigWindow::sprocess() {
        show_func() ;
        Sleep(50) ; //wait until opened window.
        Jump2ActiveWindow::sprocess() ;
    }
    void ShowConfigWindow::sprocess(NTypeLogger& parent_lgr) {
        if(parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void ShowConfigWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
    void ShowConfigWindow::register_show_func(std::function<void()> func) noexcept {
        show_func = std::move(func) ;
    }


    //ExitConfigWindow
    ExitConfigWindow::ExitConfigWindow()
    : BindedFuncCreator("exit_config_window")
    {}
    void ExitConfigWindow::sprocess() {
        exit_func() ;
    }
    void ExitConfigWindow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void ExitConfigWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
    void ExitConfigWindow::register_exit_func(std::function<void()> func) noexcept {
        exit_func = std::move(func) ;
    }
}
