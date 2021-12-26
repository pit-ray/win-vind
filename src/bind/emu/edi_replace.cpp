#include "edi_replace.hpp"

#include <windows.h>

#include "bind/safe_repeater.hpp"
#include "core/background.hpp"
#include "core/charlogger.hpp"
#include "core/entry.hpp"
#include "core/inputgate.hpp"
#include "core/keycode.hpp"
#include "core/keycodedef.hpp"
#include "core/ntypelogger.hpp"
#include "opt/async_uia_cache_builder.hpp"
#include "opt/blockstyle_caret.hpp"
#include "opt/optionlist.hpp"
#include "opt/vcmdline.hpp"
#include "text_analyzer.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"

#include <vector>

namespace
{
    using namespace vind ;

    inline bool is_shift(unsigned char key) noexcept {
        return key == KEYCODE_SHIFT || key == KEYCODE_LSHIFT || key == KEYCODE_RSHIFT ;
    }

    class ReplaceMatching {
    private:
        core::Background bg_ ;

        /*
         * @return: [true] continue loop, [false] break loop
         */
        virtual bool do_loop_hook(
                core::KeyCode UNUSED(keycode),
                bool UNUSED(shifted)) {
            return false ;
        }

    public:
        explicit ReplaceMatching()
        : bg_(opt::ref_global_options_bynames(
                opt::AsyncUIACacheBuilder().name(),
                opt::BlockStyleCaret().name(),
                opt::VCmdLine().name()
          ))
        {}

        virtual ~ReplaceMatching() noexcept = default ;

        ReplaceMatching(ReplaceMatching&&) = default ;
        ReplaceMatching& operator=(ReplaceMatching&&) = default ;

        ReplaceMatching(const ReplaceMatching&) = delete ;
        ReplaceMatching& operator=(const ReplaceMatching&) = delete ;

        void launch_loop() {
            auto& igate = core::InputGate::get_instance() ;

            //reset keys downed in order to call this function.
            for(auto& key : igate.pressed_list()) {
                if(is_shift(key)) continue ;
                igate.release_keystate(key) ;
            }

            while(true) {
                bg_.update() ;

                if(igate.is_pressed(KEYCODE_ESC)) {
                    return ;
                }

                auto log = igate.pop_log() ;
                if(!log.is_containing(KEYCODE_SHIFT)) {
                    //not shifted
                    for(const auto& key : log) {
                        //For example, if replace by 'i' and 'i' key is downed,
                        //immediately will call "insert-mode", so release 'i'.
                        igate.release_keystate(key) ;

                        if(!key.to_ascii()) {
                            continue ;
                        }
                        if(!do_loop_hook(key, false)) {
                            return ;
                        }
                    }
                }
                else {
                    //shifted
                    for(const auto& key : log) {
                        if(is_shift(key)) {
                            continue ;
                        }

                        igate.release_keystate(key) ;
                        if(!key.to_ascii()) {
                            continue ;
                        }
                        if(!do_loop_hook(key, true)) {
                            return ;
                        }
                    }
                }
            }
        }
    } ;


    class ReplaceMatchingForChar : public ReplaceMatching {
    private:
        core::KeyCode captured_ ;
        bool shifted_ ;

        bool do_loop_hook(
                core::KeyCode keycode,
                bool shifted=false) override {
            captured_ = keycode ;
            shifted_ = shifted ;
            return false ; // break
        }

    public:
        explicit ReplaceMatchingForChar()
        : captured_(),
          shifted_(false)
        {}

        void launch_loop() {
            captured_ = KEYCODE_UNDEFINED ;
            shifted_ = false ;
            ReplaceMatching::launch_loop() ;
        }

        void replace_char(unsigned int repeat_num) {
            launch_loop() ;

            auto& igate = core::InputGate::get_instance() ;

            bind::safe_for(repeat_num, [this, &igate] {
                igate.pushup(KEYCODE_DELETE) ;

                if(shifted_) {
                    igate.pushup(KEYCODE_LSHIFT, captured_) ;
                }
                else {
                    igate.pushup(captured_) ;
                }
            }) ;

            // returns the cursor to its original position.
            bind::safe_for(repeat_num, [&igate] {
                igate.pushup(KEYCODE_LEFT) ;
            }) ;
        }
    } ;


    class ReplaceMatchingForSequence : public ReplaceMatching {
    private:
        std::vector<core::KeyCode> str_ ;
        std::vector<bool> shifteds_ ;

