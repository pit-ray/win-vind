#include "undo.hpp"

#include <windows.h>

#include "bind/saferepeat.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/mouse.hpp"


namespace vind
{
    namespace bind
    {
        //Redo
        Redo::Redo()
        : BindedFuncVoid("redo")
        {}
        void Redo::sprocess(
                std::uint16_t count, const std::string& args) {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_Y) ;
            }) ;
        }

        //Undo
        Undo::Undo()
        : BindedFuncVoid("undo")
        {}
        void Undo::sprocess(
                std::uint16_t count, const std::string& args) {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_Z) ;
            }) ;
        }
    }
}
