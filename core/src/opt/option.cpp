#include "err_logger.hpp"
#include "opt/option.hpp"

#include <iostream>
#include <mutex>
#include <stdexcept>


namespace vind
{
    /*
    class-Option use in order to change on running process, use dynamically resource need to refresh resource.
    else, read directly Options-Section at ini-file.
    */

    struct Option::Impl {
        bool flag ;

        explicit Impl() : flag(false) {}
        virtual ~Impl() noexcept = default ;

        Impl(Impl&&) noexcept = default ;
        Impl& operator=(Impl&&) noexcept = default ;

        Impl(const Impl&) = default ;
        Impl& operator=(const Impl&) = default ;
    } ;


    Option::Option()
    : pimpl(std::make_unique<Impl>())
    {}

    Option::~Option() noexcept                            = default ;
    Option::Option(Option&&) noexcept              = default ;
    Option& Option::operator=(Option&&) noexcept   = default ;

    void Option::enable() {
        try {
            do_enable() ;
        }
        catch(const std::runtime_error& e) {
            PRINT_ERROR(name() + " did not enable. " + e.what()) ;
            return ;
        }

        pimpl->flag = true ;
    }

    void Option::disable() {
        try {
            do_disable() ;
        }
        catch(const std::runtime_error& e) {
            PRINT_ERROR(name() + " did not disable. " + e.what()) ;
            return ;
        }
        pimpl->flag = false ;
    }

    bool Option::is_enabled() const noexcept {
        return pimpl->flag ;
    }

    void Option::process() const {
        if(!pimpl->flag) {
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
