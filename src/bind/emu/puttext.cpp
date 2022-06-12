#include "puttext.hpp"

#include "bind/saferepeat.hpp"
#include "core/inputgate.hpp"
#include "core/keycodedef.hpp"
#include "core/ntypelogger.hpp"
#include "deltext.hpp"
#include "smartclipboard.hpp"
#include "textreg.hpp"
#include "util/def.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/winwrap.hpp"


namespace
{
    using namespace vind ;
    // If the clipboard does not have string, return false.
    bool remove_crlf_in_clipboard() {
        auto hwnd = util::get_foreground_window() ;
        bind::SmartClipboard scb(hwnd) ;
        scb.open() ;

        std::string str ;
        scb.get_as_str(str) ;

        if(!str.empty()) {
            auto pos = str.find_last_not_of("\r\n") ;
            if(pos != std::string::npos) {
                str = str.substr(0, pos + 1) ;
                if(!str.empty()) {
                    scb.set(str) ;
                }
            }
        }

        return !str.empty() ;
    }
}


namespace vind
{
    namespace bind
    {
        //PutAfter (EdiNormal or EdiVisual)
        struct PutAfter::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        PutAfter::PutAfter()
        : ChangeBaseCreator("put_after"),
          pimpl(std::make_unique<Impl>())
        {}

        PutAfter::~PutAfter() noexcept            = default ;
        PutAfter::PutAfter(PutAfter&&)            = default ;
        PutAfter& PutAfter::operator=(PutAfter&&) = default ;

        void PutAfter::sprocess(unsigned int count) {
            if(!remove_crlf_in_clipboard()) {
                return ;
            }

            auto& igate = core::InputGate::get_instance() ;
            auto t = get_register_type() ;
            if(t == RegType::Chars) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_V) ;
                }) ;
            }
            else if(t == RegType::Lines) {
                igate.pushup(KEYCODE_END) ;
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_ENTER) ;
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_V) ;
                }) ;
                igate.pushup(KEYCODE_HOME) ;
            }
        }
        void PutAfter::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void PutAfter::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //PutBefore
        struct PutBefore::Impl {
            util::KeyStrokeRepeater ksr{} ;
        } ;

        PutBefore::PutBefore()
        : ChangeBaseCreator("put_before"),
          pimpl(std::make_unique<Impl>())
        {}

        PutBefore::~PutBefore() noexcept                   = default ;
        PutBefore::PutBefore(PutBefore&&)            = default ;
        PutBefore& PutBefore::operator=(PutBefore&&) = default ;

        void PutBefore::sprocess(unsigned int count) {
            if(!remove_crlf_in_clipboard()) {
                return ;
            }

            auto& igate = core::InputGate::get_instance() ;
            auto t = get_register_type() ;
            if(t == RegType::Chars) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_V) ;
                }) ;
            }
            else if(t == RegType::Lines) {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_HOME) ;
                    igate.pushup(KEYCODE_ENTER) ;
                    igate.pushup(KEYCODE_UP) ;
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_V) ;
                }) ;
                igate.pushup(KEYCODE_HOME) ;
            }
        }
        void PutBefore::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr.reset() ;
            }
            else if(pimpl->ksr.is_passed()) {
                sprocess(1) ;
            }
        }
        void PutBefore::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }
    }
}
