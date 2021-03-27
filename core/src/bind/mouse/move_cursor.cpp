#include "move_cursor.hpp"

#include <windows.h>

#include <chrono>
#include <iostream>
#include <cmath>

#include "i_params.hpp"
#include "err_logger.hpp"
#include "util/def.hpp"
#include "util/math.hpp"

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

    inline void _move_cursor(const int dx, const int dy) {
        static INPUT in = {INPUT_MOUSE, {.mi = {0, 0, 0, MOUSEEVENTF_MOVE, 0, 0}}} ;

        in.mi.dx = dx ;
        in.mi.dy = dy ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            throw RUNTIME_EXCEPT("cannot send the moving event of the mouse") ;
        }
    }

    inline const auto _compute_deltat(
            const system_clock::time_point& start_time) {

        return duration_cast<microseconds>(
                system_clock::now() - start_time).count() ;
    }

    class MoveDeltaCalculator {
    private:
        float v = INITIAL_VELOCITY ;
        system_clock::time_point start_time = system_clock::now() ;

    public:
        void reset() noexcept {
            v = INITIAL_VELOCITY ;
            start_time = system_clock::now() ;
        }

        const auto delta() {
            return static_cast<int>(
                    constant_accelerate(v, _compute_deltat(start_time))) ;
        }
    } ;
}

namespace vind
{
    //MoveLeft
    struct MoveLeft::Impl {
        MoveDeltaCalculator calcer{} ;
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

    void MoveLeft::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) pimpl->calcer.reset() ;
        _move_cursor(-pimpl->calcer.delta() * repeat_num, 0) ;
    }


    //MoveRight
    struct MoveRight::Impl {
        MoveDeltaCalculator calcer{} ;
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

    void MoveRight::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) pimpl->calcer.reset() ;
        _move_cursor(pimpl->calcer.delta() * repeat_num, 0) ;
    }


    //MoveUp
    struct MoveUp::Impl {
        MoveDeltaCalculator calcer{} ;
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
    void MoveUp::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) pimpl->calcer.reset() ;
        _move_cursor(0, -pimpl->calcer.delta() * repeat_num) ;
    }

    //MoveDown
    struct MoveDown::Impl {
        MoveDeltaCalculator calcer{} ;
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
    void MoveDown::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) pimpl->calcer.reset() ;
        _move_cursor(0, pimpl->calcer.delta() * repeat_num) ;
    }
}
