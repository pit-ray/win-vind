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
        struct Redo::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        Redo::Redo()
        : BindedFuncVoid("redo"),
          pimpl(std::make_unique<Impl>())
        {}

        Redo::~Redo() noexcept          = default ;
        Redo::Redo(Redo&&)            = default ;
        Redo& Redo::operator=(Redo&&) = default ;

        void Redo::sprocess(unsigned int count) const {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_Y) ;
            }) ;
        }
        void Redo::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void Redo::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess() ;
        }


        //Undo
        struct Undo::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        Undo::Undo()
        : BindedFuncVoid("undo"),
          pimpl(std::make_unique<Impl>())
        {}

        Undo::~Undo() noexcept          = default ;
        Undo::Undo(Undo&&)            = default ;
        Undo& Undo::operator=(Undo&&) = default ;

        void Undo::sprocess(unsigned int count) const {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_Z) ;
            }) ;
        }
        void Undo::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void Undo::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess() ;
        }
    }
}
