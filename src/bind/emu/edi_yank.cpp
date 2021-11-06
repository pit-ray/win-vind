#include "edi_yank.hpp"

#include "bind/mode/change_mode.hpp"
#include "bind/safe_repeater.hpp"
#include "core/keycode_def.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "simple_text_register.hpp"
#include "text_util.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"


namespace vind
{
    //YankHighlightText (EdiVisual only)
    YankHighlightText::YankHighlightText()
    : BindedFuncCreator("yank_highlight_text")
    {}
    void YankHighlightText::sprocess() {
        using namespace mode ;
        using namespace simpletxreg ;

        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_C) ;

        if(get_global_flags() == ModeFlags::VISUAL_LINE) {
            set_register_type(RegType::Lines) ;
        }
        else {
            set_register_type(RegType::Chars) ;
        }

        keybrd::pushup(KEYCODE_LEFT) ; //unselect, but this is for the time being
        ToEdiNormal::sprocess(true) ;
    }
    void YankHighlightText::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void YankHighlightText::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //YankLine (EdiNormal only)
    YankLine::YankLine()
    : BindedFuncCreator("yank_line")
    {}
    void YankLine::sprocess(
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
    void YankLine::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void YankLine::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1, nullptr) ;
    }
}
