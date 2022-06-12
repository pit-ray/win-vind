#include "opt/option.hpp"

#include "core/errlogger.hpp"

#include <mutex>
#include <stdexcept>


namespace vind
{
    namespace opt
    {
        /*
        class-Option use in order to change on running process, use dynamically resource need to refresh resource.
        else, read directly Options-Section at ini-file.
        */

        struct Option::Impl {
            bool flag_ ;
            std::string name_ ;

            explicit Impl(const std::string& name)
            : flag_(false),
              name_(name)
            {}

            explicit Impl(std::string&& name)
            : flag_(false),
              name_(std::move(name))
            {}

            virtual ~Impl() noexcept = default ;

            Impl(Impl&&) noexcept = default ;
            Impl& operator=(Impl&&) noexcept = default ;

            Impl(const Impl&) = default ;
            Impl& operator=(const Impl&) = default ;
        } ;

        Option::Option(const std::string& name)
        : pimpl(std::make_unique<Impl>(name))
        {}

        Option::Option(std::string&& name)
        : pimpl(std::make_unique<Impl>(std::move(name)))
        {}

        Option::~Option() noexcept = default ;
        Option::Option(Option&&) noexcept = default ;
        Option& Option::operator=(Option&&) noexcept = default ;

        const std::string& Option::name() const noexcept {
            return pimpl->name_ ;
        }

        void Option::enable() {
            try {
                do_enable() ;
            }
            catch(const std::runtime_error& e) {
                PRINT_ERROR(name() + " did not enable. " + e.what()) ;
                return ;
            }

            pimpl->flag_ = true ;
        }

        void Option::disable() {
            try {
                do_disable() ;
            }
            catch(const std::runtime_error& e) {
                PRINT_ERROR(name() + " did not disable. " + e.what()) ;
                return ;
            }
            pimpl->flag_ = false ;
        }

        bool Option::is_enabled() const noexcept {
            return pimpl->flag_ ;
        }

        void Option::process() {
            if(!pimpl->flag_) {
                return ;
            }

            try {
                do_process() ;
            }
            catch(const std::runtime_error& e) {
                PRINT_ERROR(name() + " failed. " + e.what()) ;
                return ;
            }
        }
    }
}
