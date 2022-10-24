#include "movecaret.hpp"

#include "bind/saferepeat.hpp"
#include "core/charlogger.hpp"
#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/keycodedef.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "motionids.hpp"
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
        MoveCaretLeft::MoveCaretLeft()
        : MoveBaseCreator("move_caret_left")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveCaretLeft::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
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

        //MoveCaretRight
        MoveCaretRight::MoveCaretRight()
        : MoveBaseCreator("move_caret_right")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveCaretRight::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
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

        //MoveCaretUp
        MoveCaretUp::MoveCaretUp()
        : MoveBaseCreator("move_caret_up")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveCaretUp::sprocess(
                std::uint16_t count,
                const std::string& args) {
            if(!args.empty()) {
                if(auto num = util::extract_num(args)) {
                    count = static_cast<std::uint16_t>(num) ;
                }
            }

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

        //MoveCaretDown
        MoveCaretDown::MoveCaretDown()
        : MoveBaseCreator("move_caret_down")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveCaretDown::sprocess(
                std::uint16_t count,
                const std::string& args) {
            if(!args.empty()) {
                if(auto num = util::extract_num(args)) {
                    count = static_cast<std::uint16_t>(num) ;
                }
            }

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

        //EdiMoveCaretNwordsForward
        MoveCaretWordForward::MoveCaretWordForward()
        : MoveBaseCreator("move_caret_word_forward")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveCaretWordForward::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
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

        //EdiMoveCaretNwordsBackward
        MoveCaretWordBackward::MoveCaretWordBackward()
        : MoveBaseCreator("move_caret_word_backward")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveCaretWordBackward::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
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

        //EdiMoveCaretNWORDSForward
        MoveCaretNonBlankWordForward::MoveCaretNonBlankWordForward()
        : MoveBaseCreator("move_caret_nonblank_word_forward")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveCaretNonBlankWordForward::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
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

        //EdiMoveCaretNWORDSBackward
        MoveCaretNonBlankWordBackward::MoveCaretNonBlankWordBackward()
        : MoveBaseCreator("move_caret_nonblank_word_backward")
        {
            MotionIds::get_instance().register_id(id()) ;
        }
        void MoveCaretNonBlankWordBackward::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
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
    }
}
