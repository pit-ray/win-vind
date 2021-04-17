#include "bind/emu/edi_jump_caret.hpp"

#include "bind/base/mode.hpp"
#include "io/keybrd.hpp"
#include "text/simple_text_selecter.hpp"
#include "util/def.hpp"

#include "bind/base/char_logger.hpp"
#include "bind/base/ntype_logger.hpp"


namespace vind
{
    //EdiJumpCaret2BOL
    EdiJumpCaret2BOL::EdiJumpCaret2BOL()
    : BindedFuncCreator("edi_jump_caret_to_BOL")
    {}
    bool EdiJumpCaret2BOL::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiJumpCaret2BOL::sprocess() {
        if(mode::is_edi_visual())
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;
        else
            keybrd::pushup(KEYCODE_HOME) ;
    }
    void EdiJumpCaret2BOL::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EdiJumpCaret2BOL::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //EdiJumpCaret2EOL
    EdiJumpCaret2EOL::EdiJumpCaret2EOL()
    : BindedFuncCreator("edi_jump_caret_to_EOL")
    {}
    bool EdiJumpCaret2EOL::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiJumpCaret2EOL::sprocess(unsigned int repeat_num) {
        //down caret N - 1
        for(decltype(repeat_num) i = 1 ; i < repeat_num ; i ++)
            keybrd::pushup(KEYCODE_DOWN) ;

        if(mode::is_edi_visual()) {
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
        }
        else {
            keybrd::pushup(KEYCODE_END) ;
            keybrd::pushup(KEYCODE_LEFT) ;
        }
    }
    void EdiJumpCaret2EOL::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(1) ;
        }
    }
    void EdiJumpCaret2EOL::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //EdiJumpCaret2NLine_DfBOF
    EdiNJumpCaret2Line_DfBOF::EdiNJumpCaret2Line_DfBOF()
    : BindedFuncCreator("edi_n_jump_caret_to_line_default_BOF")
    {}
    bool EdiNJumpCaret2Line_DfBOF::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiNJumpCaret2Line_DfBOF::sprocess(unsigned int repeat_num) {
        if(textselect::is_first_line_selection())
            textselect::select_line_EOL2BOL() ;

        using keybrd::pushup ;

        if(mode::is_edi_visual()) {
            pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_HOME) ;

            //down caret N - 1
            for(decltype(repeat_num) i = 1 ; i < repeat_num ; i ++)
                pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
        }
        else {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_HOME) ;

            //down caret N - 1
            for(decltype(repeat_num) i = 1 ; i < repeat_num ; i ++)
                pushup(KEYCODE_DOWN) ;
        }
    }
    void EdiNJumpCaret2Line_DfBOF::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(1) ;
        }
    }
    void EdiNJumpCaret2Line_DfBOF::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) return ;
        if(auto num = keyloggerutil::extract_num(str)) {
            sprocess(num) ;
        }
        else {
            throw RUNTIME_EXCEPT("There is no numeric character in the passed command.") ;
        }
    }


    //EdiJumpCaret2NLine_DfEOF
    EdiNJumpCaret2Line_DfEOF::EdiNJumpCaret2Line_DfEOF()
    : BindedFuncCreator("edi_n_jump_caret_to_line_default_EOF")
    {}
    bool EdiNJumpCaret2Line_DfEOF::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiNJumpCaret2Line_DfEOF::sprocess(unsigned int repeat_num) {
        using keybrd::pushup ;
        using namespace mode ;

        if(repeat_num == 1) {
            if(is_edi_visual()) {
                if(textselect::is_first_line_selection())
                    textselect::select_line_BOL2EOL() ;

                pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_END) ;

                if(get_global_mode() != Mode::EdiLineVisual)
                    pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;
            }
            else {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_END) ;
            }
        }
        else {
            EdiNJumpCaret2Line_DfBOF::sprocess(repeat_num) ;
        }
    }
    void EdiNJumpCaret2Line_DfEOF::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiNJumpCaret2Line_DfEOF::sprocess(const CharLogger& parent_lgr) {
        EdiNJumpCaret2Line_DfBOF::sprocess(parent_lgr) ;
    }
}
