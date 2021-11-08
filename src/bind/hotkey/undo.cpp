#include "undo.hpp"

#include <windows.h>

#include "bind/safe_repeater.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/mouse.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    //Redo
    struct Redo::Impl {
        util::KeyStrokeRepeater ksr{} ;
    } ;

    Redo::Redo()
    : BindedFuncCreator("redo"),
      pimpl(std::make_unique<Impl>())
    {}

    Redo::~Redo() noexcept          = default ;
    Redo::Redo(Redo&&)            = default ;
    Redo& Redo::operator=(Redo&&) = default ;

    void Redo::sprocess(unsigned int repeat_num) const {
        repeater::safe_for(repeat_num, [] {
            util::pushup(KEYCODE_LCTRL, KEYCODE_Y) ;
        }) ;
    }
    void Redo::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void Redo::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    //Undo
    struct Undo::Impl {
        util::KeyStrokeRepeater ksr{} ;
    } ;

    Undo::Undo()
    : BindedFuncCreator("undo"),
      pimpl(std::make_unique<Impl>())
    {}

    Undo::~Undo() noexcept          = default ;
    Undo::Undo(Undo&&)            = default ;
    Undo& Undo::operator=(Undo&&) = default ;

    void Undo::sprocess(unsigned int repeat_num) const {
        repeater::safe_for(repeat_num, [] {
            util::pushup(KEYCODE_LCTRL, KEYCODE_Z) ;
        }) ;
    }
    void Undo::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void Undo::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
}
