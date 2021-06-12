#include "bind/file/filer.hpp"

#include <windows.h>

#include <memory>

#include "bind/mode/change_mode.hpp"
#include "io/keybrd.hpp"
#include "key/char_logger.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"

namespace vind
{
    //Save
    Save::Save()
    : BindedFuncCreator("save")
    {}
    void Save::sprocess() {
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_S) ;
    }
    void Save::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Save::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //Open
    Open::Open()
    : BindedFuncCreator("open")
    {}
    void Open::sprocess() {
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_O) ;
    }
    void Open::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Open::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;

    }
}
