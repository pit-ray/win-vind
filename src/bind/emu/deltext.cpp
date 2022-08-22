#include "deltext.hpp"

#include "bind/mode/change_mode.hpp"
#include "bind/saferepeat.hpp"
#include "core/inputgate.hpp"
#include "core/keycodedef.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "core/settable.hpp"
#include "smartclipboard.hpp"
#include "textreg.hpp"
#include "textutil.hpp"
#include "util/def.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind
    {
        //DeleteHighlightText (Visual only)
        DeleteHighlightText::DeleteHighlightText()
        : BindedFuncVoid("delete_highlight_text")
        {}
        void DeleteHighlightText::sprocess(
                std::uint16_t count, const std::string& args) {
            core::InputGate::get_instance().pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            if(core::get_global_mode_flags() & core::ModeFlags::VISUAL_LINE) {
                set_register_type(RegType::Lines) ;
            }
            else {
                set_register_type(RegType::Chars) ;
            }
            ToEdiNormal::sprocess(1, "", false) ;
        }


        //DeleteLine
        DeleteLine::DeleteLine()
        : ChangeBaseCreator("delete_line")
        {}

        void DeleteLine::sprocess(
                std::uint16_t count, const std::string& args) {
            auto& igate = core::InputGate::get_instance() ;

            igate.pushup(KEYCODE_HOME) ;
            safe_for(count, [&igate] {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
            }) ;

            Sleep(24) ;

            igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            set_register_type(RegType::Lines) ;
        }


        // DeleteLineUntilEOL
        DeleteLineUntilEOL::DeleteLineUntilEOL()
        : ChangeBaseCreator("delete_line_until_EOL")
        {}

        void DeleteLineUntilEOL::sprocess(
                std::uint16_t count, const std::string& args) {
            auto& igate = core::InputGate::get_instance() ;

            //delete N - 1 lines under the current line
            safe_for(count - 1, [&igate] {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
            }) ;

            auto res = get_selected_text([&igate] {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
                igate.pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            }) ;

            // Some editors have a visible EOL mark in a line.
            if(res.having_EOL) {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                if(res.str.empty()) {
                    // clear clipboard with null
                    auto hwnd = util::get_foreground_window() ;
                    SmartClipboard scb(hwnd) ;
                    scb.open() ;
                    scb.set("") ;
                    scb.close() ;
                }
            }

            igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            set_register_type(RegType::Chars) ;
        }

        //DeleteAfter
        DeleteAfter::DeleteAfter()
        : ChangeBaseCreator("delete_after")
        {}

        void DeleteAfter::sprocess(
                std::uint16_t count, const std::string& args) {
            auto& igate = core::InputGate::get_instance() ;
            auto& settable = core::SetTable::get_instance() ;
            if(settable.get("charcache").get<bool>()) {
                safe_for(count, [&igate] {
                        igate.pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                        igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                        set_register_type(RegType::Chars) ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_DELETE) ;
                }) ;
            }
        }

        //DeleteBefore
        DeleteBefore::DeleteBefore()
        : ChangeBaseCreator("delete_before")
        {}

        void DeleteBefore::sprocess(
                std::uint16_t count, const std::string& args) {
            auto& igate = core::InputGate::get_instance() ;
            auto& settable = core::SetTable::get_instance() ;
            if(settable.get("charcache").get<bool>()) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                    set_register_type(RegType::Chars) ;
                }) ;
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_BKSPACE) ;
                }) ;
            }
        }
    }
}
