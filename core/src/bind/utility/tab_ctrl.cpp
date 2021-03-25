#include "tab_ctrl.hpp"

#include <windows.h>

#include "io/keybrd_eventer.hpp"
#include "mode/change_mode.hpp"
#include "msg_logger.hpp"
#include "utility.hpp"


namespace vind
{
    //Switch2LeftTab
    const std::string Switch2LeftTab::sname() noexcept {
        return "switch_to_left_tab" ;
    }

    void Switch2LeftTab::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(VKC_LCTRL, VKC_LSHIFT, VKC_TAB) ;
        }
    }


    //Switch2RightTab
    const std::string Switch2RightTab::sname() noexcept {
        return "switch_to_right_tab" ;
    }

    void Switch2RightTab::sprocess(
            const bool first_call,
            unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(VKC_LCTRL, VKC_TAB) ;
        }
    }

    //OpenNewTab
    const std::string OpenNewTab::sname() noexcept {
        return "open_new_tab" ;
    }

    void OpenNewTab::sprocess(
            const bool first_call,
            unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        keybrd::pushup(VKC_LCTRL, VKC_T) ;
    }



    //CloseCurrentTab
    const std::string CloseCurrentTab::sname() noexcept {
        return "close_current_tab" ;
    }

    void CloseCurrentTab::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        keybrd::pushup(VKC_LCTRL, VKC_F4) ;

        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("The foreground window is not existed.") ;
        }

        Sleep(500) ; //wait by openning the dialog for saving
        if(hwnd != GetForegroundWindow()) { //opened popup
            Change2Normal::sprocess(true, 1, nullptr, nullptr) ;
        }
    }
}
