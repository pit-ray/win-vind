#include "bind/emu/edi_yank.hpp"

#include "bind/emu/edi_change_mode.hpp"
#include "bind/emu/simple_text_register.hpp"
#include "bind/emu/text_util.hpp"
#include "bind/safe_repeater.hpp"
#include "io/keybrd.hpp"
#include "key/keycode_def.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"


namespace vind
{
    //EdiCopyHighlightText (EdiVisual only)
    EdiCopyHighlightText::EdiCopyHighlightText()
    : BindedFuncCreator("edi_copy_highlight_text")
    {}
    void EdiCopyHighlightText::sprocess() {
        using namespace mode ;
        using namespace simpletxreg ;

        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_C) ;

        if(get_global_mode() == Mode::EdiLineVisual) {
            set_register_type(RegType::Lines) ;
        }
        else {
            set_register_type(RegType::Chars) ;
        }

        keybrd::pushup(KEYCODE_LEFT) ; //unselect, but this is for the time being
        Change2EdiNormal::sprocess(true) ;
    }
    void EdiCopyHighlightText::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EdiCopyHighlightText::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //EdiNCopyLine (EdiNormal only)
    EdiNCopyLine::EdiNCopyLine()
    : BindedFuncCreator("edi_n_copy_line")
    {}
    void EdiNCopyLine::sprocess(
            unsigned int repeat_num,
            const textanalyze::SelRes* const exres) {

        using keybrd::pushup ;
        pushup(KEYCODE_HOME) ;

        //copy N - 1 lines
        repeater::safe_for(repeat_num - 1, [] {
            pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
        }) ;

        if(!txutil::select_line_until_EOL(exres)) {
            txutil::clear_clipboard_with_null() ;
        }

        pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        pushup(KEYCODE_END) ;

        simpletxreg::set_register_type(simpletxreg::RegType::Lines) ;
    }
    void EdiNCopyLine::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiNCopyLine::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1, nullptr) ;
    }
}
