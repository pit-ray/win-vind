#include "bind/binded_func.hpp"

#include <array>
#include <functional>

#include "bind/emu/edi_dot.hpp"
#include "err_logger.hpp"
#include "io/keybrd.hpp"
#include "key/key_absorber.hpp"
#include "key/keycodecvt.hpp"
#include "mode.hpp"

#include "key/char_logger.hpp"
#include "key/ntype_logger.hpp"

namespace vind
{
    struct BindedFunc::Impl {
        std::string name_ ;
        std::size_t id_ ;

        explicit Impl()
        : Impl("undefined_function")
        {}
        explicit Impl(const std::string& name)
        : name_(name),
          id_(name_to_id(name_))
        {}
        explicit Impl(std::string&& name)
        : name_(std::move(name)),
          id_(name_to_id(name_))
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
            auto buf = keyabsorber::get_pressed_list() ;
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
    const std::size_t& BindedFunc::id() const noexcept {
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
        try {
            do_process() ;
            pimpl->release_fake_press() ;
        }
        catch(const std::runtime_error& e) {
            error_process(e) ;
        }
    }

    void BindedFunc::process(NTypeLogger& parent_lgr) const {
        try {
            do_process(parent_lgr) ;
            pimpl->release_fake_press() ;

            if(is_for_changing_text()) {
                RepeatLastChange::store_change(
                        this, parent_lgr.get_head_num()) ;
            }
        }
        catch(const std::runtime_error& e) {
            error_process(e) ;
        }
    }

    void BindedFunc::process(const CharLogger& parent_lgr) const {
        try {
            do_process(parent_lgr) ;
            pimpl->release_fake_press() ;

            if(is_for_changing_text()) {
                RepeatLastChange::store_change(this) ;
            }
        }
        catch(const std::runtime_error& e) {
            error_process(e) ;
        }
    }

    bool BindedFunc::is_for_moving_caret() const noexcept {
        return false ;
    }

    bool BindedFunc::is_for_changing_text() const noexcept {
        return false ;
    }

    void BindedFunc::reconstruct() {
        return ;
    }

    bool BindedFunc::operator==(const BindedFunc& rhs) const noexcept {
        return pimpl->id_ == rhs.pimpl->id_ ;
    }
    bool BindedFunc::operator==(BindedFunc&& rhs) const noexcept {
        return pimpl->id_ == rhs.pimpl->id_ ;
    }

    bool BindedFunc::operator!=(const BindedFunc& rhs) const noexcept {
        return pimpl->id_ != rhs.pimpl->id_ ;
    }
    bool BindedFunc::operator!=(BindedFunc&& rhs) const noexcept {
        return pimpl->id_ != rhs.pimpl->id_ ;
    }
}
