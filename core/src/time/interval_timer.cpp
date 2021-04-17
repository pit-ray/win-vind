#include "time/interval_timer.hpp"
#include <chrono>
#include <mutex>

namespace vind
{
    using namespace std::chrono ;

    struct IntervalTimer::Impl
    {
        microseconds delta_time ;
        system_clock::time_point start_time ;
        bool first_call ;
        std::mutex mtx ;

        explicit Impl(const int delta_t=30)
        : delta_time(static_cast<microseconds>(delta_t)),
          start_time(system_clock::now()),
          first_call(true),
          mtx()
        {}

        virtual ~Impl() noexcept = default ;

        Impl(const Impl& rhs)
        : delta_time(rhs.delta_time),
          start_time(rhs.start_time),
          first_call(rhs.first_call),
          mtx()
        {}

        Impl& operator=(const Impl& rhs)
        {
            delta_time = rhs.delta_time ;
            start_time = rhs.start_time ;
            first_call = rhs.first_call ;
            return *this ;
        }

        Impl(Impl&& rhs)            = default ;
        Impl& operator=(Impl&& rhs) = default ;
    } ;


    IntervalTimer::IntervalTimer(const int delta_us)
    : pimpl(std::make_unique<Impl>(delta_us))
    {}

    IntervalTimer::~IntervalTimer() noexcept = default ;

    IntervalTimer::IntervalTimer(const IntervalTimer& rhs)
    : pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
    {}

    IntervalTimer& IntervalTimer::operator=(const IntervalTimer& rhs) {
        if(rhs.pimpl) *pimpl = *(rhs.pimpl) ;
        return *this ;
    }

    IntervalTimer::IntervalTimer(IntervalTimer&&)               = default ;
    IntervalTimer& IntervalTimer::operator=(IntervalTimer&&)    = default ;

    void IntervalTimer::set_delta(const int delta_us) noexcept {
        pimpl->delta_time = static_cast<microseconds>(delta_us) ;
    }

    void IntervalTimer::reset() noexcept {
        pimpl->start_time = system_clock::now() ;
        pimpl->first_call = true ;
    }

    bool IntervalTimer::is_passed() const {
        std::lock_guard<std::mutex> lock{pimpl->mtx} ;

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
}
