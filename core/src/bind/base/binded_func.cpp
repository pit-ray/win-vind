#include "bind/base/binded_func.hpp"

#include <array>
#include <atomic>
#include <functional>

#include "io/keybrd.hpp"
#include "key/key_absorber.hpp"
#include "key/keycodecvt.hpp"
#include "bind/base/mode.hpp"
#include "coreio/err_logger.hpp"

#include "bind/base/char_logger.hpp"
#include "bind/base/ntype_logger.hpp"
#include "bind/base/keycode_logger.hpp"

namespace vind
{
    struct BindedFunc::Impl {
        std::atomic_bool running_now_ ;
        std::string name_ ;
        std::size_t id_ ;

        explicit Impl()
        : Impl("UndefinedFunction")
        {}
        explicit Impl(const std::string& name)
        : running_now_(false),
          name_(name),
          id_(std::hash<std::string>()(name_))
        {}
        explicit Impl(std::string&& name)
        : running_now_(false),
          name_(std::move(name)),
          id_(std::hash<std::string>()(name_))
        {}

        void release_fake_press() {
            //correct the state to avoid cases that a virtual key is judged to be pressed,
            //though a real key is released.
            for(auto& key : keyabsorber::get_pressed_list()) {
                if(!keyabsorber::is_really_pressed(key)) {
                    keyabsorber::release_virtually(key) ;
                }
            }
        }

        void calibrate_absorber_state() {
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
    } ;

    BindedFunc::BindedFunc()
    : pimpl(std::make_unique<Impl>())
    {}
    BindedFunc::BindedFunc(const std::string& name)
    : pimpl(std::make_unique<Impl>(name))
    {}
    BindedFunc::BindedFunc(std::string&& name)
    : pimpl(std::make_unique<Impl>(std::move(name)))
    {}

    BindedFunc::~BindedFunc() noexcept              = default ;
    BindedFunc::BindedFunc(BindedFunc&&)            = default ;
    BindedFunc& BindedFunc::operator=(BindedFunc&&) = default ;

    const std::string& BindedFunc::name() const noexcept {
        return pimpl->name_ ;
    }
    std::size_t BindedFunc::id() const noexcept {
        return pimpl->id_ ;
    }

    void BindedFunc::error_process(const std::exception& e) const {
        PRINT_ERROR(name() + " failed. " + e.what()) ;
        try {
            pimpl->calibrate_absorber_state() ;
        }
        catch(const std::runtime_error& e2) {
            PRINT_ERROR(name()
                    + " failed. Cannot refresh all key state. "
                    + e2.what()) ;
        }
    }

    void BindedFunc::process() const {
        pimpl->running_now_.store(true) ;
        try {
            do_process() ;
            pimpl->release_fake_press() ;
        }
        catch(const std::runtime_error& e) {
            error_process(e) ;
        }
        pimpl->running_now_.store(false) ;
    }

    void BindedFunc::process(NTypeLogger& parent_lgr) const {
        pimpl->running_now_.store(true) ;
        try {
            do_process(parent_lgr) ;
            pimpl->release_fake_press() ;
        }
        catch(const std::runtime_error& e) {
            error_process(e) ;
        }
        pimpl->running_now_.store(false) ;
    }

    void BindedFunc::process(const CharLogger& parent_lgr) const {
        pimpl->running_now_.store(true) ;
        try {
            do_process(parent_lgr) ;
            pimpl->release_fake_press() ;
        }
        catch(const std::runtime_error& e) {
            error_process(e) ;
        }
        pimpl->running_now_.store(false) ;
    }

    bool BindedFunc::is_for_moving_caret() const noexcept {
        return false ;
    }

    void BindedFunc::load_config() {
        return ;
    }
}
