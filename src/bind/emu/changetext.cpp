#include "changetext.hpp"

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
        void ChangeHighlightText::sprocess() {
            core::InputGate::get_instance().pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            if(core::get_global_mode_flags() & core::ModeFlags::VISUAL_LINE) {
                set_register_type(RegType::Lines) ;
            }
            else {
                set_register_type(RegType::Chars) ;
            }
            ToInsert::sprocess(false) ;
        }
        void ChangeHighlightText::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void ChangeHighlightText::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //ChangeLine
        ChangeLine::ChangeLine()
        : DeleteLineUntilEOLBase("change_line")
        {}
        void ChangeLine::sprocess(unsigned int count) {
            auto& igate = core::InputGate::get_instance() ;
            auto res = get_selected_text([&igate] {
                igate.pushup(KEYCODE_HOME) ;
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
                igate.pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            }) ;
            if(res.str.empty()) {
                ToInsert::sprocess(false) ;
                return ;
            }
            igate.pushup(KEYCODE_HOME) ;

            auto pos = res.str.find_first_not_of(" \t") ;
            if(pos != std::string::npos) {
                safe_for(pos, [&igate] {
                    igate.pushup(KEYCODE_RIGHT) ;
                }) ;
            }
            DeleteLineUntilEOLBase<ChangeLine>::sprocess(count) ;
            ToInsert::sprocess(false) ;
        }
        void ChangeLine::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void ChangeLine::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //ChangeChar
        ChangeChar::ChangeChar()
        : ChangeBaseCreator("change_char")
        {}
        void ChangeChar::sprocess(unsigned int count) {
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

            ToInsert::sprocess(false) ;
        }
        void ChangeChar::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void ChangeChar::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //ChangeUntilEOL
        ChangeUntilEOL::ChangeUntilEOL()
        : DeleteLineUntilEOLBase("change_until_EOL")
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
        void ChangeUntilEOL::sprocess(unsigned int count) {
            DeleteLineUntilEOLBase<ChangeUntilEOL>::sprocess(count) ;
            ToInsert::sprocess(false) ;
        }
        void ChangeUntilEOL::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void ChangeUntilEOL::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }
    }
}
