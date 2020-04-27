#include "move_cursor.hpp"
#include "interval_timer.hpp"
#include "default_config.hpp"

#include <windows.h>

#include <iostream>
#include <cmath>
using namespace std ;

using namespace std::chrono ;

namespace MoveUtility
{
    static constexpr auto INITIAL_VELOCITY = 1.0f ;

    template <typename T1, typename T2>
    inline static const auto const_accelerate(T1& velocity, const T2 us) noexcept {
        static constexpr auto TIME_COEF = static_cast<float>(pow(10, -3)) ;
        static constexpr auto MAX_VELOCITY = 10.0f ;

        const auto t = us * TIME_COEF / DefaultConfig::CURSOR_WEIGHT() ; //accuracy

        const auto x = velocity*t + 0.5f*DefaultConfig::MOVE_ACCELERATION()*t*t ;

        if(velocity < MAX_VELOCITY) {
            velocity += DefaultConfig::MOVE_ACCELERATION() * t ;
        }
        else {
            velocity = MAX_VELOCITY ;
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
            Logger::error_stream << "[Error] windows.h: " << GetLastError()  << " (MoveUtility::move_cursor)\n" ;
            return false ;
        }
        return true ;
    }

    inline static const auto compute_deltat(const system_clock::time_point& start_time) noexcept {
        return duration_cast<microseconds>(system_clock::now() - start_time).count() ;
    }
}

using namespace MoveUtility ;

//MoveLeft
struct MoveLeft::Impl
{
    float v = INITIAL_VELOCITY ;
    system_clock::time_point start_time = system_clock::now() ;
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
        pimpl->v = INITIAL_VELOCITY ;
        pimpl->start_time = system_clock::now() ;
    }
    const auto delta = static_cast<int>(const_accelerate(pimpl->v, compute_deltat(pimpl->start_time))) ;
    return is_move_cursor(-delta, 0) ;
}


//MoveRight
struct MoveRight::Impl
{
    float v = INITIAL_VELOCITY ;
    system_clock::time_point start_time = system_clock::now() ;
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
        pimpl->v = INITIAL_VELOCITY ;
        pimpl->start_time = system_clock::now() ;
    }
    const auto delta = static_cast<int>(const_accelerate(pimpl->v, compute_deltat(pimpl->start_time))) ;
    return is_move_cursor(delta, 0) ;
}


//MoveUp
struct MoveUp::Impl
{
    float v = INITIAL_VELOCITY ;
    system_clock::time_point start_time = system_clock::now() ;
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
        pimpl->v = INITIAL_VELOCITY ;
        pimpl->start_time = system_clock::now() ;
    }
    const auto delta = static_cast<int>(const_accelerate(pimpl->v, compute_deltat(pimpl->start_time))) ;
    return is_move_cursor(0, -delta) ;
}

//MoveDown
struct MoveDown::Impl
{
    float v = INITIAL_VELOCITY ;
    system_clock::time_point start_time = system_clock::now() ;
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
        pimpl->v = INITIAL_VELOCITY ;
        pimpl->start_time = system_clock::now() ;
    }
    const auto delta = static_cast<int>(const_accelerate(pimpl->v, compute_deltat(pimpl->start_time))) ;
    return is_move_cursor(0, delta) ;
}