#include "mywindow_ctrl.hpp"

#include <windows.h>

#include "bind/mouse/jump_actwin.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"

namespace
{
    std::function<void()> show_func = [] {return ;} ;
    std::function<void()> exit_func = [] {return ;} ;
}

namespace vind
{
    //ShowConfigGUI
    ShowConfigGUI::ShowConfigGUI()
    : BindedFuncCreator("show_config_gui")
    {}
    void ShowConfigGUI::sprocess() {
        show_func() ;
        Sleep(50) ; //wait until opened window.
        JumpToActiveWindow::sprocess() ;
    }
    void ShowConfigGUI::sprocess(NTypeLogger& parent_lgr) {
        if(parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void ShowConfigGUI::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
    void ShowConfigGUI::register_show_func(std::function<void()> func) noexcept {
        show_func = std::move(func) ;
    }


    //ExitConfigGUI
    ExitConfigGUI::ExitConfigGUI()
    : BindedFuncCreator("exit_config_gui")
    {}
    void ExitConfigGUI::sprocess() {
        exit_func() ;
    }
    void ExitConfigGUI::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void ExitConfigGUI::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
    void ExitConfigGUI::register_exit_func(std::function<void()> func) noexcept {
        exit_func = std::move(func) ;
    }
}
