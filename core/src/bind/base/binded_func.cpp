#include "bind/base/binded_func.hpp"

#include <array>
#include <atomic>

#include "io/keybrd.hpp"
#include "bind/base/char_logger.hpp"
#include "key/key_absorber.hpp"
#include "key/keycodecvt.hpp"
#include "bind/base/keycode_logger.hpp"
#include "bind/base/mode.hpp"
#include "coreio/err_logger.hpp"

namespace vind
{
    struct BindedFunc::Impl {
        std::array<BindingsMatcher::SPtr, static_cast<int>(mode::Mode::NUM)> matchers_ ;
        unsigned char current_mode_ ;
        std::atomic_bool running_now_ ;

        explicit Impl()
        : matchers_(),
          current_mode_(0),
          running_now_(false)
        {
            matchers_.fill(nullptr) ;
        }
    } ;

    BindedFunc::BindedFunc()
    : pimpl(std::make_unique<Impl>())
    {}

    BindedFunc::~BindedFunc() noexcept              = default ;
    BindedFunc::BindedFunc(BindedFunc&&)            = default ;
    BindedFunc& BindedFunc::operator=(BindedFunc&&) = default ;

    void BindedFunc::process(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* parent_keycodelgr,
            const CharLogger* const parent_charlgr) const {
        if(repeat_num == 0) return ;

        pimpl->running_now_.store(true) ;
        try {
            do_process(first_call, repeat_num, parent_keycodelgr, parent_charlgr) ;

            //correct the state
            //to avoid cases that a virtual key is judged to be pressed,
            //though a real key is released.
            for(auto& key : keyabsorber::get_pressed_list()) {
                if(!keyabsorber::is_really_pressed(key)) {
                    keyabsorber::release_virtually(key) ;
                }
            }
        }
        catch(const std::runtime_error& e) {
            PRINT_ERROR(name() + " failed. " + e.what()) ;
            try {
                const auto buf = keyabsorber::get_pressed_list() ;
                if(!buf.empty()) {
                    if(keyabsorber::is_absorbed()) {
                        keyabsorber::open_some_ports(buf.get()) ;
                    }
                    for(auto& key : buf) {
                        keybrd::release_keystate(key) ;
                    }
                    if(keyabsorber::is_absorbed()) {
                        keyabsorber::close_all_ports() ;
                        keyabsorber::absorb() ;
                    }
                    else {
                        keyabsorber::close_all_ports() ;
                        keyabsorber::unabsorb() ;
                    }
                }
            }
            catch(const std::runtime_error& e2) {
                PRINT_ERROR(name()
                        + " failed. Cannot refresh all key state. "
                        + e2.what()) ;
            }
        }
        pimpl->running_now_.store(false) ;
    }

    void BindedFunc::register_matcher(
            const mode::Mode mode,
            const BindingsMatcher::SPtr matcher) const {
        pimpl->matchers_.at(static_cast<unsigned char>(mode)) = matcher ;
    }

    void BindedFunc::register_matcher(
            const unsigned char mode,
            const BindingsMatcher::SPtr matcher) const {
        pimpl->matchers_.at(mode) = matcher ;
    }

    unsigned int BindedFunc::validate_if_match(
            const KeyLoggerBase& lgr,
            mode::Mode mode) const {
        if(pimpl->running_now_.load()) return 0 ;

        pimpl->current_mode_ = static_cast<unsigned char>(mode) ;
        if(auto& ptr = pimpl->matchers_.at(pimpl->current_mode_))
            return ptr->compare_to_latestlog(lgr) ;

        return 0 ;
    }

    unsigned int BindedFunc::validate_if_fullmatch(
            const KeyLoggerBase& lgr,
            mode::Mode mode) const {
        if(pimpl->running_now_.load()) return 0 ;

        pimpl->current_mode_ = static_cast<unsigned char>(mode) ;
        if(auto& ptr = pimpl->matchers_.at(pimpl->current_mode_))
            return ptr->compare_to_alllog(lgr) ;

        return 0 ;
    }

    bool BindedFunc::is_callable() const noexcept {
        if(pimpl->running_now_.load()) return false ;
        if(auto& ptr = pimpl->matchers_.at(pimpl->current_mode_))
            return ptr->is_accepted() ;
        return false ;
    }

    bool BindedFunc::is_for_moving_caret() const noexcept {
        return false ;
    }

    void BindedFunc::load_config() {
        return ;
    }

    bool BindedFunc::is_matched_syskey_in_combined_bindings() const noexcept {
        if(pimpl->running_now_.load()) return false ;
        if(auto& ptr = pimpl->matchers_.at(pimpl->current_mode_))
            return ptr->is_matched_syskey_in_combined_bindings() ;
        return false ;
    }
}
