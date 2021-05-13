#include "bind/mouse/move_cursor.hpp"

#include <windows.h>

#include <chrono>
#include <cmath>
#include <iostream>

#include "err_logger.hpp"
#include "i_params.hpp"
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
        auto acc = iparams::get_f("cursor_acceleration") ;
        auto mvc = iparams::get_f("cursor_max_velocity") ;

        constexpr auto TIME_COEF = util::pow_f(10, -3) ;
        auto t = us * TIME_COEF / iparams::get_i("cursor_weight") ; //accuracy
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
    //MoveLeft
    struct MoveLeft::Impl {
        MoveDeltaCalculator calcer_{} ;
    } ;

    MoveLeft::MoveLeft()
    : BindedFuncCreator("move_left"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveLeft::~MoveLeft()                     = default ;
    MoveLeft::MoveLeft(MoveLeft&&)            = default ;
    MoveLeft& MoveLeft::operator=(MoveLeft&&) = default ;

    void MoveLeft::sprocess(int delta) const {
        mouse::move_cursor(-delta, 0) ;
    }
    void MoveLeft::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->calcer_.reset() ;
        }
        sprocess(pimpl->calcer_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    //MoveRight
    struct MoveRight::Impl {
        MoveDeltaCalculator calcer_{} ;
    } ;

    MoveRight::MoveRight()
    : BindedFuncCreator("move_right"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveRight::~MoveRight()                      = default ;
    MoveRight::MoveRight(MoveRight&&)            = default ;
    MoveRight& MoveRight::operator=(MoveRight&&) = default ;

    void MoveRight::sprocess(int delta) const {
        mouse::move_cursor(delta, 0) ;
    }
    void MoveRight::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->calcer_.reset() ;
        }
        sprocess(pimpl->calcer_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveRight::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    //MoveUp
    struct MoveUp::Impl {
        MoveDeltaCalculator calcer_{} ;
    } ;

    MoveUp::MoveUp()
    : BindedFuncCreator("move_up"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveUp::~MoveUp()                   = default ;
    MoveUp::MoveUp(MoveUp&&)            = default ;
    MoveUp& MoveUp::operator=(MoveUp&&) = default ;

    void MoveUp::sprocess(int delta) const {
        mouse::move_cursor(0, -delta) ;
    }
    void MoveUp::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->calcer_.reset() ;
        }
        sprocess(pimpl->calcer_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveUp::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    //MoveDown
    struct MoveDown::Impl {
        MoveDeltaCalculator calcer_{} ;
    } ;

    MoveDown::MoveDown()
    : BindedFuncCreator("move_down"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveDown::~MoveDown()                     = default ;
    MoveDown::MoveDown(MoveDown&&)            = default ;
    MoveDown& MoveDown::operator=(MoveDown&&) = default ;

    void MoveDown::sprocess(int delta) const {
        mouse::move_cursor(0, delta) ;
    }
    void MoveDown::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->calcer_.reset() ;
        }
        sprocess(pimpl->calcer_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveDown::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
}
