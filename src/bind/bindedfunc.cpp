#include "bindedfunc.hpp"

#include <array>
#include <cstdint>
#include <functional>

#include "core/charlogger.hpp"
#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
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
                auto& igate = core::InputGate::get_instance() ;
                for(auto& key : igate.pressed_list()) {
                    if(!igate.is_really_pressed(key)) {
                        igate.release_virtually(key) ;
                    }
                }
            }

            void calibrate_absorber_state() {
                auto& igate = core::InputGate::get_instance() ;
                auto buf = igate.pressed_list() ;
                if(!buf.empty()) {
                    if(igate.is_absorbed()) {
                        igate.open_some_ports(buf.get()) ;
                    }
                    for(auto& key : buf) {
                        igate.release_keystate(key) ;
                    }
                    if(igate.is_absorbed()) {
                        igate.close_all_ports() ;
                        igate.absorb() ;
                    }
                    else {
                        igate.close_all_ports() ;
                        igate.unabsorb() ;
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

        void BindedFunc::error_process(const std::exception& e) {
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

        SystemCall BindedFunc::process(
                std::uint16_t count, const std::string& args) {
            auto result = SystemCall::NOTHING ;

            try {
                result = do_process(count, args) ;
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

        SystemCall BindedFunc::do_process(
                std::uint16_t UNUSED(count), const std::string& UNUSED(args)) {
            return SystemCall::NOTHING ;
        }
    }
}
