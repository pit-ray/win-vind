#include "replacetext.hpp"

#include "bind/saferepeat.hpp"
#include "core/background.hpp"
#include "core/entry.hpp"
#include "core/inputgate.hpp"
#include "core/keycode.hpp"
#include "core/keycodedef.hpp"
#include "opt/blockstylecaret.hpp"
#include "opt/optionlist.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"
#include "smartclipboard.hpp"
#include "textutil.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"

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
                const core::KeyCode& UNUSED(keycode),
                const core::KeyCode& UNUSED(shift)) {
            return false ;
        }

    public:
        explicit ReplaceMatching()
        : bg_(opt::search_options(
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

                auto inputs = igate.pressed_list() ;
                for(const auto& keycode : inputs) {
                    if(keycode.is_major_system()) {
                        continue ;
                    }

                    auto uni = core::keycode_to_unicode(keycode, inputs) ;
                    if(!uni.empty()) {
                        auto shift = core::get_shift_keycode(uni.front()) ;

                        igate.release_keystate(keycode) ;
                        if(!do_loop_hook(keycode, shift)) {
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
        core::KeyCode shift_ ;

        bool do_loop_hook(
                const core::KeyCode& keycode,
                const core::KeyCode& shift) override {
            captured_ = keycode ;
            shift_    = shift ;
            return false ; // break
        }

    public:
        explicit ReplaceMatchingForChar()
        : captured_(),
          shift_()
        {}

        void launch_loop() {
            captured_ = KEYCODE_UNDEFINED ;
            shift_ = KEYCODE_UNDEFINED ;
            ReplaceMatching::launch_loop() ;
        }

        void replace_char(unsigned int count) {
            launch_loop() ;

            auto& igate = core::InputGate::get_instance() ;

            bind::safe_for(count, [this, &igate] {
                igate.pushup(KEYCODE_DELETE) ;

                if(shift_) {
                    igate.pushup(shift_, captured_) ;
                }
                else {
                    igate.pushup(captured_) ;
                }
            }) ;

            // returns the cursor to its original position.
            bind::safe_for(count, [&igate] {
                igate.pushup(KEYCODE_LEFT) ;
            }) ;
        }
    } ;


    class ReplaceMatchingForSequence : public ReplaceMatching {
    private:
        std::vector<core::KeyCode> str_ ;
        std::vector<core::KeyCode> shifts_ ;

        bool do_loop_hook(
                const core::KeyCode& keycode,
                const core::KeyCode& shift) override {
            auto& igate = core::InputGate::get_instance() ;

            igate.pushup(KEYCODE_DELETE) ;

            if(shift) {
                igate.pushup(shift, keycode) ;
                str_.push_back(keycode) ;
            }
            else {
                igate.pushup(keycode) ;
                str_.push_back(keycode) ;
            }
            shifts_.push_back(shift) ;

            return true ; //continue looping
        }

    public:
        explicit ReplaceMatchingForSequence()
        : str_(),
          shifts_()
        {}

        void launch_loop() {
            str_.clear() ;
            shifts_.clear() ;
            ReplaceMatching::launch_loop() ;
        }

        void replace_sequence(unsigned int count) {
            launch_loop() ;

            // append the input string according to count.
            if(count > 1) {
                auto& igate = core::InputGate::get_instance() ;

                igate.release_virtually(KEYCODE_ESC) ;
                bind::safe_for(count - 1, [this, &igate] {
                    for(std::size_t i = 0 ; i < str_.size() ; i ++) {
                        igate.pushup(KEYCODE_DELETE) ;

                        if(shifts_[i]) {
                            igate.pushup(shifts_[i], str_[i]) ;
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

        void ReplaceChar::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            pimpl->match_.replace_char(count) ;
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

        void ReplaceSequence::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            opt::VCmdLine::clear() ;
            opt::VCmdLine::print(opt::GeneralMessage("-- EDI REPLACE --")) ;

            pimpl->match_.replace_sequence(count) ;

            opt::VCmdLine::reset() ;
            opt::VCmdLine::print(opt::GeneralMessage("-- EDI NORMAL --")) ;
        }

        //SwitchCharCase
        SwitchCharCase::SwitchCharCase()
        : ChangeBaseCreator("switch_char_case")
        {}
        void SwitchCharCase::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto hwnd = util::get_foreground_window() ;
            if(!hwnd) {
                throw std::runtime_error("There is no foreground window.") ;
            }

            auto& igate = core::InputGate::get_instance() ;

            auto res = get_selected_text([count, &igate] {
                bind::safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                }) ;
                igate.pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            }) ;

            for(auto& c : res.str) {
                if('a' <= c && c <= 'z') {
                    c -= ('a' - 'A') ;
                }
                else if('A' <= c && c <= 'Z') {
                    c += ('a' - 'A') ;
                }
            }

            SmartClipboard scb(hwnd) ;
            scb.open() ;
            scb.set(res.str) ;
            scb.close() ;

            Sleep(30) ;
            igate.pushup(KEYCODE_LSHIFT, KEYCODE_INSERT) ;
        }
    }
}
