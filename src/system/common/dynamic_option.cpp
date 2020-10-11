#include "dynamic_option.hpp"
#include "msg_logger.hpp"

#include <iostream>
#include <mutex>
#include <stdexcept>

using namespace std ;

/*
class-DynamicOption use in order to change on running process, use dynamically resource need to refresh resource.
else, read directly Options-Section at ini-file.
*/

struct DynamicOption::Impl
{
    bool flag ;

    explicit Impl() : flag(false) {}
    virtual ~Impl() noexcept = default ;

    Impl(Impl&&) noexcept = default ;
    Impl& operator=(Impl&&) noexcept = default ;

    Impl(const Impl&) = default ;
    Impl& operator=(const Impl&) = default ;
} ;


DynamicOption::DynamicOption()
: pimpl(make_unique<Impl>())
{}

DynamicOption::~DynamicOption() noexcept                            = default ;
DynamicOption::DynamicOption(DynamicOption&&) noexcept              = default ;
DynamicOption& DynamicOption::operator=(DynamicOption&&) noexcept   = default ;

void DynamicOption::enable()
{
    try {
        do_enable() ;
    }
    catch(const std::runtime_error& e) {
        ERROR_PRINT(name() + " did not enable. " + e.what()) ;
        return ;
    }

    pimpl->flag = true ;
}

void DynamicOption::disable()
{
    try {
        do_disable() ;
    }
    catch(const std::runtime_error& e) {
        ERROR_PRINT(name() + " did not disable. " + e.what()) ;
        return ;
    }
    pimpl->flag = false ;
}

bool DynamicOption::is_enabled() const noexcept
{
    return pimpl->flag ;
}

void DynamicOption::process() const
{
    if(!pimpl->flag) {
        return ;
    }

    try {
        do_process() ;
    }
    catch(const std::runtime_error& e) {
        ERROR_PRINT(name() + " is failed. " + e.what()) ;
        return ;
    }
}
