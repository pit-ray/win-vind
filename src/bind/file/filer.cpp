#include "filer.hpp"

#include <windows.h>

#include <memory>

#include "bind/mode/change_mode.hpp"
#include "core/charlogger.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        //Save
        Save::Save()
        : BindedFuncVoid("save")
        {}
        void Save::sprocess(
                std::uint16_t count, const std::string& args) {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_S) ;
        }

        //Open
        Open::Open()
        : BindedFuncVoid("open")
        {}
        void Open::sprocess(
                std::uint16_t count, const std::string& args) {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_O) ;
        }
    }
}
