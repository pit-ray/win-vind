#include "edi_jump_caret.hpp"

#include "io/keybrd.hpp"
#include "mode.hpp"
#include "text/simple_text_selecter.hpp"
#include "utility.hpp"


namespace vind
{
    //EdiJumpCaret2BOL
    const std::string EdiJumpCaret2BOL::sname() noexcept {
        return "edi_jump_caret_to_BOL" ;
    }
    bool EdiJumpCaret2BOL::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiJumpCaret2BOL::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        if(mode::is_edi_visual())
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;
        else
            keybrd::pushup(KEYCODE_HOME) ;
    }


    //EdiJumpCaret2EOL
    const std::string EdiJumpCaret2EOL::sname() noexcept {
        return "edi_jump_caret_to_EOL" ;
    }
    bool EdiJumpCaret2EOL::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiJumpCaret2EOL::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        //down caret N - 1
        for(unsigned int i = 1 ; i < repeat_num ; i ++)
            keybrd::pushup(KEYCODE_DOWN) ;

        if(mode::is_edi_visual()) {
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
        }
        else {
            keybrd::pushup(KEYCODE_END) ;
            keybrd::pushup(KEYCODE_LEFT) ;
        }
    }


    //EdiJumpCaret2NLine_DfBOF
    const std::string EdiNJumpCaret2Line_DfBOF::sname() noexcept {
        return "edi_n_jump_caret_to_line_default_BOF" ;
    }
    bool EdiNJumpCaret2Line_DfBOF::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiNJumpCaret2Line_DfBOF::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const parent_charlgr) {
        if(!first_call) return ;

        if(textselect::is_first_line_selection())
            textselect::select_line_EOL2BOL() ;

        using keybrd::pushup ;

        auto rn = repeat_num ;
        if(parent_charlgr != nullptr) {
            auto str = parent_charlgr->to_str() ;
            if(str.empty()) return ;
            rn = keyloggerutil::extract_num(str) ;
        }

        if(mode::is_edi_visual()) {
            pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_HOME) ;

            //down caret N - 1
            for(unsigned int i = 1 ; i < rn ; i ++)
                pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
        }
        else {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_HOME) ;

            //down caret N - 1
            for(unsigned int i = 1 ; i < rn ; i ++)
                pushup(KEYCODE_DOWN) ;
        }
    }


    //EdiJumpCaret2NLine_DfEOF
    const std::string EdiNJumpCaret2Line_DfEOF::sname() noexcept {
        return "edi_n_jump_caret_to_line_default_EOF" ;
    }
    bool EdiNJumpCaret2Line_DfEOF::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiNJumpCaret2Line_DfEOF::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const parent_charlgr) {
        if(!first_call) return ;

        using keybrd::pushup ;
        using namespace mode ;

        if(parent_charlgr != nullptr) {
            EdiNJumpCaret2Line_DfBOF::sprocess(true, 1, nullptr, parent_charlgr) ;
            return ;
        }

        if(repeat_num == 1) {
            if(is_edi_visual()) {
                if(textselect::is_first_line_selection())
                    textselect::select_line_BOL2EOL() ;

                pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_END) ;

                if(get_mode() != Mode::EdiLineVisual)
                    pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;
            }
            else {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_END) ;
            }
        }
        else {
            EdiNJumpCaret2Line_DfBOF::sprocess(true, repeat_num, nullptr, parent_charlgr) ;
        }
    }
}
