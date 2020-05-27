#include "move_cursor.hpp"
#include "dynamic_config.hpp"

#include <windows.h>

#include <chrono>
#include <iostream>
#include <cmath>
using namespace std ;

using namespace std::chrono ;

namespace MoveUtility
{
    //hardcoded (The cursor move 1px necessarilly by one push)
    static constexpr auto INITIAL_VELOCITY = 1.0f ;

    template <typename T>
    inline static const auto const_accelerate(float& velocity, T&& us) noexcept {
        static constexpr auto TIME_COEF = static_cast<float>(pow(10, -3)) ;

        const auto t = us * TIME_COEF / DynamicConfig::CURSOR_WEIGHT() ; //accuracy

        const auto x = velocity*t + 0.5f*DynamicConfig::CURSOR_ACCELERATION()*t*t ;

        const auto delta_v = DynamicConfig::CURSOR_ACCELERATION() * t ;
        if(velocity + delta_v < DynamicConfig::CURSOR_MAX_VELOCITY()) {
            velocity += delta_v ;
        }
        else {
            velocity = DynamicConfig::CURSOR_MAX_VELOCITY() ;
        }
        return x ;
    }

    inline static INPUT& _initialize_cursor() noexcept {
        static INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ;
        in.mi.dy = 0 ;
        in.mi.mouseData = 0 ;
        in.mi.dwFlags = MOUSEEVENTF_MOVE ;
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = 0 ;

        return in ;
    }

    inline static bool is_move_cursor(const int dx, const int dy) noexcept {
        static INPUT in = _initialize_cursor() ;

        in.mi.dx = dx ;
        in.mi.dy = dy ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            WIN_ERROR_STREAM << "(MoveUtility::move_cursor)\n" ;
            return false ;
        }
        return true ;
    }

    inline static const auto compute_deltat(const system_clock::time_point& start_time) noexcept {
        return duration_cast<microseconds>(system_clock::now() - start_time).count() ;
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

        const auto delta() noexcept {
            return static_cast<int>(const_accelerate(v, compute_deltat(start_time))) ;
        }
    } ;
}

using namespace MoveUtility ;

//MoveLeft
struct MoveLeft::Impl
{
    MoveDeltaCalculator calcer{} ;
} ;

MoveLeft::MoveLeft()
: pimpl(make_unique<Impl>())
{}

MoveLeft::~MoveLeft() = default ;

MoveLeft::MoveLeft(MoveLeft&&) = default ;
MoveLeft& MoveLeft::operator=(MoveLeft&&) = default ;

const std::string MoveLeft::sname() noexcept
{
    return "move_left" ;
}

bool MoveLeft::sprocess(const bool first_call) const
{
    if(first_call) {
        pimpl->calcer.reset() ;
    }

    return is_move_cursor(-pimpl->calcer.delta(), 0) ;
}


//MoveRight
struct MoveRight::Impl
{
    MoveDeltaCalculator calcer{} ;
} ;

MoveRight::MoveRight()
: pimpl(make_unique<Impl>())
{}

MoveRight::~MoveRight() = default ;

MoveRight::MoveRight(MoveRight&&) = default ;
MoveRight& MoveRight::operator=(MoveRight&&) = default ;

const std::string MoveRight::sname() noexcept
{
    return "move_right" ;
}

bool MoveRight::sprocess(const bool first_call) const
{
    if(first_call) {
        pimpl->calcer.reset() ;
    }

    return is_move_cursor(pimpl->calcer.delta(), 0) ;
}


//MoveUp
struct MoveUp::Impl
{
    MoveDeltaCalculator calcer{} ;
} ;

MoveUp::MoveUp()
: pimpl(make_unique<Impl>())
{}

MoveUp::~MoveUp() = default ;

MoveUp::MoveUp(MoveUp&&) = default ;
MoveUp& MoveUp::operator=(MoveUp&&) = default ;

const std::string MoveUp::sname() noexcept
{
    return "move_up" ;
}

bool MoveUp::sprocess(const bool first_call) const
{
    if(first_call) {
        pimpl->calcer.reset() ;
    }

    return is_move_cursor(0, -pimpl->calcer.delta()) ;
}

//MoveDown
struct MoveDown::Impl
{
    MoveDeltaCalculator calcer{} ;
} ;

MoveDown::MoveDown()
: pimpl(make_unique<Impl>())
{}

MoveDown::~MoveDown() = default ;

MoveDown::MoveDown(MoveDown&&) = default ;
MoveDown& MoveDown::operator=(MoveDown&&) = default ;

const std::string MoveDown::sname() noexcept
{
    return "move_down" ;
}

bool MoveDown::sprocess(const bool first_call) const
{
    if(first_call) {
        pimpl->calcer.reset() ;
    }
    return is_move_cursor(0, pimpl->calcer.delta()) ;
}