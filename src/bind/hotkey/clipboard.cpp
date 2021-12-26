#include "clipboard.hpp"

#include <windows.h>

#include "bind/emu/moveinsert.hpp"
#include "bind/mode/change_mode.hpp"
#include "core/charlogger.hpp"
#include "core/inputgate.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/mouse.hpp"


namespace vind
{
    namespace bind
    {
        //HotkeyCopy
        HotkeyCopy::HotkeyCopy()
        : BindedFuncVoid("hotkey_copy")
        {}
        void HotkeyCopy::sprocess() {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //there are cases in which not editable.
            //thus, not use Message Copy
            core::InputGate::get_instance().pushup(KEYCODE_LCTRL, KEYCODE_C) ;
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
        : BindedFuncVoid("hotkey_paste")
        {}
        void HotkeyPaste::sprocess() {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //not selecting at paste.
            core::InputGate::get_instance().pushup(KEYCODE_LCTRL, KEYCODE_V) ;
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
        : BindedFuncVoid("hotkey_cut")
        {}
        void HotkeyCut::sprocess() {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;
            core::InputGate::get_instance().pushup(KEYCODE_LCTRL, KEYCODE_X) ;
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
        : BindedFuncVoid("hotkey_delete")
        {}
        void HotkeyDelete::sprocess() {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //selecting->cut
            //unselecting->delete
            auto& igate = core::InputGate::get_instance() ;
            igate.pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            igate.pushup(KEYCODE_DELETE) ;
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
        : BindedFuncVoid("hotkey_backspace")
        {}

        void HotkeyBackspace::sprocess() {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //selecting->cut
            //unselecting->delete
            auto& igate = core::InputGate::get_instance() ;
            igate.pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            igate.pushup(KEYCODE_BKSPACE) ;
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
