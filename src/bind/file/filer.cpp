#include "filer.hpp"

#include <windows.h>

#include <memory>

#include "bind/mode/change_mode.hpp"
#include "core/char_logger.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"

namespace vind
{
    namespace bind
    {
        //Save
        Save::Save()
        : BindedFuncCreator("save")
        {}
        void Save::sprocess() {
            util::pushup(KEYCODE_LCTRL, KEYCODE_S) ;
        }
        void Save::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void Save::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //Open
        Open::Open()
        : BindedFuncCreator("open")
        {}
        void Open::sprocess() {
            util::pushup(KEYCODE_LCTRL, KEYCODE_O) ;
        }
        void Open::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void Open::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;

        }
    }
}
