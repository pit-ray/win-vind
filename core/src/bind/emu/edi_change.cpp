#include "bind/emu/edi_change.hpp"

#include "bind/emu/edi_change_mode.hpp"
#include "bind/emu/simple_text_register.hpp"
#include "bind/emu/text_util.hpp"
#include "bind/safe_repeater.hpp"
#include "g_params.hpp"
#include "io/keybrd.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"

namespace vind
{
    //EdiChangeHighlightText (Visual only)
    EdiChangeHighlightText::EdiChangeHighlightText()
    : BindedFuncCreator("edi_change_highlight_text")
    {}
    void EdiChangeHighlightText::sprocess() {
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
        Change2EdiInsert::sprocess(false) ;
    }
    void EdiChangeHighlightText::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EdiChangeHighlightText::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //EdiChangeLines
    EdiChangeLines::EdiChangeLines()
    : BindedFuncCreator("edi_change_lines")
    {}
    void EdiChangeLines::sprocess(unsigned int repeat_num) {
        auto res = textanalyze::get_selected_text([] {
            keybrd::pushup(KEYCODE_HOME) ;
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        }) ;
        if(res.str.empty()) {
            Change2EdiInsert::sprocess(false) ;
            return ;
        }

        auto pos = res.str.find_first_not_of(" \t") ; //position except for space or tab
        if(pos == std::string::npos) { //space only
            Change2EdiEOLInsert::sprocess(false) ;
            return ;
        }
        keybrd::pushup(KEYCODE_HOME) ;

        repeater::safe_for(pos, [] {
            keybrd::pushup(KEYCODE_RIGHT) ;
        }) ;
        EdiChangeUntilEOL::sprocess(repeat_num, &res) ;
    }
    void EdiChangeLines::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiChangeLines::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //EdiChangeChars
    EdiChangeChars::EdiChangeChars()
    : BindedFuncCreator("edi_change_chars")
    {}
    void EdiChangeChars::sprocess(unsigned int repeat_num) {
        repeater::safe_for(repeat_num, [] {
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
        }) ;

        if(gparams::get_b("enable_char_cache")) {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            simpletxreg::set_register_type(simpletxreg::RegType::Chars) ;
        }
        else {
            keybrd::pushup(KEYCODE_DELETE) ;
        }

        Change2EdiInsert::sprocess(false) ;
    }
    void EdiChangeChars::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiChangeChars::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //EdiChangeUntilEOL
    EdiChangeUntilEOL::EdiChangeUntilEOL()
    : BindedFuncCreator("edi_change_until_eol")
    {}
    /* Actually, If N >= 2
     *
     * Command: 2C
     * 
     * If the caret is positioned the head of a line.
     *
     * Original Vim:
     * [Before]
     *      |   AAAAAA
     *          BBBBBB
     *          CCCCCC
     * [After]
     *      |
     *          CCCCC
     *
     * win-vind:
     * [Before]
     *      |   AAAAAA
     *          BBBBBB
     *          CCCCCC
     * [After]
     *         |CCCCCC
     *
     * In future, must fix.
     *
     */
    void EdiChangeUntilEOL::sprocess(
            unsigned int repeat_num,
            const textanalyze::SelRes* const exres) {

        repeater::safe_for(repeat_num - 1, [] {
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
        }) ;

        if(!txutil::select_line_until_EOL(exres)) {
            txutil::clear_clipboard_with_null() ;
        }
        else {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            simpletxreg::set_register_type(simpletxreg::RegType::Chars) ;
        }
        Change2EdiInsert::sprocess(false) ;
    }
    void EdiChangeUntilEOL::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiChangeUntilEOL::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }
}
