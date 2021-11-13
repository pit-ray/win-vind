#include "edi_put.hpp"

#include "bind/safe_repeater.hpp"
#include "core/keycode_def.hpp"
#include "core/ntype_logger.hpp"
#include "edi_delete.hpp"
#include "simple_text_register.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"
#include "util/keystroke_repeater.hpp"


namespace vind
{
    //PutAfter (EdiNormal or EdiVisual)
    struct PutAfter::Impl {
        util::KeyStrokeRepeater ksr{} ;
    } ;

    PutAfter::PutAfter()
    : BindedFuncCreator("put_after"),
      pimpl(std::make_unique<Impl>())
    {}

    PutAfter::~PutAfter() noexcept            = default ;
    PutAfter::PutAfter(PutAfter&&)            = default ;
    PutAfter& PutAfter::operator=(PutAfter&&) = default ;

    void PutAfter::sprocess(unsigned int repeat_num) const {
        using util::pushup ;
        using namespace simpletxreg ;
        auto t = get_register_type() ;
        if(t == RegType::Chars) {
            pushup(KEYCODE_RIGHT) ;

            repeater::safe_for(repeat_num, [] {
                pushup(KEYCODE_LCTRL, KEYCODE_V) ;
            }) ;
        }
        else if(t == RegType::Lines) {
            pushup(KEYCODE_END) ;
            repeater::safe_for(repeat_num, [] {
                pushup(KEYCODE_ENTER) ;
                pushup(KEYCODE_LCTRL, KEYCODE_V) ;
            }) ;
        }
    }
    void PutAfter::sprocess(core::NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void PutAfter::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //PutBefore
    struct PutBefore::Impl {
        util::KeyStrokeRepeater ksr{} ;
    } ;

    PutBefore::PutBefore()
    : BindedFuncCreator("put_before"),
      pimpl(std::make_unique<Impl>())
    {}

    PutBefore::~PutBefore() noexcept                   = default ;
    PutBefore::PutBefore(PutBefore&&)            = default ;
    PutBefore& PutBefore::operator=(PutBefore&&) = default ;

    void PutBefore::sprocess(unsigned int repeat_num) const {
        using util::pushup ;
        using namespace simpletxreg ;
        auto t = get_register_type() ;
        if(t == RegType::Chars) {
            repeater::safe_for(repeat_num, [] {
                pushup(KEYCODE_LCTRL, KEYCODE_V) ;
            }) ;
        }
        else if(t == RegType::Lines) {
            repeater::safe_for(repeat_num, [] {
                pushup(KEYCODE_HOME) ;
                pushup(KEYCODE_ENTER) ;
                pushup(KEYCODE_UP) ;
                pushup(KEYCODE_LCTRL, KEYCODE_V) ;
            }) ;
        }
    }
    void PutBefore::sprocess(core::NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void PutBefore::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }
}
