#ifndef _CONSTANT_ACCELERATOR_HPP
#define _CONSTANT_ACCELERATOR_HPP

#include <memory>

namespace vind
{
    class ConstAccelerator {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        float compute_delta() noexcept ;

    public:
        explicit ConstAccelerator() ;
        explicit ConstAccelerator(
                float acceleration,
                float max_velocity,
                int time_weight,
                float initial_velocity=1.0f) ;

        virtual ~ConstAccelerator() noexcept ;

        ConstAccelerator(ConstAccelerator&&) ;
        ConstAccelerator& operator=(ConstAccelerator&&) ;

        ConstAccelerator(const ConstAccelerator&)            = delete ;
        ConstAccelerator& operator=(const ConstAccelerator&) = delete ;

        void set_initial_velocity(float velocity) noexcept ;
        void set_acceleration(float acceleration) noexcept ;
        void set_max_velocity(float velocity) noexcept ;
        void set_time_weight(int weight) noexcept ;

        float get_initial_velocity() noexcept ;
        float get_acceleration() noexcept ;
        float get_max_velocity() noexcept ;
        int get_time_weight() noexcept ;

        virtual void reset() noexcept ;

        template <typename T>
        auto delta() {
            return static_cast<T>(compute_delta()) ;
        }
    } ;
}

#endif
