#include "bind/emu/edi_put.hpp"

#include "bind/emu/edi_change_mode.hpp"
#include "bind/emu/edi_delete.hpp"
#include "bind/emu/simple_text_register.hpp"
#include "bind/safe_repeater.hpp"
#include "io/keybrd.hpp"
#include "key/keycode_def.hpp"
#include "key/ntype_logger.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/def.hpp"

namespace vind
{
    //EdiNPasteAfter (EdiNormal or EdiVisual)
    struct EdiNPasteAfter::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNPasteAfter::EdiNPasteAfter()
    : BindedFuncCreator("edi_n_paste_after"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNPasteAfter::~EdiNPasteAfter() noexcept                  = default ;
    EdiNPasteAfter::EdiNPasteAfter(EdiNPasteAfter&&)            = default ;
    EdiNPasteAfter& EdiNPasteAfter::operator=(EdiNPasteAfter&&) = default ;

    void EdiNPasteAfter::sprocess(unsigned int repeat_num) const {
        using keybrd::pushup ;
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
    void EdiNPasteAfter::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNPasteAfter::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiNPasteBefore
    struct EdiNPasteBefore::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNPasteBefore::EdiNPasteBefore()
    : BindedFuncCreator("edi_n_paste_before"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNPasteBefore::~EdiNPasteBefore() noexcept                   = default ;
    EdiNPasteBefore::EdiNPasteBefore(EdiNPasteBefore&&)            = default ;
    EdiNPasteBefore& EdiNPasteBefore::operator=(EdiNPasteBefore&&) = default ;

    void EdiNPasteBefore::sprocess(unsigned int repeat_num) const {
        using keybrd::pushup ;
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
    void EdiNPasteBefore::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNPasteBefore::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiDeleteHighlightText (Visual only)
    EdiDeleteHighlightText::EdiDeleteHighlightText()
    : BindedFuncCreator("edi_delete_highlight_text")
    {}
    void EdiDeleteHighlightText::sprocess() {
        using namespace mode ;
        using keybrd::pushup ;
        using namespace simpletxreg ;

        pushup(KEYCODE_LCTRL, KEYCODE_X) ;
        if(get_global_mode() == Mode::EdiLineVisual) {
            set_register_type(RegType::Lines) ;
        }
        else {
            set_register_type(RegType::Chars) ;
        }
        Change2EdiNormal::sprocess(false) ;
    }
    void EdiDeleteHighlightText::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EdiDeleteHighlightText::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
