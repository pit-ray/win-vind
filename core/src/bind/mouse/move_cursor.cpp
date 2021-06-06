#include "bind/mouse/move_cursor.hpp"

#include <windows.h>

#include <chrono>
#include <cmath>
#include <iostream>

#include "err_logger.hpp"
#include "g_params.hpp"
#include "io/mouse.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/math.hpp"

namespace
{
    using namespace vind ;

    using namespace std::chrono ;
    //hardcoded (The cursor move 1px necessarilly by one press)
    constexpr auto INITIAL_VELOCITY = 1.0f ;

    template <typename T>
    inline auto constant_accelerate(float& velocity, T&& us) {
        auto acc = gparams::get_f("cursor_acceleration") ;
        auto mvc = gparams::get_f("cursor_max_velocity") ;

        constexpr auto TIME_COEF = util::pow_f(10, -3) ;
        auto t = us * TIME_COEF / gparams::get_i("cursor_weight") ; //accuracy
        auto x = velocity*t + 0.5f*acc*t*t ;
        auto delta_v = acc * t ;
        if(velocity + delta_v < mvc) velocity += delta_v ;
        else velocity = mvc ;
        return x ;
    }

    inline auto compute_delta_t(
            const system_clock::time_point& start_time) {

        return duration_cast<microseconds>(
                system_clock::now() - start_time).count() ;
    }

    class MoveDeltaCalculator {
    private:
        float v_ = INITIAL_VELOCITY ;
        system_clock::time_point start_time_ = system_clock::now() ;

    public:
        void reset() noexcept {
            v_ = INITIAL_VELOCITY ;
            start_time_ = system_clock::now() ;
        }

        template <typename T>
        auto delta() {
            return static_cast<T>(
                    constant_accelerate(v_, compute_delta_t(start_time_))) ;
        }
    } ;
}

namespace vind
{
    //MoveCursorLeft
    struct MoveCursorLeft::Impl {
        MoveDeltaCalculator calcer_{} ;
    } ;

    MoveCursorLeft::MoveCursorLeft()
    : BindedFuncCreator("move_cursor_left"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorLeft::~MoveCursorLeft()                     = default ;
    MoveCursorLeft::MoveCursorLeft(MoveCursorLeft&&)            = default ;
    MoveCursorLeft& MoveCursorLeft::operator=(MoveCursorLeft&&) = default ;

    void MoveCursorLeft::sprocess(int delta) const {
        mouse::move_cursor(-delta, 0) ;
    }
    void MoveCursorLeft::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->calcer_.reset() ;
        }
        sprocess(pimpl->calcer_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    //MoveCursorRight
    struct MoveCursorRight::Impl {
        MoveDeltaCalculator calcer_{} ;
    } ;

    MoveCursorRight::MoveCursorRight()
    : BindedFuncCreator("move_cursor_right"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorRight::~MoveCursorRight()                      = default ;
    MoveCursorRight::MoveCursorRight(MoveCursorRight&&)            = default ;
    MoveCursorRight& MoveCursorRight::operator=(MoveCursorRight&&) = default ;

    void MoveCursorRight::sprocess(int delta) const {
        mouse::move_cursor(delta, 0) ;
    }
    void MoveCursorRight::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->calcer_.reset() ;
        }
        sprocess(pimpl->calcer_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorRight::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    //MoveCursorUp
    struct MoveCursorUp::Impl {
        MoveDeltaCalculator calcer_{} ;
    } ;

    MoveCursorUp::MoveCursorUp()
    : BindedFuncCreator("move_cursor_up"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorUp::~MoveCursorUp()                   = default ;
    MoveCursorUp::MoveCursorUp(MoveCursorUp&&)            = default ;
    MoveCursorUp& MoveCursorUp::operator=(MoveCursorUp&&) = default ;

    void MoveCursorUp::sprocess(int delta) const {
        mouse::move_cursor(0, -delta) ;
    }
    void MoveCursorUp::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->calcer_.reset() ;
        }
        sprocess(pimpl->calcer_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorUp::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    //MoveCursorDown
    struct MoveCursorDown::Impl {
        MoveDeltaCalculator calcer_{} ;
    } ;

    MoveCursorDown::MoveCursorDown()
    : BindedFuncCreator("move_cursor_down"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorDown::~MoveCursorDown()                     = default ;
    MoveCursorDown::MoveCursorDown(MoveCursorDown&&)            = default ;
    MoveCursorDown& MoveCursorDown::operator=(MoveCursorDown&&) = default ;

    void MoveCursorDown::sprocess(int delta) const {
        mouse::move_cursor(0, delta) ;
    }
    void MoveCursorDown::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->calcer_.reset() ;
        }
        sprocess(pimpl->calcer_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorDown::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
}
