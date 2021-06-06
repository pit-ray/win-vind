#include "bind/emu/edi_move_caret.hpp"

#include "bind/emu/simple_text_selecter.hpp"
#include "bind/safe_repeater.hpp"
#include "err_logger.hpp"
#include "io/keybrd.hpp"
#include "key/char_logger.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "key/ntype_logger.hpp"
#include "mode.hpp"
#include "time/interval_timer.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/def.hpp"


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

    bool MoveCaretLeft::is_for_moving_caret() const noexcept {
        return true ;
    }
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
        else if(pimpl->ksr.is_pressed()) {
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

    bool MoveCaretRight::is_for_moving_caret() const noexcept {
        return true ;
    }
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
        else if(pimpl->ksr.is_pressed()) {
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

    bool MoveCaretUp::is_for_moving_caret() const noexcept {
        return true ;
    }
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
        else if(pimpl->ksr.is_pressed()) {
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

    bool MoveCaretDown::is_for_moving_caret() const noexcept {
        return true ;
    }
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
        else if(pimpl->ksr.is_pressed()) {
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

    bool MoveCaretWordForward::is_for_moving_caret() const noexcept {
        return true ;
    }
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
        else if(pimpl->ksr.is_pressed()) {
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

    bool MoveCaretWordBackward::is_for_moving_caret() const noexcept {
        return true ;
    }
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
        else if(pimpl->ksr.is_pressed()) {
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

    bool MoveCaretNonBlankWordForward::is_for_moving_caret() const noexcept {
        return true ;
    }

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
        else if(pimpl->ksr.is_pressed()) {
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

    bool MoveCaretNonBlankWordBackward::is_for_moving_caret() const noexcept {
        return true ;
    }
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
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void MoveCaretNonBlankWordBackward::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }
}
