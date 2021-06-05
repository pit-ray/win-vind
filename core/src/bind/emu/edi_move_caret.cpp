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
    //EdiMoveCaretLeft
    struct EdiMoveCaretLeft::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiMoveCaretLeft::EdiMoveCaretLeft()
    : BindedFuncCreator("edi_move_caret_left"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiMoveCaretLeft::~EdiMoveCaretLeft() noexcept                    = default ;
    EdiMoveCaretLeft::EdiMoveCaretLeft(EdiMoveCaretLeft&&)            = default ;
    EdiMoveCaretLeft& EdiMoveCaretLeft::operator=(EdiMoveCaretLeft&&) = default ;

    bool EdiMoveCaretLeft::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiMoveCaretLeft::sprocess(unsigned int repeat_num) const {
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
    void EdiMoveCaretLeft::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiMoveCaretLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiMoveCaretRight
    struct EdiMoveCaretRight::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiMoveCaretRight::EdiMoveCaretRight()
    : BindedFuncCreator("edi_move_caret_right"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiMoveCaretRight::~EdiMoveCaretRight() noexcept                     = default ;
    EdiMoveCaretRight::EdiMoveCaretRight(EdiMoveCaretRight&&)            = default ;
    EdiMoveCaretRight& EdiMoveCaretRight::operator=(EdiMoveCaretRight&&) = default ;

    bool EdiMoveCaretRight::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiMoveCaretRight::sprocess(unsigned int repeat_num) const {
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
    void EdiMoveCaretRight::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiMoveCaretRight::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiMoveCaretUp
    struct EdiMoveCaretUp::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiMoveCaretUp::EdiMoveCaretUp()
    : BindedFuncCreator("edi_move_caret_up"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiMoveCaretUp::~EdiMoveCaretUp() noexcept                  = default ;
    EdiMoveCaretUp::EdiMoveCaretUp(EdiMoveCaretUp&&)            = default ;
    EdiMoveCaretUp& EdiMoveCaretUp::operator=(EdiMoveCaretUp&&) = default ;

    bool EdiMoveCaretUp::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiMoveCaretUp::sprocess(unsigned int repeat_num) const {
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
    void EdiMoveCaretUp::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiMoveCaretUp::sprocess(const CharLogger& parent_lgr) const {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) return ;

        if(auto num = keyloggerutil::extract_num(str)) {
            sprocess(num) ;
        }
        else {
            throw RUNTIME_EXCEPT("There is no number in the passed command.") ;
        }
    }


    //EdiMoveCaretDown
    struct EdiMoveCaretDown::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiMoveCaretDown::EdiMoveCaretDown()
    : BindedFuncCreator("edi_move_caret_down"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiMoveCaretDown::~EdiMoveCaretDown() noexcept                    = default ;
    EdiMoveCaretDown::EdiMoveCaretDown(EdiMoveCaretDown&&)            = default ;
    EdiMoveCaretDown& EdiMoveCaretDown::operator=(EdiMoveCaretDown&&) = default ;

    bool EdiMoveCaretDown::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiMoveCaretDown::sprocess(unsigned int repeat_num) const {
        if(mode::get_global_mode() == mode::Mode::EDI_VISUAL) {
            if(textselect::is_first_line_selection()) {
                textselect::select_line_BOL2EOL() ;
            }
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;

                //If call EdiMoveCaretDown after EdiMoveCaretUp,
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
    void EdiMoveCaretDown::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiMoveCaretDown::sprocess(const CharLogger& parent_lgr) const {
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
    struct EdiNMoveCaretwordsForward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNMoveCaretwordsForward::EdiNMoveCaretwordsForward()
    : BindedFuncCreator("edi_n_move_caret_words_forward"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNMoveCaretwordsForward::~EdiNMoveCaretwordsForward() noexcept                             = default ;
    EdiNMoveCaretwordsForward::EdiNMoveCaretwordsForward(EdiNMoveCaretwordsForward&&)            = default ;
    EdiNMoveCaretwordsForward& EdiNMoveCaretwordsForward::operator=(EdiNMoveCaretwordsForward&&) = default ;

    bool EdiNMoveCaretwordsForward::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiNMoveCaretwordsForward::sprocess(unsigned int repeat_num) const {
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
    void EdiNMoveCaretwordsForward::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNMoveCaretwordsForward::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiMoveCaretNwordsBackward
    struct EdiNMoveCaretwordsBackward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNMoveCaretwordsBackward::EdiNMoveCaretwordsBackward()
    : BindedFuncCreator("edi_n_move_caret_words_backward"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNMoveCaretwordsBackward::~EdiNMoveCaretwordsBackward() noexcept                              = default ;
    EdiNMoveCaretwordsBackward::EdiNMoveCaretwordsBackward(EdiNMoveCaretwordsBackward&&)            = default ;
    EdiNMoveCaretwordsBackward& EdiNMoveCaretwordsBackward::operator=(EdiNMoveCaretwordsBackward&&) = default ;

    bool EdiNMoveCaretwordsBackward::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiNMoveCaretwordsBackward::sprocess(unsigned int repeat_num) const {
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
    void EdiNMoveCaretwordsBackward::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNMoveCaretwordsBackward::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiMoveCaretNWORDSForward
    struct EdiNMoveCaretWORDSForward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNMoveCaretWORDSForward::EdiNMoveCaretWORDSForward()
    : BindedFuncCreator("edi_n_move_caret_WORDS_forward"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNMoveCaretWORDSForward::~EdiNMoveCaretWORDSForward() noexcept                             = default ;
    EdiNMoveCaretWORDSForward::EdiNMoveCaretWORDSForward(EdiNMoveCaretWORDSForward&&)            = default ;
    EdiNMoveCaretWORDSForward& EdiNMoveCaretWORDSForward::operator=(EdiNMoveCaretWORDSForward&&) = default ;

    bool EdiNMoveCaretWORDSForward::is_for_moving_caret() const noexcept {
        return true ;
    }

    void EdiNMoveCaretWORDSForward::sprocess(unsigned int repeat_num) const {
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
    void EdiNMoveCaretWORDSForward::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNMoveCaretWORDSForward::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiMoveCaretNWORDSBackward
    struct EdiNMoveCaretWORDSBackward::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNMoveCaretWORDSBackward::EdiNMoveCaretWORDSBackward()
    : BindedFuncCreator("edi_n_move_caret_WORDS_backward"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNMoveCaretWORDSBackward::~EdiNMoveCaretWORDSBackward() noexcept                              = default ;
    EdiNMoveCaretWORDSBackward::EdiNMoveCaretWORDSBackward(EdiNMoveCaretWORDSBackward&&)            = default ;
    EdiNMoveCaretWORDSBackward& EdiNMoveCaretWORDSBackward::operator=(EdiNMoveCaretWORDSBackward&&) = default ;

    bool EdiNMoveCaretWORDSBackward::is_for_moving_caret() const noexcept {
        return true ;
    }
    void EdiNMoveCaretWORDSBackward::sprocess(unsigned int repeat_num) const {
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
    void EdiNMoveCaretWORDSBackward::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNMoveCaretWORDSBackward::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }
}
