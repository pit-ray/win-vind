#include "filer.hpp"

#include <windows.h>

#include <memory>

#include "change_mode.hpp"
#include "keybrd_eventer.hpp"
#include "utility.hpp"

namespace vind
{
    //SaveOpenedFile
    const std::string SaveOpenedFile::sname() noexcept {
        return "save_opened_file" ;
    }

    void SaveOpenedFile::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            RUNTIME_EXCEPT("The foreground window is not existed.") ;
        }

        KeybrdEventer::pushup(VKC_LCTRL, VKC_S) ;

        Sleep(500) ; //wait by openning the dialog for saving
        if(hwnd != GetForegroundWindow()) { //opened popup
            Change2Normal::sprocess(true, 1, nullptr, nullptr) ;
        }
    }


    //OpenOtherFile
    const std::string OpenOtherFile::sname() noexcept {
        return "open_other_file" ;
    }

    void OpenOtherFile::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        Change2Normal::sprocess(true, 1, nullptr, nullptr) ;
        KeybrdEventer::pushup(VKC_LCTRL, VKC_O) ;
    }
}
