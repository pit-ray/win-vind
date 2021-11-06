#include "edi_jump_caret.hpp"

#include "bind/safe_repeater.hpp"
#include "core/char_logger.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "simple_text_selecter.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"


namespace vind
{
    //JumpCaretToBOL
    JumpCaretToBOL::JumpCaretToBOL()
    : BindedFuncCreator("jump_caret_to_BOL")
    {}
    bool JumpCaretToBOL::is_for_moving_caret() const noexcept {
        return true ;
    }
    void JumpCaretToBOL::sprocess() {
        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;
        }
        else {
            keybrd::pushup(KEYCODE_HOME) ;
        }
    }
    void JumpCaretToBOL::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void JumpCaretToBOL::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //JumpCaretToEOL
    JumpCaretToEOL::JumpCaretToEOL()
    : BindedFuncCreator("jump_caret_to_EOL")
    {}
    bool JumpCaretToEOL::is_for_moving_caret() const noexcept {
        return true ;
    }
    void JumpCaretToEOL::sprocess(unsigned int repeat_num) {
        //down caret N - 1
        repeater::safe_for(repeat_num - 1, [] {
            keybrd::pushup(KEYCODE_DOWN) ;
        }) ; 

        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
        }
        else {
            keybrd::pushup(KEYCODE_END) ;
            keybrd::pushup(KEYCODE_LEFT) ;
        }
    }
    void JumpCaretToEOL::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(1) ;
        }
    }
    void JumpCaretToEOL::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //EdiJumpCaret2NLine_DfBOF
    JumpCaretToBOF::JumpCaretToBOF()
    : BindedFuncCreator("jump_caret_to_BOF")
    {}
    bool JumpCaretToBOF::is_for_moving_caret() const noexcept {
        return true ;
    }
    void JumpCaretToBOF::sprocess(unsigned int repeat_num) {
        if(textselect::is_first_line_selection())
            textselect::select_line_EOL2BOL() ;

        using keybrd::pushup ;

        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_HOME) ;

            //down caret N - 1
            repeater::safe_for(repeat_num - 1, [] {
                pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
            }) ;
        }
        else {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_HOME) ;

            //down caret N - 1
            repeater::safe_for(repeat_num - 1, [] {
                pushup(KEYCODE_DOWN) ;
            }) ;
        }
    }
    void JumpCaretToBOF::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(1) ;
        }
    }
    void JumpCaretToBOF::sprocess(const CharLogger& parent_lgr) {
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
    JumpCaretToEOF::JumpCaretToEOF()
    : BindedFuncCreator("jump_caret_to_EOF")
    {}
    bool JumpCaretToEOF::is_for_moving_caret() const noexcept {
        return true ;
    }
    void JumpCaretToEOF::sprocess(unsigned int repeat_num) {
        using keybrd::pushup ;

        if(repeat_num == 1) {
            if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
                if(textselect::is_first_line_selection())
                    textselect::select_line_BOL2EOL() ;

                pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_END) ;

                if(!(mode::get_global_flags() & mode::ModeFlags::VISUAL_LINE)) {
                    pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;
                }
            }
            else {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_END) ;
            }
        }
        else {
            JumpCaretToBOF::sprocess(repeat_num) ;
        }
    }
    void JumpCaretToEOF::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void JumpCaretToEOF::sprocess(const CharLogger& parent_lgr) {
        JumpCaretToBOF::sprocess(parent_lgr) ;
    }
}
