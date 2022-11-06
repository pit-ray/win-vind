#include "movecursor.hpp"

#include <windows.h>

#include <chrono>
#include <cmath>
#include <functional>
#include <future>

#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/settable.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/math.hpp"
#include "util/mouse.hpp"


namespace
{
    using namespace vind ;
    using namespace std::chrono ;

    class AsyncConstAccelerator {
    private:
        float accel_ ;
        int time_w_ ;

        std::future<void> ft_ ;
        std::function<void(int)> func_ ;
        std::atomic_bool continue_flag_ ;
        core::CmdUnit triggered_keys_ ;

    public:
        explicit AsyncConstAccelerator()
        : accel_(1.0f),
          time_w_(100),
          ft_(),
          func_(),
          continue_flag_(false),
          triggered_keys_()
        {}

        bool is_running() const noexcept {
            return continue_flag_ ;
        }

        void cancel() noexcept {
            continue_flag_ = false ;
        }

        void launch(const std::function<void(int)>& func) {
            if(ft_.valid()) {
                ft_.wait() ;
                ft_.get() ;
            }

            func_ = func ;
            continue_flag_ = true ;

            // When moving a mouse, discrete and variable sensory signals,
            // such as text input signals, are not smooth. Therefore, to
            // make it closer to continuous, it asynchronously checks the
            // current input state and maintains a constant accelerated
            // motion if it is the same as the input state at the time it
            // is called at first time.
            triggered_keys_ = core::InputGate::get_instance().pressed_list() ;
            if(triggered_keys_.empty()) {
                return ;
            }

            ft_ = std::async(
                std::launch::async,
                &AsyncConstAccelerator::loop,
                this) ;
        }

        void loop() {
            constexpr auto to_s = util::pow_f(10, -3) ;
            constexpr float v0 = 1.0f ;

            auto start = system_clock::now() ;
            while(continue_flag_) {
                auto inputs = core::InputGate::get_instance().pressed_list() ;
                // If the current key states is different from the first time set, breaks it.
                if(triggered_keys_ != inputs) {
                    break ;
                }

                auto nt = system_clock::now() ;
                auto t = static_cast<float>(duration_cast<microseconds>(nt - start).count()) ;
                // The time scale of constant accelerated motion
                // contributes directly to smoothness. Therefore,
                // smoothness is a user-modifiable parameter.
                t *= to_s / time_w_ ;
                auto x = v0 * t + 0.5f * accel_ * t * t ;

                func_(static_cast<int>(x)) ;

                Sleep(5) ;
            }
            continue_flag_ = false ;
        }

        void set_acceleration(float acceleration) noexcept {
            accel_ = acceleration ;
        }

        void set_time_weight(int weight) noexcept {
            time_w_ = weight <= 0 ? 1 : weight ;
        }
    } ;
}


namespace vind
{
    namespace bind
    {
        //MoveCursorLeft
        struct MoveCursorLeft::Impl {
            AsyncConstAccelerator accel_{} ;
        } ;
        MoveCursorLeft::MoveCursorLeft()
        : BindedFuncVoid("move_cursor_left"),
          pimpl(std::make_unique<Impl>())
        {}
        MoveCursorLeft::~MoveCursorLeft() noexcept                  = default ;
        MoveCursorLeft::MoveCursorLeft(MoveCursorLeft&&)            = default ;
        MoveCursorLeft& MoveCursorLeft::operator=(MoveCursorLeft&&) = default ;

        void MoveCursorLeft::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            if(!pimpl->accel_.is_running()) {
                pimpl->accel_.launch(
                    [count](int delta) {
                        util::move_cursor(-delta * count, 0) ;
                    }) ;
            }
        }
        void MoveCursorLeft::reconstruct() {
            auto& settable = core::SetTable::get_instance() ;
            pimpl->accel_.set_acceleration(
                    settable.get("cursor_accel").get<float>()) ;
            pimpl->accel_.set_time_weight(
                    settable.get("cursor_resolution").get<int>()) ;
        }

        //MoveCursorRight
        struct MoveCursorRight::Impl {
            AsyncConstAccelerator accel_{} ;
        } ;
        MoveCursorRight::MoveCursorRight()
        : BindedFuncVoid("move_cursor_right"),
          pimpl(std::make_unique<Impl>())
        {}
        MoveCursorRight::~MoveCursorRight() noexcept                   = default ;
        MoveCursorRight::MoveCursorRight(MoveCursorRight&&)            = default ;
        MoveCursorRight& MoveCursorRight::operator=(MoveCursorRight&&) = default ;

        void MoveCursorRight::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            if(!pimpl->accel_.is_running()) {
                pimpl->accel_.launch(
                    [count](int delta) {
                        util::move_cursor(count * delta, 0) ;
                    }) ;
            }
        }
        void MoveCursorRight::reconstruct() {
            auto& settable = core::SetTable::get_instance() ;
            pimpl->accel_.set_acceleration(
                    settable.get("cursor_accel").get<float>()) ;
            pimpl->accel_.set_time_weight(
                    settable.get("cursor_resolution").get<int>()) ;
        }

        //MoveCursorUp
        struct MoveCursorUp::Impl {
            AsyncConstAccelerator accel_{} ;
        } ;
        MoveCursorUp::MoveCursorUp()
        : BindedFuncVoid("move_cursor_up"),
          pimpl(std::make_unique<Impl>())
        {}
        MoveCursorUp::~MoveCursorUp() noexcept                = default ;
        MoveCursorUp::MoveCursorUp(MoveCursorUp&&)            = default ;
        MoveCursorUp& MoveCursorUp::operator=(MoveCursorUp&&) = default ;

        void MoveCursorUp::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            if(!pimpl->accel_.is_running()) {
                pimpl->accel_.launch(
                    [count](int delta) {
                        util::move_cursor(0, -delta * count) ;
                    }) ;
            }
        }
        void MoveCursorUp::reconstruct() {
            auto& settable = core::SetTable::get_instance() ;
            pimpl->accel_.set_acceleration(
                    settable.get("cursor_accel").get<float>()) ;
            pimpl->accel_.set_time_weight(
                    settable.get("cursor_resolution").get<int>()) ;
        }

        //MoveCursorDown
        struct MoveCursorDown::Impl {
            AsyncConstAccelerator accel_{} ;
        } ;
        MoveCursorDown::MoveCursorDown()
        : BindedFuncVoid("move_cursor_down"),
          pimpl(std::make_unique<Impl>())
        {}
        MoveCursorDown::~MoveCursorDown() noexcept                  = default ;
        MoveCursorDown::MoveCursorDown(MoveCursorDown&&)            = default ;
        MoveCursorDown& MoveCursorDown::operator=(MoveCursorDown&&) = default ;

        void MoveCursorDown::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            if(!pimpl->accel_.is_running()) {
                pimpl->accel_.launch(
                    [count](int delta) {
                        util::move_cursor(0, delta * count) ;
                    }) ;
            }
        }
        void MoveCursorDown::reconstruct() {
            auto& settable = core::SetTable::get_instance() ;
            pimpl->accel_.set_acceleration(
                    settable.get("cursor_accel").get<float>()) ;
            pimpl->accel_.set_time_weight(
                    settable.get("cursor_resolution").get<int>()) ;
        }
    }
}
