#include "movecaret.hpp"

#include "bind/saferepeat.hpp"
#include "core/charlogger.hpp"
#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/keycodedef.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "textsel.hpp"
#include "util/def.hpp"
#include "util/interval_timer.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/string.hpp"


namespace vind
{
    namespace bind
    {
        //MoveCaretLeft
        struct MoveCaretLeft::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        MoveCaretLeft::MoveCaretLeft()
        : MoveBaseCreator("move_caret_left"),
          pimpl(std::make_unique<Impl>())
        {}

        MoveCaretLeft::~MoveCaretLeft() noexcept = default ;
        MoveCaretLeft::MoveCaretLeft(MoveCaretLeft&&) = default ;
        MoveCaretLeft& MoveCaretLeft::operator=(MoveCaretLeft&&) = default ;

        void MoveCaretLeft::sprocess(unsigned int count) const {
            auto& igate = core::InputGate::get_instance() ;
            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LEFT) ;
                }) ;
            }
        }
        void MoveCaretLeft::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void MoveCaretLeft::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        //MoveCaretRight
        struct MoveCaretRight::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        MoveCaretRight::MoveCaretRight()
        : MoveBaseCreator("move_caret_right"),
          pimpl(std::make_unique<Impl>())
        {}

        MoveCaretRight::~MoveCaretRight() noexcept = default ;
        MoveCaretRight::MoveCaretRight(MoveCaretRight&&) = default ;
        MoveCaretRight& MoveCaretRight::operator=(MoveCaretRight&&) = default ;

        void MoveCaretRight::sprocess(unsigned int count) const {
            auto& igate = core::InputGate::get_instance() ;
            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                }) ;
            }
            else {
                safe_for(count, [&igate]{
                    igate.pushup(KEYCODE_RIGHT) ;
                }) ;
            }
        }
        void MoveCaretRight::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void MoveCaretRight::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        //MoveCaretUp
        struct MoveCaretUp::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        MoveCaretUp::MoveCaretUp()
        : MoveBaseCreator("move_caret_up"),
          pimpl(std::make_unique<Impl>())
        {}

        MoveCaretUp::~MoveCaretUp() noexcept = default ;
        MoveCaretUp::MoveCaretUp(MoveCaretUp&&) = default ;
        MoveCaretUp& MoveCaretUp::operator=(MoveCaretUp&&) = default ;

        void MoveCaretUp::sprocess(unsigned int count) const {
            auto& igate = core::InputGate::get_instance() ;

            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                if(is_first_line_selection()) {
                    select_line_EOL2BOL() ;
                }
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_UP) ;
                    //moving_update() ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_UP) ;
                }) ;
            }
        }
        void MoveCaretUp::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void MoveCaretUp::sprocess(const core::CharLogger& parent_lgr) const {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) return ;

            if(auto num = util::extract_num(str)) {
                sprocess(num) ;
            }
            else {
                throw RUNTIME_EXCEPT("There is no number in the passed command.") ;
            }
        }


        //MoveCaretDown
        struct MoveCaretDown::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        MoveCaretDown::MoveCaretDown()
        : MoveBaseCreator("move_caret_down"),
          pimpl(std::make_unique<Impl>())
        {}

        MoveCaretDown::~MoveCaretDown() noexcept = default ;
        MoveCaretDown::MoveCaretDown(MoveCaretDown&&) = default ;
        MoveCaretDown& MoveCaretDown::operator=(MoveCaretDown&&) = default ;

        void MoveCaretDown::sprocess(unsigned int count) const {
            auto& igate = core::InputGate::get_instance() ;

            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                if(is_first_line_selection()) {
                    select_line_BOL2EOL() ;
                }
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;

                    //If call MoveCaretDown after MoveCaretUp,
                    //inner variables of moving_update() are dedicated to EOL2BOL.
                    //so we cannot move caret down.
                    //moving_update() ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_DOWN) ;
                }) ;
            }
        }
        void MoveCaretDown::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void MoveCaretDown::sprocess(const core::CharLogger& parent_lgr) const {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) return ;

            if(auto num = util::extract_num(str)) {
                sprocess(num) ;
            }
            else {
                throw RUNTIME_EXCEPT("There is no number in the passed command.") ;
            }
        }


        //EdiMoveCaretNwordsForward
        struct MoveCaretWordForward::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        MoveCaretWordForward::MoveCaretWordForward()
        : MoveBaseCreator("move_caret_word_forward"),
          pimpl(std::make_unique<Impl>())
        {}

        MoveCaretWordForward::~MoveCaretWordForward() noexcept = default ;
        MoveCaretWordForward::MoveCaretWordForward(MoveCaretWordForward&&) = default ;
        MoveCaretWordForward& MoveCaretWordForward::operator=(MoveCaretWordForward&&) = default ;

        void MoveCaretWordForward::sprocess(unsigned int count) const {
            auto& igate = core::InputGate::get_instance() ;

            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_RIGHT) ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_RIGHT) ;
                }) ;
            }
        }
        void MoveCaretWordForward::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void MoveCaretWordForward::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        //EdiMoveCaretNwordsBackward
        struct MoveCaretWordBackward::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        MoveCaretWordBackward::MoveCaretWordBackward()
        : MoveBaseCreator("move_caret_word_backward"),
          pimpl(std::make_unique<Impl>())
        {}

        MoveCaretWordBackward::~MoveCaretWordBackward() noexcept = default ;
        MoveCaretWordBackward::MoveCaretWordBackward(MoveCaretWordBackward&&) = default ;
        MoveCaretWordBackward& MoveCaretWordBackward::operator=(MoveCaretWordBackward&&) = default ;

        void MoveCaretWordBackward::sprocess(unsigned int count) const {
            auto& igate = core::InputGate::get_instance() ;

            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_LEFT) ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_LEFT) ;
                }) ;
            }
        }
        void MoveCaretWordBackward::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void MoveCaretWordBackward::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        //EdiMoveCaretNWORDSForward
        struct MoveCaretNonBlankWordForward::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        MoveCaretNonBlankWordForward::MoveCaretNonBlankWordForward()
        : MoveBaseCreator("move_caret_nonblank_word_forward"),
          pimpl(std::make_unique<Impl>())
        {}

        MoveCaretNonBlankWordForward::~MoveCaretNonBlankWordForward() noexcept = default ;
        MoveCaretNonBlankWordForward::MoveCaretNonBlankWordForward(MoveCaretNonBlankWordForward&&) = default ;
        MoveCaretNonBlankWordForward& MoveCaretNonBlankWordForward::operator=(MoveCaretNonBlankWordForward&&) = default ;

        void MoveCaretNonBlankWordForward::sprocess(unsigned int count) const {
            auto& igate = core::InputGate::get_instance() ;

            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_RIGHT) ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_RIGHT) ;
                }) ;
            }
        }
        void MoveCaretNonBlankWordForward::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void MoveCaretNonBlankWordForward::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        //EdiMoveCaretNWORDSBackward
        struct MoveCaretNonBlankWordBackward::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        MoveCaretNonBlankWordBackward::MoveCaretNonBlankWordBackward()
        : MoveBaseCreator("move_caret_nonblank_word_backward"),
          pimpl(std::make_unique<Impl>())
        {}

        MoveCaretNonBlankWordBackward::~MoveCaretNonBlankWordBackward() noexcept = default ;
        MoveCaretNonBlankWordBackward::MoveCaretNonBlankWordBackward(MoveCaretNonBlankWordBackward&&) = default ;
        MoveCaretNonBlankWordBackward& MoveCaretNonBlankWordBackward::operator=(MoveCaretNonBlankWordBackward&&) = default ;

        void MoveCaretNonBlankWordBackward::sprocess(unsigned int count) const {
            auto& igate = core::InputGate::get_instance() ;

            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_LEFT) ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_LEFT) ;
                }) ;
            }
        }
        void MoveCaretNonBlankWordBackward::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void MoveCaretNonBlankWordBackward::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }
    }
}
