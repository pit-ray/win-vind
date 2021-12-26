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
        void Save::sprocess() {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_S) ;
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
        : BindedFuncVoid("open")
        {}
        void Open::sprocess() {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_O) ;
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
