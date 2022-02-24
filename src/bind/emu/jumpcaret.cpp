#include "jumpcaret.hpp"

#include "bind/saferepeat.hpp"
#include "core/charlogger.hpp"
#include "core/inputgate.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "textsel.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/string.hpp"


namespace vind
{
    namespace bind
    {
        //JumpCaretToBOL
        JumpCaretToBOL::JumpCaretToBOL()
        : MoveBaseCreator("jump_caret_to_BOL")
        {}
        void JumpCaretToBOL::sprocess() {
            auto& igate = core::InputGate::get_instance() ;
            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;
            }
            else {
                igate.pushup(KEYCODE_HOME) ;
            }
        }
        void JumpCaretToBOL::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void JumpCaretToBOL::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //JumpCaretToEOL
        JumpCaretToEOL::JumpCaretToEOL()
        : MoveBaseCreator("jump_caret_to_EOL")
        {}
        void JumpCaretToEOL::sprocess(unsigned int count) {
            auto& igate = core::InputGate::get_instance() ;

            //down caret N - 1
            safe_for(count - 1, [&igate] {
                igate.pushup(KEYCODE_DOWN) ;
            }) ; 

            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
            }
            else {
                igate.pushup(KEYCODE_END) ;
                igate.pushup(KEYCODE_LEFT) ;
            }
        }
        void JumpCaretToEOL::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(1) ;
            }
        }
        void JumpCaretToEOL::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //EdiJumpCaret2NLine_DfBOF
        JumpCaretToBOF::JumpCaretToBOF()
        : MoveBaseCreator("jump_caret_to_BOF")
        {}
        void JumpCaretToBOF::sprocess(unsigned int count) {
            if(is_first_line_selection()) {
                select_line_EOL2BOL() ;
            }

            auto& igate = core::InputGate::get_instance() ;

            if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_HOME) ;

                //down caret N - 1
                safe_for(count - 1, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
                }) ;
            }
            else {
                igate.pushup(KEYCODE_LCTRL, KEYCODE_HOME) ;

                //down caret N - 1
                safe_for(count - 1, [&igate] {
                    igate.pushup(KEYCODE_DOWN) ;
                }) ;
            }
        }
        void JumpCaretToBOF::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(1) ;
            }
        }
        void JumpCaretToBOF::sprocess(const core::CharLogger& parent_lgr) {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) return ;
            if(auto num = util::extract_num(str)) {
                sprocess(num) ;
            }
            else {
                throw RUNTIME_EXCEPT("There is no numeric character in the passed command.") ;
            }
        }


        //EdiJumpCaret2NLine_DfEOF
        JumpCaretToEOF::JumpCaretToEOF()
        : MoveBaseCreator("jump_caret_to_EOF")
        {}
        void JumpCaretToEOF::sprocess(unsigned int count) {
            auto& igate = core::InputGate::get_instance() ;

            if(count == 1) {
                if(core::get_global_mode() == core::Mode::EDI_VISUAL) {
                    if(is_first_line_selection()) {
                        select_line_BOL2EOL() ;
                    }

                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_END) ;

                    if(!(core::get_global_mode_flags() & core::ModeFlags::VISUAL_LINE)) {
                        igate.pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;
                    }
                }
                else {
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_END) ;
                }
            }
            else {
                JumpCaretToBOF::sprocess(count) ;
            }
        }
        void JumpCaretToEOF::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void JumpCaretToEOF::sprocess(const core::CharLogger& parent_lgr) {
            JumpCaretToBOF::sprocess(parent_lgr) ;
        }
    }
}
