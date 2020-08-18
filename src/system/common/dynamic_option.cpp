#include "dynamic_option.hpp"
#include "msg_logger.hpp"

#include <iostream>
#include <mutex>

using namespace std ;

/*
class-DynamicOption use in order to change on running process, use dynamically resource need to refresh resource.
else, read directly Options-Section at ini-file.
*/

struct DynamicOption::Impl
{
    bool flag ;

    explicit Impl() : flag(false) {}

    ~Impl() = default ;

    Impl(Impl&&) noexcept = default ;
    Impl& operator=(Impl&&) noexcept = default ;

    Impl(const Impl&) = default ;
    Impl& operator=(const Impl&) = default ;
} ;


DynamicOption::DynamicOption()
: pimpl(make_unique<Impl>())
{}

DynamicOption::~DynamicOption() noexcept = default ;

DynamicOption::DynamicOption(DynamicOption&&) noexcept = default ;
DynamicOption& DynamicOption::operator=(DynamicOption&&) noexcept = default ;

void DynamicOption::enable() noexcept
{
    pimpl->flag = true ;

    if(!do_enable()) {
        ERROR_STREAM << "do_enable is failed " << "(" << name() << ")\n" ;
        return ;
    }
}

void DynamicOption::disable() noexcept
{
    pimpl->flag = false ;
    if(!do_disable()) {
        ERROR_STREAM << "do_disable is failed " << "(" << name() << ")\n" ;
        return ;
    }
}

bool DynamicOption::is_enabled() const noexcept
{
    return pimpl->flag ;
}

void DynamicOption::process() const
{
    if(!pimpl->flag) return ;

    if(!do_process()) {
        ERROR_STREAM << "do_process is failed " << "(" << name() << ")\n" ;
        return ;
    }
}