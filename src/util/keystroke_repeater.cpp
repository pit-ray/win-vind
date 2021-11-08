#include "keystroke_repeater.hpp"

#include <chrono>
#include <mutex>
#include <random>

#include "interval_timer.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace
{
    using namespace std::chrono ;

    inline auto uniform_accelerate(float velocity, float ms) noexcept {
        //hardcoded
        constexpr auto MAX_ACCELERATION = 1.0f ;
        constexpr auto MAX_VELOCITY     = 1.0f ;

        //ms -> s
        auto t = ms / 1000 ;

        auto delta_v = MAX_ACCELERATION * t ;
        if(velocity + delta_v < MAX_VELOCITY) {
            return velocity + delta_v ;
        }
        else {
            return MAX_VELOCITY ;
        }
    }

    inline auto compute_delta_t(const system_clock::time_point& start_time) noexcept {
        return duration_cast<milliseconds>(system_clock::now() - start_time) ;
    }

    inline auto generate_uniform() {
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
    namespace util
    {
        struct KeyStrokeRepeater::Impl {
            IntervalTimer timer_ ;
            float v_ ;
            system_clock::time_point start_time_ ;
            milliseconds wait_time_ ;
            std::mutex mtx_ ;

            explicit Impl(int wait_time_for_starting_ms=512)
            : timer_(REPEAT_SAMPLING_DELTA_US),
              v_(INITIAL_VELOCITY),
              start_time_(system_clock::now()),
              wait_time_(wait_time_for_starting_ms),
              mtx_()
            {}

            virtual ~Impl() noexcept = default ;

            Impl(const Impl& rhs)
            : timer_(rhs.timer_),
              v_(rhs.v_),
              start_time_(rhs.start_time_),
              wait_time_(rhs.wait_time_),
              mtx_()
            {}

            Impl& operator=(const Impl& rhs)
            {
                timer_      = rhs.timer_ ;
                v_          = rhs.v_ ;
                start_time_ = rhs.start_time_ ;
                wait_time_  = rhs.wait_time_ ;
                return *this ;
            }

            Impl(Impl&& rhs)            = default ;
            Impl& operator=(Impl&& rhs) = default ;
        } ;

        KeyStrokeRepeater::KeyStrokeRepeater(int wait_time_for_starting_ms)
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

        void KeyStrokeRepeater::set_wait_time(int delta_ms) {
            pimpl->wait_time_ = static_cast<milliseconds>(delta_ms) ;
        }
        int KeyStrokeRepeater::get_wait_time_ms() const noexcept {
            return static_cast<int>(duration_cast<milliseconds>(pimpl->wait_time_).count()) ;
        }

        void KeyStrokeRepeater::reset() noexcept {
            pimpl->v_ = INITIAL_VELOCITY ;
            pimpl->start_time_ = system_clock::now() ;
        }

        bool KeyStrokeRepeater::is_passed() const {
            std::lock_guard<std::mutex> scoped_lock(pimpl->mtx_) ;

            auto dt = compute_delta_t(pimpl->start_time_) ;

            if(dt < pimpl->wait_time_) return false ;

            //sampling
            if(!pimpl->timer_.is_passed()) return false ;

            pimpl->v_ = uniform_accelerate(pimpl->v_, static_cast<float>(dt.count())) ;
            if(pimpl->v_ < generate_uniform()) {
                return false ;
            }
            return true ;
        }
    }
}
