#include "bind/mouse/move_cursor.hpp"

#include <windows.h>

#include <chrono>
#include <iostream>
#include <cmath>

#include "coreio/i_params.hpp"
#include "coreio/err_logger.hpp"
#include "util/def.hpp"
#include "io/mouse.hpp"
#include "util/math.hpp"
#include "bind/base/ntype_logger.hpp"

namespace
{
    using namespace vind ;

    using namespace std::chrono ;
    //hardcoded (The cursor move 1px necessarilly by one press)
    constexpr auto INITIAL_VELOCITY = 1.0f ;

    template <typename T>
    inline const auto constant_accelerate(float& velocity, T&& us) {
        const auto acc = iparams::get_f("cursor_acceleration") ;
        const auto mvc = iparams::get_f("cursor_max_velocity") ;

        constexpr auto TIME_COEF = util::pow_f(10, -3) ;
        const auto t = us * TIME_COEF / iparams::get_i("cursor_weight") ; //accuracy
        const auto x = velocity*t + 0.5f*acc*t*t ;
        const auto delta_v = acc * t ;
        if(velocity + delta_v < mvc) velocity += delta_v ;
        else velocity = mvc ;
        return x ;
    }

    inline const auto compute_delta_t(
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
        const auto delta() {
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
    : pimpl(std::make_unique<Impl>())
    {}

    MoveLeft::~MoveLeft()                     = default ;
    MoveLeft::MoveLeft(MoveLeft&&)            = default ;
    MoveLeft& MoveLeft::operator=(MoveLeft&&) = default ;

    const std::string MoveLeft::sname() noexcept {
        return "move_left" ;
    }
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
    : pimpl(std::make_unique<Impl>())
    {}

    MoveRight::~MoveRight()                      = default ;
    MoveRight::MoveRight(MoveRight&&)            = default ;
    MoveRight& MoveRight::operator=(MoveRight&&) = default ;

    const std::string MoveRight::sname() noexcept {
        return "move_right" ;
    }
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
    : pimpl(std::make_unique<Impl>())
    {}

    MoveUp::~MoveUp()                   = default ;
    MoveUp::MoveUp(MoveUp&&)            = default ;
    MoveUp& MoveUp::operator=(MoveUp&&) = default ;

    const std::string MoveUp::sname() noexcept {
        return "move_up" ;
    }
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
    : pimpl(std::make_unique<Impl>())
    {}

    MoveDown::~MoveDown()                     = default ;
    MoveDown::MoveDown(MoveDown&&)            = default ;
    MoveDown& MoveDown::operator=(MoveDown&&) = default ;

    const std::string MoveDown::sname() noexcept {
        return "move_down" ;
    }
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
