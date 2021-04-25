#include "time/keystroke_repeater.hpp"

#include <chrono>
#include <iostream>
#include <mutex>
#include <random>

#include "time/interval_timer.hpp"

namespace
{
    using namespace std::chrono ;

    inline auto const_accelerate(float& velocity, float ms) noexcept {
        //hardcoded
        constexpr auto MAX_ACCELERATION = 1.0f ;
        constexpr auto MAX_VELOCITY     = 1.0f ;

        //ms -> s
        auto t = ms / 1000 ;

        auto delta_v = MAX_ACCELERATION * t ;
        if(velocity + delta_v < MAX_VELOCITY) {
            velocity += delta_v ;
        }
        else {
            velocity = MAX_VELOCITY ;
        }
        return velocity ;
    }

    inline auto compute_delta_t(const system_clock::time_point& start_time) noexcept {
        return duration_cast<milliseconds>(system_clock::now() - start_time) ;
    }

    inline auto _generate_uniform() {
        static std::random_device seed_gen ;
        static std::default_random_engine engine(seed_gen()) ;
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f) ;
        return dist(engine) ;
    }

    //hardcoded
    //constexpr auto WAIT_TIME_FOR_STARTING   = 512ms;
    constexpr auto REPEAT_SAMPLING_DELTA_US = 25'600 ;
    constexpr auto INITIAL_VELOCITY         = 0.000'1f ;
}

namespace vind
{
    struct KeyStrokeRepeater::Impl {
        IntervalTimer timer ;
        float v ;
        system_clock::time_point start_time ;
        milliseconds wait_time ;
        std::mutex mtx ;

        explicit Impl(unsigned int wait_time_for_starting_ms=512)
        : timer(REPEAT_SAMPLING_DELTA_US),
          v(INITIAL_VELOCITY),
          start_time(system_clock::now()),
          wait_time(wait_time_for_starting_ms),
          mtx()
        {}

        virtual ~Impl() noexcept = default ;

        Impl(const Impl& rhs)
        : timer(rhs.timer),
          v(rhs.v),
          start_time(rhs.start_time),
          wait_time(rhs.wait_time),
          mtx()
        {}

        Impl& operator=(const Impl& rhs)
        {
            timer      = rhs.timer ;
            v          = rhs.v ;
            start_time = rhs.start_time ;
            return *this ;
        }

        Impl(Impl&& rhs)            = default ;
        Impl& operator=(Impl&& rhs) = default ;
    } ;

    KeyStrokeRepeater::KeyStrokeRepeater(unsigned int wait_time_for_starting_ms)
    : pimpl(std::make_unique<Impl>(wait_time_for_starting_ms))
    {}

    KeyStrokeRepeater::~KeyStrokeRepeater() noexcept                     = default ;

    KeyStrokeRepeater::KeyStrokeRepeater(KeyStrokeRepeater&&)            = default ;
    KeyStrokeRepeater& KeyStrokeRepeater::operator=(KeyStrokeRepeater&&) = default ;

    KeyStrokeRepeater::KeyStrokeRepeater(const KeyStrokeRepeater& rhs)
    : pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
    {}
    KeyStrokeRepeater& KeyStrokeRepeater::operator=(const KeyStrokeRepeater& rhs) {
        if(rhs.pimpl) *pimpl = *(rhs.pimpl) ;
        return *this ;
    }

    void KeyStrokeRepeater::reset() noexcept {
        pimpl->v = INITIAL_VELOCITY ;
        pimpl->start_time = system_clock::now() ;
    }

    bool KeyStrokeRepeater::is_pressed() const {
        std::lock_guard<std::mutex> scoped_lock(pimpl->mtx) ;

        auto dt = compute_delta_t(pimpl->start_time) ;

        if(dt < pimpl->wait_time) return false ;

        //sampling
        if(!pimpl->timer.is_passed()) return false ;

        if(const_accelerate(pimpl->v, static_cast<float>(dt.count()))
                < _generate_uniform()) {
            return false ;
        }
        return true ;
    }
}
