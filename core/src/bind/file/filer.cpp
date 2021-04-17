#include "bind/file/filer.hpp"

#include <windows.h>

#include <memory>

#include "bind/base/char_logger.hpp"
#include "bind/base/ntype_logger.hpp"
#include "bind/mode/change_mode.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

namespace vind
{
    //SaveOpenedFile
    SaveOpenedFile::SaveOpenedFile()
    : BindedFuncCreator("save_opened_file")
    {}
    void SaveOpenedFile::sprocess() {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            RUNTIME_EXCEPT("The foreground window is not existed.") ;
        }

        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_S) ;

        Sleep(500) ; //wait by openning the dialog for saving
        if(hwnd != GetForegroundWindow()) { //opened popup
            Change2Normal::sprocess(true) ;
        }
    }
    void SaveOpenedFile::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SaveOpenedFile::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //OpenOtherFile
    OpenOtherFile::OpenOtherFile()
    : BindedFuncCreator("open_other_file")
    {}
    void OpenOtherFile::sprocess() {
        Change2Normal::sprocess(true) ;
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_O) ;
    }
    void OpenOtherFile::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void OpenOtherFile::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;

    }
}