        bool do_loop_hook(
                core::KeyCode keycode,
                bool shifted=false) override {
            auto& igate = core::InputGate::get_instance() ;

            igate.pushup(KEYCODE_DELETE) ;

            if(shifted) {
                igate.pushup(KEYCODE_LSHIFT, keycode) ;
                str_.push_back(keycode) ;
            }
            else {
                igate.pushup(keycode) ;
                str_.push_back(keycode) ;
            }
            shifteds_.push_back(shifted) ;

            return true ; //continue looping
        }

    public:
        explicit ReplaceMatchingForSequence()
        : str_(),
          shifteds_()
        {}

        void launch_loop() {
            str_.clear() ;
            shifteds_.clear() ;
            ReplaceMatching::launch_loop() ;
        }

        void replace_sequence(unsigned int repeat_num) {
            launch_loop() ;

            // append the input string according to repeat_num.
            if(repeat_num > 1) {
                auto& igate = core::InputGate::get_instance() ;

                igate.release_virtually(KEYCODE_ESC) ;
                bind::safe_for(repeat_num - 1, [this, &igate] {
                    for(std::size_t i = 0 ; i < str_.size() ; i ++) {
                        igate.pushup(KEYCODE_DELETE) ;

                        if(shifteds_[i]) {
                            igate.pushup(KEYCODE_LSHIFT, str_[i]) ;
                        }
                        else {
                            igate.pushup(str_[i]) ;
                        }
                    }
                }) ;
            }
        }
    } ;
}


namespace vind
{
    namespace bind
    {
        struct ReplaceChar::Impl {
            ReplaceMatchingForChar match_{} ;
        } ;

        //EdiNRplaceChar
        ReplaceChar::ReplaceChar()
        : ChangeBaseCreator("replace_char"),
          pimpl(std::make_unique<Impl>())
        {}

        ReplaceChar::~ReplaceChar() noexcept = default ;

        ReplaceChar::ReplaceChar(ReplaceChar&&) = default ;
        ReplaceChar& ReplaceChar::operator=(ReplaceChar&&) = default ;

        void ReplaceChar::sprocess() const {
            pimpl->match_.replace_char(1) ;
        }
        void ReplaceChar::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                pimpl->match_.replace_char(parent_lgr.get_head_num()) ;
            }
        }
        void ReplaceChar::sprocess(
                const core::CharLogger& UNUSED(parent_lgr)) const {
            pimpl->match_.replace_char(1) ;
        }


        //ReplaceSequence
        struct ReplaceSequence::Impl {
            ReplaceMatchingForSequence match_{} ;
        } ;

        ReplaceSequence::ReplaceSequence()
        : ChangeBaseCreator("replace_sequence"),
          pimpl(std::make_unique<Impl>())
        {}

        ReplaceSequence::~ReplaceSequence() noexcept = default ;

        ReplaceSequence::ReplaceSequence(ReplaceSequence&&) = default ;
        ReplaceSequence& ReplaceSequence::operator=(ReplaceSequence&&) = default ;

        void ReplaceSequence::sprocess(unsigned int repeat_num) const {
            opt::VCmdLine::clear() ;
            opt::VCmdLine::print(opt::GeneralMessage("-- EDI REPLACE --")) ;

            pimpl->match_.replace_sequence(repeat_num) ;

            opt::VCmdLine::reset() ;
            opt::VCmdLine::print(opt::GeneralMessage("-- EDI NORMAL --")) ;
        }
        void ReplaceSequence::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void ReplaceSequence::sprocess(
                const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        //SwitchCharCase
        SwitchCharCase::SwitchCharCase()
        : ChangeBaseCreator("switch_char_case")
        {}
        void SwitchCharCase::sprocess(unsigned int repeat_num) {
            auto& igate = core::InputGate::get_instance() ;

            auto res = get_selected_text([&repeat_num, &igate] {
                    bind::safe_for(repeat_num, [&igate] {
                        igate.pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                    }) ;
                    igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                }) ;

            for(char c : res.str) {
                if(c >= 'a' && c <= 'z') {
                    igate.pushup(KEYCODE_LSHIFT, core::KeyCode(c)) ;
                }
                else if(c >= 'A' && c <= 'Z') {
                    constexpr char delta = 'a' - 'A' ;
                    igate.pushup(core::KeyCode(c + delta)) ;
                }
                else {
                    if(core::KeyCode::is_shifted(c)) {
                        igate.pushup(KEYCODE_LSHIFT, c) ;
                    }
                    else {
                        igate.pushup(c) ;
                    }
                }
            }
        }
        void SwitchCharCase::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void SwitchCharCase::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }
    }
}
