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
    mutex mtx ;

    explicit Impl() : flag(false), mtx() {}

    ~Impl() = default ;

    Impl(Impl&&) = default ;
    Impl& operator=(Impl&&) = default ;

    Impl(const Impl&) = delete ;
    Impl& operator=(const Impl&) = delete ;
} ;


DynamicOption::DynamicOption()
: pimpl(make_unique<Impl>())
{}

DynamicOption::~DynamicOption() noexcept = default ;

DynamicOption::DynamicOption(DynamicOption&&) = default ;
DynamicOption& DynamicOption::operator=(DynamicOption&&) = default ;

void DynamicOption::enable() noexcept
{
    pimpl->flag = true ;

    if(!do_enable()) {
        Logger::error_stream << "[Error] do_enable is failed "\
        << "(" << name() << ")\n" ;
        return ;
    }
}

void DynamicOption::disable() noexcept
{
    pimpl->flag = false ;
    if(!do_disable()) {
        Logger::error_stream << "[Error] do_disable is failed "\
        << "(" << name() << ")\n" ;
        return ;
    }
}

bool DynamicOption::is_enabled() const noexcept
{
    return pimpl->flag ;
}

void DynamicOption::process()
{
    if(!pimpl->flag) return ;

    lock_guard <mutex> lock(pimpl->mtx) ;

    if(!do_process()) {
        Logger::error_stream << "[Error] do_process is failed "\
        << "(" << name() << ")\n" ;
        return ;
    }
}