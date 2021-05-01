#include "time/interval_timer.hpp"
#include <chrono>
#include <mutex>

namespace vind
{
    using namespace std::chrono ;

    struct IntervalTimer::Impl
    {
        microseconds delta_time_ ;
        system_clock::time_point start_time_ ;
        bool first_call_ ;
        std::mutex mtx_ ;

        explicit Impl(int delta_t=30)
        : delta_time_(static_cast<microseconds>(delta_t)),
          start_time_(system_clock::now()),
          first_call_(true),
          mtx_()
        {}

        virtual ~Impl() noexcept = default ;

        Impl(const Impl& rhs)
        : delta_time_(rhs.delta_time_),
          start_time_(rhs.start_time_),
          first_call_(rhs.first_call_),
          mtx_()
        {}

        Impl& operator=(const Impl& rhs)
        {
            delta_time_ = rhs.delta_time_ ;
            start_time_ = rhs.start_time_ ;
            first_call_ = rhs.first_call_ ;
            return *this ;
        }

        Impl(Impl&& rhs)            = default ;
        Impl& operator=(Impl&& rhs) = default ;
    } ;


    IntervalTimer::IntervalTimer(int delta_us)
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

    void IntervalTimer::set_delta(int delta_us) noexcept {
        pimpl->delta_time_ = static_cast<microseconds>(delta_us) ;
    }

    int IntervalTimer::get_delta_us() const noexcept{
        return static_cast<int>(duration_cast<microseconds>(pimpl->delta_time_).count()) ;
    }

    void IntervalTimer::reset() noexcept {
        pimpl->start_time_ = system_clock::now() ;
        pimpl->first_call_ = true ;
    }

    bool IntervalTimer::is_passed() const {
        std::lock_guard<std::mutex> lock{pimpl->mtx_} ;

        if(pimpl->first_call_) {
            pimpl->first_call_ = false ;
            return true ;
        }

        if((system_clock::now() - pimpl->start_time_) < pimpl->delta_time_) {
            return false ;
        }

        pimpl->start_time_ = system_clock::now() ;
        return true ;
    }
}
