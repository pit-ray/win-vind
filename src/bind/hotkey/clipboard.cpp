#include "clipboard.hpp"

#include <windows.h>

#include "bind/emu/moveinsert.hpp"
#include "bind/mode/change_mode.hpp"
#include "core/inputgate.hpp"
#include "core/mode.hpp"
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
        void HotkeyCopy::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //there are cases in which not editable.
            //thus, not use Message Copy
            core::InputGate::get_instance().pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        }

        //HotkeyPaste
        HotkeyPaste::HotkeyPaste()
        : BindedFuncVoid("hotkey_paste")
        {}
        void HotkeyPaste::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //not selecting at paste.
            core::InputGate::get_instance().pushup(KEYCODE_LCTRL, KEYCODE_V) ;
        }

        //HotkeyCut
        HotkeyCut::HotkeyCut()
        : BindedFuncVoid("hotkey_cut")
        {}
        void HotkeyCut::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;
            core::InputGate::get_instance().pushup(KEYCODE_LCTRL, KEYCODE_X) ;
        }

        //HotkeyDelete
        HotkeyDelete::HotkeyDelete()
        : BindedFuncVoid("hotkey_delete")
        {}
        void HotkeyDelete::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //selecting->cut
            //unselecting->delete
            auto& igate = core::InputGate::get_instance() ;
            igate.pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            igate.pushup(KEYCODE_DELETE) ;
        }

        //HotkeyBackspace
        HotkeyBackspace::HotkeyBackspace()
        : BindedFuncVoid("hotkey_backspace")
        {}
        void HotkeyBackspace::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            util::release_mousestate(KEYCODE_MOUSE_LEFT) ;

            //selecting->cut
            //unselecting->delete
            auto& igate = core::InputGate::get_instance() ;
            igate.pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            igate.pushup(KEYCODE_BKSPACE) ;
        }
    }
}
