#include "clipboard.hpp"

#include <windows.h>

#include "bind/emu/edi_change_mode.hpp"
#include "bind/mode/change_mode.hpp"
#include "core/char_logger.hpp"
#include "core/key_absorber.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"
#include "util/mouse.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    namespace bind
    {
        //HotkeyCopy
        HotkeyCopy::HotkeyCopy()
        : BindedFuncCreator("hotkey_copy")
        {}
        void HotkeyCopy::sprocess() {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //there are cases in which not editable.
            //thus, not use Message Copy
            util::pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        }
        void HotkeyCopy::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void HotkeyCopy::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //HotkeyPaste
        HotkeyPaste::HotkeyPaste()
        : BindedFuncCreator("hotkey_paste")
        {}
        void HotkeyPaste::sprocess() {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //not selecting at paste.
            util::pushup(KEYCODE_LCTRL, KEYCODE_V) ;
        }
        void HotkeyPaste::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void HotkeyPaste::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //HotkeyCut
        HotkeyCut::HotkeyCut()
        : BindedFuncCreator("hotkey_cut")
        {}
        void HotkeyCut::sprocess() {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;
            util::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
        }
        void HotkeyCut::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void HotkeyCut::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //HotkeyDelete
        HotkeyDelete::HotkeyDelete()
        : BindedFuncCreator("hotkey_delete")
        {}
        void HotkeyDelete::sprocess() {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //selecting->cut
            //unselecting->delete
            util::pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            util::pushup(KEYCODE_DELETE) ;
        }
        void HotkeyDelete::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void HotkeyDelete::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //HotkeyBackspace
        HotkeyBackspace::HotkeyBackspace()
        : BindedFuncCreator("hotkey_backspace")
        {}

        void HotkeyBackspace::sprocess() {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //selecting->cut
            //unselecting->delete
            util::pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            util::pushup(KEYCODE_BKSPACE) ;
        }
        void HotkeyBackspace::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void HotkeyBackspace::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
