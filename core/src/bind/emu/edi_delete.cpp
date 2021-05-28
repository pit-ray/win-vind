#include "bind/emu/edi_delete.hpp"

#include "bind/emu/simple_text_register.hpp"
#include "bind/emu/text_util.hpp"
#include "bind/safe_repeater.hpp"
#include "g_params.hpp"
#include "io/keybrd.hpp"
#include "key/keycode_def.hpp"
#include "key/ntype_logger.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/def.hpp"


namespace vind
{
    //EdiNDeleteLine
    struct EdiNDeleteLine::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteLine::EdiNDeleteLine()
    : BindedFuncCreator("edi_n_delete_line"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteLine::~EdiNDeleteLine() noexcept                  = default ;
    EdiNDeleteLine::EdiNDeleteLine(EdiNDeleteLine&&)            = default ;
    EdiNDeleteLine& EdiNDeleteLine::operator=(EdiNDeleteLine&&) = default ;

    void EdiNDeleteLine::sprocess(unsigned int repeat_num) const {
        using keybrd::pushup ;
        pushup(KEYCODE_HOME) ;

        repeater::safe_for(repeat_num - 1, [] {
            pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
        }) ;

        if(!txutil::select_line_until_EOL(nullptr)) {
            txutil::clear_clipboard_with_null() ;
        }
        else {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            simpletxreg::set_register_type(simpletxreg::RegType::Lines) ;
            pushup(KEYCODE_DELETE) ;
        }
    }
    void EdiNDeleteLine::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNDeleteLine::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiNDeleteLineUntilEOL
    struct EdiNDeleteLineUntilEOL::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteLineUntilEOL::EdiNDeleteLineUntilEOL()
    : BindedFuncCreator("edi_n_delete_line_until_EOL"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteLineUntilEOL::~EdiNDeleteLineUntilEOL() noexcept                          = default ;
    EdiNDeleteLineUntilEOL::EdiNDeleteLineUntilEOL(EdiNDeleteLineUntilEOL&&)            = default ;
    EdiNDeleteLineUntilEOL& EdiNDeleteLineUntilEOL::operator=(EdiNDeleteLineUntilEOL&&) = default ;

    void EdiNDeleteLineUntilEOL::sprocess(unsigned int repeat_num) const {
        using keybrd::pushup ;

        //delete N - 1 lines under the current line
        repeater::safe_for(repeat_num - 1, [] {
            pushup(KEYCODE_DOWN) ;
        }) ;

        if(!txutil::select_line_until_EOL(nullptr)) {
            txutil::clear_clipboard_with_null() ;
        }
        else {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            simpletxreg::set_register_type(simpletxreg::RegType::Chars) ;
        }
    }
    void EdiNDeleteLineUntilEOL::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNDeleteLineUntilEOL::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiNDeleteAfter
    struct EdiNDeleteAfter::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteAfter::EdiNDeleteAfter()
    : BindedFuncCreator("edi_n_delete_after"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteAfter::~EdiNDeleteAfter() noexcept                   = default ;
    EdiNDeleteAfter::EdiNDeleteAfter(EdiNDeleteAfter&&)            = default ;
    EdiNDeleteAfter& EdiNDeleteAfter::operator=(EdiNDeleteAfter&&) = default ;

    void EdiNDeleteAfter::sprocess(unsigned int repeat_num) const {
        if(gparams::get_b("enable_char_cache")) {
            repeater::safe_for(repeat_num, [] {
                    keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                    keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                    simpletxreg::set_register_type(simpletxreg::RegType::Chars) ;
            }) ;
        }
        else {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_DELETE) ;
            }) ;
        }
    }
    void EdiNDeleteAfter::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNDeleteAfter::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiNDeleteBefore
    struct EdiNDeleteBefore::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteBefore::EdiNDeleteBefore()
    : BindedFuncCreator("edi_n_delete_before"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteBefore::~EdiNDeleteBefore() noexcept                    = default ;
    EdiNDeleteBefore::EdiNDeleteBefore(EdiNDeleteBefore&&)            = default ;
    EdiNDeleteBefore& EdiNDeleteBefore::operator=(EdiNDeleteBefore&&) = default ;

    void EdiNDeleteBefore::sprocess(unsigned int repeat_num) const {
        if(gparams::get_b("enable_char_cache")) {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                simpletxreg::set_register_type(simpletxreg::RegType::Chars) ;
            }) ;
        }
        else {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_BKSPACE) ;
            }) ;
        }
    }
    void EdiNDeleteBefore::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNDeleteBefore::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }
}
