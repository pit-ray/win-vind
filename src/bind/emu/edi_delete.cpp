#include "edi_delete.hpp"

#include "bind/mode/change_mode.hpp"
#include "bind/safe_repeater.hpp"
#include "core/g_params.hpp"
#include "core/keycode_def.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "simple_text_register.hpp"
#include "text_util.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"
#include "util/keystroke_repeater.hpp"


namespace vind
{
    //DeleteLine
    struct DeleteLine::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    DeleteLine::DeleteLine()
    : BindedFuncCreator("delete_line"),
      pimpl(std::make_unique<Impl>())
    {}

    DeleteLine::~DeleteLine() noexcept                  = default ;
    DeleteLine::DeleteLine(DeleteLine&&)            = default ;
    DeleteLine& DeleteLine::operator=(DeleteLine&&) = default ;

    void DeleteLine::sprocess(unsigned int repeat_num) const {
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
    void DeleteLine::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void DeleteLine::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //DeleteLineUntilEOL
    struct DeleteLineUntilEOL::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    DeleteLineUntilEOL::DeleteLineUntilEOL()
    : BindedFuncCreator("delete_line_until_EOL"),
      pimpl(std::make_unique<Impl>())
    {}

    DeleteLineUntilEOL::~DeleteLineUntilEOL() noexcept                          = default ;
    DeleteLineUntilEOL::DeleteLineUntilEOL(DeleteLineUntilEOL&&)            = default ;
    DeleteLineUntilEOL& DeleteLineUntilEOL::operator=(DeleteLineUntilEOL&&) = default ;

    void DeleteLineUntilEOL::sprocess(unsigned int repeat_num) const {
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
    void DeleteLineUntilEOL::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void DeleteLineUntilEOL::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //DeleteAfter
    struct DeleteAfter::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    DeleteAfter::DeleteAfter()
    : BindedFuncCreator("delete_after"),
      pimpl(std::make_unique<Impl>())
    {}

    DeleteAfter::~DeleteAfter() noexcept                   = default ;
    DeleteAfter::DeleteAfter(DeleteAfter&&)            = default ;
    DeleteAfter& DeleteAfter::operator=(DeleteAfter&&) = default ;

    void DeleteAfter::sprocess(unsigned int repeat_num) const {
        if(gparams::get_b("charcache")) {
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
    void DeleteAfter::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void DeleteAfter::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //DeleteBefore
    struct DeleteBefore::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    DeleteBefore::DeleteBefore()
    : BindedFuncCreator("delete_before"),
      pimpl(std::make_unique<Impl>())
    {}

    DeleteBefore::~DeleteBefore() noexcept                    = default ;
    DeleteBefore::DeleteBefore(DeleteBefore&&)            = default ;
    DeleteBefore& DeleteBefore::operator=(DeleteBefore&&) = default ;

    void DeleteBefore::sprocess(unsigned int repeat_num) const {
        if(gparams::get_b("charcache")) {
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
    void DeleteBefore::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void DeleteBefore::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //DeleteHighlightText (Visual only)
    DeleteHighlightText::DeleteHighlightText()
    : BindedFuncCreator("delete_highlight_text")
    {}
    void DeleteHighlightText::sprocess() {
        using namespace mode ;
        using keybrd::pushup ;
        using namespace simpletxreg ;

        pushup(KEYCODE_LCTRL, KEYCODE_X) ;
        if(get_global_flags() & ModeFlags::VISUAL_LINE) {
            set_register_type(RegType::Lines) ;
        }
        else {
            set_register_type(RegType::Chars) ;
        }
        ToEdiNormal::sprocess(false) ;
    }
    void DeleteHighlightText::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void DeleteHighlightText::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
