#include "edi_move_caret.hpp"

#include "bind/safe_repeater.hpp"
#include "core/char_logger.hpp"
#include "core/err_logger.hpp"
#include "core/key_absorber.hpp"
#include "core/keycode_def.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "simple_text_selecter.hpp"
#include "util/def.hpp"
#include "util/interval_timer.hpp"
#include "util/keybrd.hpp"
#include "util/keystroke_repeater.hpp"


namespace vind
{
    //MoveCaretLeft
    struct MoveCaretLeft::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    MoveCaretLeft::MoveCaretLeft()
    : BindedFuncCreator("move_caret_left"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCaretLeft::~MoveCaretLeft() noexcept                    = default ;
    MoveCaretLeft::MoveCaretLeft(MoveCaretLeft&&)            = default ;
    MoveCaretLeft& MoveCaretLeft::operator=(MoveCaretLeft&&) = default ;

    void MoveCaretLeft::sprocess(unsigned int repeat_num) const {
        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
            }) ;
        }
        else {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LEFT) ;
            }) ;
        }
    }
    void MoveCaretLeft::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void MoveCaretLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //MoveCaretRight
    struct MoveCaretRight::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    MoveCaretRight::MoveCaretRight()
    : BindedFuncCreator("move_caret_right"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCaretRight::~MoveCaretRight() noexcept                     = default ;
    MoveCaretRight::MoveCaretRight(MoveCaretRight&&)            = default ;
    MoveCaretRight& MoveCaretRight::operator=(MoveCaretRight&&) = default ;

    void MoveCaretRight::sprocess(unsigned int repeat_num) const {
        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
            }) ;
        }
        else {
            repeater::safe_for(repeat_num, []{
                keybrd::pushup(KEYCODE_RIGHT) ;
            }) ;
        }
    }
    void MoveCaretRight::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void MoveCaretRight::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //MoveCaretUp
    struct MoveCaretUp::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    MoveCaretUp::MoveCaretUp()
    : BindedFuncCreator("move_caret_up"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCaretUp::~MoveCaretUp() noexcept                  = default ;
    MoveCaretUp::MoveCaretUp(MoveCaretUp&&)            = default ;
    MoveCaretUp& MoveCaretUp::operator=(MoveCaretUp&&) = default ;

    void MoveCaretUp::sprocess(unsigned int repeat_num) const {
        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            if(textselect::is_first_line_selection()) {
                textselect::select_line_EOL2BOL() ;
            }
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_UP) ;
                //textselect::moving_update() ;
            }) ;
        }
        else {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_UP) ;
            }) ;
        }
    }
    void MoveCaretUp::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void MoveCaretUp::sprocess(const CharLogger& parent_lgr) const {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) return ;

        if(auto num = keyloggerutil::extract_num(str)) {
            sprocess(num) ;
        }
        else {
            throw RUNTIME_EXCEPT("There is no number in the passed command.") ;
        }
    }


    //MoveCaretDown
    struct MoveCaretDown::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    MoveCaretDown::MoveCaretDown()
    : BindedFuncCreator("move_caret_down"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCaretDown::~MoveCaretDown() noexcept                    = default ;
    MoveCaretDown::MoveCaretDown(MoveCaretDown&&)            = default ;
    MoveCaretDown& MoveCaretDown::operator=(MoveCaretDown&&) = default ;

    void MoveCaretDown::sprocess(unsigned int repeat_num) const {
        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            if(textselect::is_first_line_selection()) {
                textselect::select_line_BOL2EOL() ;
            }
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;

                //If call MoveCaretDown after MoveCaretUp,
                //inner variables of moving_update() are dedicated to EOL2BOL.
                //so we cannot move caret down.
                //textselect::moving_update() ;
            }) ;
        }
        else {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_DOWN) ;
            }) ;
        }
    }
    void MoveCaretDown::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void MoveCaretDown::sprocess(const CharLogger& parent_lgr) const {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) return ;

        if(auto num = keyloggerutil::extract_num(str)) {
            sprocess(num) ;
        }
        else {
            throw RUNTIME_EXCEPT("There is no number in the passed command.") ;
        }
    }


    //EdiMoveCaretNwordsForward
    struct MoveCaretWordForward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    MoveCaretWordForward::MoveCaretWordForward()
    : BindedFuncCreator("move_caret_word_forward"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCaretWordForward::~MoveCaretWordForward() noexcept                             = default ;
    MoveCaretWordForward::MoveCaretWordForward(MoveCaretWordForward&&)            = default ;
    MoveCaretWordForward& MoveCaretWordForward::operator=(MoveCaretWordForward&&) = default ;

    void MoveCaretWordForward::sprocess(unsigned int repeat_num) const {
        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_RIGHT) ;
            }) ;
        }
        else {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_RIGHT) ;
            }) ;
        }
    }
    void MoveCaretWordForward::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void MoveCaretWordForward::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiMoveCaretNwordsBackward
    struct MoveCaretWordBackward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    MoveCaretWordBackward::MoveCaretWordBackward()
    : BindedFuncCreator("move_caret_word_backward"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCaretWordBackward::~MoveCaretWordBackward() noexcept                              = default ;
    MoveCaretWordBackward::MoveCaretWordBackward(MoveCaretWordBackward&&)            = default ;
    MoveCaretWordBackward& MoveCaretWordBackward::operator=(MoveCaretWordBackward&&) = default ;

    void MoveCaretWordBackward::sprocess(unsigned int repeat_num) const {
        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_LEFT) ;
            }) ;
        }
        else {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_LEFT) ;
            }) ;
        }
    }
    void MoveCaretWordBackward::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void MoveCaretWordBackward::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiMoveCaretNWORDSForward
    struct MoveCaretNonBlankWordForward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    MoveCaretNonBlankWordForward::MoveCaretNonBlankWordForward()
    : BindedFuncCreator("move_caret_nonblank_word_forward"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCaretNonBlankWordForward::~MoveCaretNonBlankWordForward() noexcept                             = default ;
    MoveCaretNonBlankWordForward::MoveCaretNonBlankWordForward(MoveCaretNonBlankWordForward&&)            = default ;
    MoveCaretNonBlankWordForward& MoveCaretNonBlankWordForward::operator=(MoveCaretNonBlankWordForward&&) = default ;

    void MoveCaretNonBlankWordForward::sprocess(unsigned int repeat_num) const {
        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_RIGHT) ;
            }) ;
        }
        else {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_RIGHT) ;
            }) ;
        }
    }
    void MoveCaretNonBlankWordForward::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void MoveCaretNonBlankWordForward::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiMoveCaretNWORDSBackward
    struct MoveCaretNonBlankWordBackward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    MoveCaretNonBlankWordBackward::MoveCaretNonBlankWordBackward()
    : BindedFuncCreator("move_caret_nonblank_word_backward"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCaretNonBlankWordBackward::~MoveCaretNonBlankWordBackward() noexcept                              = default ;
    MoveCaretNonBlankWordBackward::MoveCaretNonBlankWordBackward(MoveCaretNonBlankWordBackward&&)            = default ;
    MoveCaretNonBlankWordBackward& MoveCaretNonBlankWordBackward::operator=(MoveCaretNonBlankWordBackward&&) = default ;

    void MoveCaretNonBlankWordBackward::sprocess(unsigned int repeat_num) const {
        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_LEFT) ;
            }) ;
        }
        else {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_LEFT) ;
            }) ;
        }
    }
    void MoveCaretNonBlankWordBackward::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
        else if(pimpl->ksr.is_passed()) {
            sprocess(1) ;
        }
    }
    void MoveCaretNonBlankWordBackward::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }
}
