#include "constant_accelerator.hpp"

#include <chrono>

#include "math.hpp"


namespace vind
{
    struct ConstAccelerator::Impl {
        float v_ ;
        float init_v_ ;
        float max_v_ ;

        float accel_ ;

        int time_w_ ;

        std::chrono::system_clock::time_point start_ ;

        explicit Impl(
                float acceleration,
                float max_velocity,
                int time_weight,
                float initial_velocity)
        : v_(initial_velocity),
          init_v_(initial_velocity),
          max_v_(max_velocity),
          accel_(acceleration),
          time_w_(time_weight),
          start_(std::chrono::system_clock::now())
        {}

        inline auto delta_t() const noexcept {
            using namespace std::chrono ;
            return duration_cast<microseconds>(
                    system_clock::now() - start_).count() ;
        }
    } ;

    ConstAccelerator::ConstAccelerator()
    : pimpl(std::make_unique<Impl>(95.0f, 12.0f, 250, 1.0f))
    {}

    ConstAccelerator::ConstAccelerator(
            float acceleration,
            float max_velocity,
            int time_weight,
            float initial_velocity)
    : pimpl(std::make_unique<Impl>(
                acceleration,
                max_velocity,
                time_weight,
                initial_velocity))
    {}

    ConstAccelerator::~ConstAccelerator() noexcept                    = default ;
    ConstAccelerator::ConstAccelerator(ConstAccelerator&&)            = default ;
    ConstAccelerator& ConstAccelerator::operator=(ConstAccelerator&&) = default ;

    void ConstAccelerator::set_initial_velocity(float velocity) noexcept {
        pimpl->init_v_ = velocity ;
    }
    void ConstAccelerator::set_acceleration(float acceleration) noexcept {
        pimpl->accel_ = acceleration ;
    }
    void ConstAccelerator::set_max_velocity(float velocity) noexcept {
        pimpl->max_v_ = velocity ;
    }
    void ConstAccelerator::set_time_weight(int weight) noexcept {
        pimpl->time_w_ = weight ;
    }

    float ConstAccelerator::get_initial_velocity() noexcept {
        return pimpl->init_v_ ;
    }
    float ConstAccelerator::get_acceleration() noexcept {
        return pimpl->accel_ ;
    }
    float ConstAccelerator::get_max_velocity() noexcept {
        return pimpl->max_v_ ;
    }
    int ConstAccelerator::get_time_weight() noexcept {
        return pimpl->time_w_ ;
    }

    void ConstAccelerator::reset() noexcept {
        pimpl->v_ = pimpl->init_v_ ;
        pimpl->start_ = std::chrono::system_clock::now() ;
    }

    float ConstAccelerator::compute_delta() noexcept {
        constexpr auto TIME_COEF = util::pow_f(10, -3) ;
        auto t = pimpl->delta_t() * TIME_COEF / pimpl->time_w_ ;
        auto x = pimpl->v_*t + 0.5f*pimpl->accel_*t*t ;
        auto delta_v = pimpl->accel_ * t ;
        if(pimpl->v_ + delta_v < pimpl->max_v_) {
            pimpl->v_ += delta_v ;
        }
        else {
            pimpl->v_ = pimpl->max_v_ ;
        }
        return x ;
    }
}
