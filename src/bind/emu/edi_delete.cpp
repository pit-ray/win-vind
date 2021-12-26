#include "edi_delete.hpp"

#include "bind/mode/change_mode.hpp"
#include "bind/safe_repeater.hpp"
#include "core/inputgate.hpp"
#include "core/keycodedef.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "core/settable.hpp"
#include "simple_text_register.hpp"
#include "text_util.hpp"
#include "util/def.hpp"
#include "util/keystroke_repeater.hpp"


namespace vind
{
    namespace bind
    {
        //DeleteHighlightText (Visual only)
        DeleteHighlightText::DeleteHighlightText()
        : BindedFuncVoid("delete_highlight_text")
        {}
        void DeleteHighlightText::sprocess() {
            core::InputGate::get_instance().pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            if(core::get_global_mode_flags() & core::ModeFlags::VISUAL_LINE) {
                set_register_type(RegType::Lines) ;
            }
            else {
                set_register_type(RegType::Chars) ;
            }
            ToEdiNormal::sprocess(false) ;
        }
        void DeleteHighlightText::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void DeleteHighlightText::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //DeleteLine
        struct DeleteLine::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        DeleteLine::DeleteLine()
        : ChangeBaseCreator("delete_line"),
          pimpl(std::make_unique<Impl>())
        {}

        DeleteLine::~DeleteLine() noexcept                  = default ;
        DeleteLine::DeleteLine(DeleteLine&&)            = default ;
        DeleteLine& DeleteLine::operator=(DeleteLine&&) = default ;

        void DeleteLine::sprocess(unsigned int repeat_num) const {
            auto& igate = core::InputGate::get_instance() ;

            igate.pushup(KEYCODE_HOME) ;

            safe_for(repeat_num - 1, [&igate] {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
            }) ;

            if(!select_line_until_EOL(nullptr)) {
                clear_clipboard_with_null() ;
            }
            else {
                igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                set_register_type(RegType::Lines) ;
                igate.pushup(KEYCODE_DELETE) ;
            }
        }
        void DeleteLine::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void DeleteLine::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        //DeleteLineUntilEOL
        struct DeleteLineUntilEOL::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        DeleteLineUntilEOL::DeleteLineUntilEOL()
        : ChangeBaseCreator("delete_line_until_EOL"),
          pimpl(std::make_unique<Impl>())
        {}

        DeleteLineUntilEOL::~DeleteLineUntilEOL() noexcept                          = default ;
        DeleteLineUntilEOL::DeleteLineUntilEOL(DeleteLineUntilEOL&&)            = default ;
        DeleteLineUntilEOL& DeleteLineUntilEOL::operator=(DeleteLineUntilEOL&&) = default ;

        void DeleteLineUntilEOL::sprocess(unsigned int repeat_num) const {
            auto& igate = core::InputGate::get_instance() ;

            //delete N - 1 lines under the current line
            safe_for(repeat_num - 1, [&igate] {
                igate.pushup(KEYCODE_DOWN) ;
            }) ;

            if(!select_line_until_EOL(nullptr)) {
                clear_clipboard_with_null() ;
            }
            else {
                igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                set_register_type(RegType::Chars) ;
            }
        }
        void DeleteLineUntilEOL::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void DeleteLineUntilEOL::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        //DeleteAfter
        struct DeleteAfter::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        DeleteAfter::DeleteAfter()
        : ChangeBaseCreator("delete_after"),
          pimpl(std::make_unique<Impl>())
        {}

        DeleteAfter::~DeleteAfter() noexcept                   = default ;
        DeleteAfter::DeleteAfter(DeleteAfter&&)            = default ;
        DeleteAfter& DeleteAfter::operator=(DeleteAfter&&) = default ;

        void DeleteAfter::sprocess(unsigned int repeat_num) const {
            auto& igate = core::InputGate::get_instance() ;
            auto& settable = core::SetTable::get_instance() ;
            if(settable.get("charcache").get<bool>()) {
                safe_for(repeat_num, [&igate] {
                        igate.pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                        igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                        set_register_type(RegType::Chars) ;
                }) ;
            }
            else {
                safe_for(repeat_num, [&igate] {
                    igate.pushup(KEYCODE_DELETE) ;
                }) ;
            }
        }
        void DeleteAfter::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void DeleteAfter::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        //DeleteBefore
        struct DeleteBefore::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        DeleteBefore::DeleteBefore()
        : ChangeBaseCreator("delete_before"),
          pimpl(std::make_unique<Impl>())
        {}

        DeleteBefore::~DeleteBefore() noexcept                    = default ;
        DeleteBefore::DeleteBefore(DeleteBefore&&)            = default ;
        DeleteBefore& DeleteBefore::operator=(DeleteBefore&&) = default ;

        void DeleteBefore::sprocess(unsigned int repeat_num) const {
            auto& igate = core::InputGate::get_instance() ;
            auto& settable = core::SetTable::get_instance() ;
            if(settable.get("charcache").get<bool>()) {
                safe_for(repeat_num, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                    set_register_type(RegType::Chars) ;
                }) ;
            }
            else {
                safe_for(repeat_num, [&igate] {
                    igate.pushup(KEYCODE_BKSPACE) ;
                }) ;
            }
        }
        void DeleteBefore::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void DeleteBefore::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }
    }
}
