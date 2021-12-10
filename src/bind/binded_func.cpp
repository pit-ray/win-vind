#include "binded_func.hpp"

#include <array>
#include <functional>

#include "core/char_logger.hpp"
#include "core/err_logger.hpp"
#include "core/key_absorber.hpp"
#include "core/keycodecvt.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "util/keybrd.hpp"
#include "util/string.hpp"


namespace vind
{
    namespace bind
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
                for(auto& key : core::get_pressed_list()) {
                    if(!core::is_really_pressed(key)) {
                        core::release_virtually(key) ;
                    }
                }
            }

            void calibrate_absorber_state() {
                auto buf = core::get_pressed_list() ;
                if(!buf.empty()) {
                    if(core::is_absorbed()) {
                        core::open_some_ports(buf.get()) ;
                    }
                    for(auto& key : buf) {
                        util::release_keystate(key) ;
                    }
                    if(core::is_absorbed()) {
                        core::close_all_ports() ;
                        core::absorb() ;
                    }
                    else {
                        core::close_all_ports() ;
                        core::unabsorb() ;
                    }
                }
            }
        } ;

        BindedFunc::BindedFunc()
        : pimpl(std::make_unique<Impl>())
        {}
        BindedFunc::BindedFunc(const std::string& name)
        : pimpl(std::make_unique<Impl>(util::A2a(name)))
        {}
        BindedFunc::BindedFunc(std::string&& name)
        : pimpl(std::make_unique<Impl>(util::A2a(std::move(name))))
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

        core::SystemCall BindedFunc::process() const {
            auto result = core::SystemCall::NOTHING ;

            try {
                result = do_process() ;
                pimpl->release_fake_press() ;
            }
            catch(const std::runtime_error& e) {
                error_process(e) ;
            }

            return result ;
        }

        core::SystemCall BindedFunc::process(core::NTypeLogger& parent_lgr) const {
            auto result = core::SystemCall::NOTHING ;

            try {
                result = do_process(parent_lgr) ;
                pimpl->release_fake_press() ;
            }
            catch(const std::runtime_error& e) {
                error_process(e) ;
            }

            return result ;
        }

        core::SystemCall BindedFunc::process(const core::CharLogger& parent_lgr) const {
            auto result = core::SystemCall::NOTHING ;

            try {
                result = do_process(parent_lgr) ;
                pimpl->release_fake_press() ;
            }
            catch(const std::runtime_error& e) {
                error_process(e) ;
            }

            return result ;
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

        core::SystemCall BindedFunc::do_process() const {
            return core::SystemCall::NOTHING ;
        }
        core::SystemCall BindedFunc::do_process(core::NTypeLogger&) const {
            return core::SystemCall::NOTHING ;
        }
        core::SystemCall BindedFunc::do_process(const core::CharLogger&) const {
            return core::SystemCall::NOTHING ;
        }
    }
}
