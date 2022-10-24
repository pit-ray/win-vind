#include "changetext.hpp"

#include "bind/emu/deltext.hpp"
#include "bind/mode/change_mode.hpp"
#include "bind/saferepeat.hpp"
#include "core/inputgate.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "core/settable.hpp"
#include "moveinsert.hpp"
#include "textreg.hpp"
#include "textutil.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        //ChangeHighlightText (Visual only)
        ChangeHighlightText::ChangeHighlightText()
        : BindedFuncVoid("change_highlight_text")
        {}
        void ChangeHighlightText::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            core::InputGate::get_instance().pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            if(core::get_global_mode_flags() & core::ModeFlags::VISUAL_LINE) {
                set_register_type(RegType::Lines) ;
            }
            else {
                set_register_type(RegType::Chars) ;
            }
            ToInsert::sprocess(1, "", false) ;
        }

        //ChangeLine
        ChangeLine::ChangeLine()
        : ChangeBaseCreator("change_line")
        {}
        void ChangeLine::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& igate = core::InputGate::get_instance() ;
            auto res = get_selected_text([&igate] {
                igate.pushup(KEYCODE_HOME) ;
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
                igate.pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            }) ;
            if(res.str.empty()) {
                ToInsert::sprocess(1, "", false) ;
                return ;
            }
            igate.pushup(KEYCODE_HOME) ;

            auto pos = res.str.find_first_not_of(" \t") ;
            if(pos != std::string::npos) {
                safe_for(pos, [&igate] {
                    igate.pushup(KEYCODE_RIGHT) ;
                }) ;
            }
            DeleteLineUntilEOL::sprocess(count, "") ;
            ToInsert::sprocess(1, "", false) ;
        }

        //ChangeChar
        ChangeChar::ChangeChar()
        : ChangeBaseCreator("change_char")
        {}
        void ChangeChar::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& igate = core::InputGate::get_instance() ;

            safe_for(count, [&igate] {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
            }) ;

            auto& settable = core::SetTable::get_instance() ;
            if(settable.get("charcache").get<bool>()) {
                igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                set_register_type(RegType::Chars) ;
            }
            else {
                igate.pushup(KEYCODE_DELETE) ;
            }

            ToInsert::sprocess(1, "", false) ;
        }

        //ChangeUntilEOL
        ChangeUntilEOL::ChangeUntilEOL()
        : ChangeBaseCreator("change_until_EOL")
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
        void ChangeUntilEOL::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            DeleteLineUntilEOL::sprocess(count, "") ;
            ToInsert::sprocess(1, "", false) ;
        }
    }
}
