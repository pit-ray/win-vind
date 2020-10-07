#include "interval_timer.hpp"
#include <mutex>
#include <chrono>

using namespace std ;
using namespace std::chrono ;

struct IntervalTimer::Impl
{
    microseconds delta_time ;
    system_clock::time_point start_time ;
    bool first_call ;
    mutex mtx ;

    explicit Impl(const int delta_t) noexcept
    : delta_time(static_cast<microseconds>(delta_t)),
      start_time(system_clock::now()),
      first_call(true),
      mtx()
    {}
} ;


IntervalTimer::IntervalTimer(const int delta_us)
: pimpl(make_unique<Impl>(delta_us))
{}

IntervalTimer::~IntervalTimer() noexcept = default ;

IntervalTimer::IntervalTimer(IntervalTimer&&) noexcept = default ;
IntervalTimer& IntervalTimer::operator=(IntervalTimer&&) noexcept = default ;

void IntervalTimer::set_delta(const int delta_us) noexcept
{
    pimpl->delta_time = static_cast<microseconds>(delta_us) ;
}

void IntervalTimer::reset() noexcept
{
    pimpl->start_time = system_clock::now() ;
    pimpl->first_call = true ;
}

bool IntervalTimer::is_passed() const noexcept
{
    lock_guard<mutex> lock{pimpl->mtx} ;

    if(pimpl->first_call) {
        pimpl->first_call = false ;
        return true ;
    }

    if((system_clock::now() - pimpl->start_time) < pimpl->delta_time) {
        return false ;
    }

    pimpl->start_time = system_clock::now() ;
    return true ;
}
