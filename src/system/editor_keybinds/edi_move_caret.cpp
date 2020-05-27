#include "edi_move_caret.hpp"

#include <chrono>
#include <random>

#include "keybrd_eventer.hpp"
#include "interval_timer.hpp"
#include "msg_logger.hpp"
#include "dynamic_config.hpp"
#include "key_absorber.hpp"
#include "text_selecter.hpp"

using namespace std ;
using namespace std::chrono ;


namespace EdiMoveCaretUtility
{

    inline static const auto const_accelerate(float& velocity, const float ms) noexcept {
        //hardcoded
        static constexpr auto MAX_ACCELERATION = 1.0f ;
        static constexpr auto MAX_VELOCITY = 1.0f ;

        //ms -> s
        const auto t = ms / 1000 ;

        const auto delta_v = MAX_ACCELERATION * t ;
        if(velocity + delta_v < MAX_VELOCITY) {
            velocity += delta_v ;
        }
        else {
            velocity = MAX_VELOCITY ;
        }

        return velocity ;
    }

    inline static const auto compute_deltat(const system_clock::time_point& start_time) noexcept {
        return duration_cast<milliseconds>(system_clock::now() - start_time) ;
    }

    inline static const auto generate_uniform() noexcept {
        static std::random_device seed_gen ;
        static std::default_random_engine engine(seed_gen()) ;
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f) ;

        return dist(engine) ;
    }

    //hardcoded
    static constexpr auto WAIT_TIME_FOR_STARTING_REPEAT_KEYSTROKE = 512ms;
    static constexpr auto REPEAT_SAMPLING_DELTA_US = 25'600 ;
    static constexpr auto INITIAL_VELOCITY = 0.000'1f ;

    class MoveableCalculator {
    private:
        IntervalTimer timer{REPEAT_SAMPLING_DELTA_US} ;
        float v = INITIAL_VELOCITY ;
        system_clock::time_point start_time{system_clock::now()} ;

    public:
        void reset() noexcept {
            v = INITIAL_VELOCITY ;
            start_time = system_clock::now() ;
        }

        auto is_moveable() noexcept {
            const auto dt = compute_deltat(start_time) ;

            if(dt < WAIT_TIME_FOR_STARTING_REPEAT_KEYSTROKE) {
                return false ;
            }
            if(!timer.is_passed()) { //sampling
                return false ;
            }

            const auto rate = const_accelerate(v, static_cast<float>(dt.count())) ; //speed
            if(rate < generate_uniform()) {
                return false ;
            }

            return true ;
        }
    } ;
}

using namespace EdiMoveCaretUtility ;

//EdiMoveCaretLeft
struct EdiMoveCaretLeft::Impl
{
    MoveableCalculator calcer{} ;
} ;

EdiMoveCaretLeft::EdiMoveCaretLeft()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretLeft::~EdiMoveCaretLeft() = default ;

EdiMoveCaretLeft::EdiMoveCaretLeft(EdiMoveCaretLeft&&) = default ;
EdiMoveCaretLeft& EdiMoveCaretLeft::operator=(EdiMoveCaretLeft&&) = default ;

const string EdiMoveCaretLeft::sname() noexcept
{
    return "edi_move_caret_left" ;
}

bool EdiMoveCaretLeft::sprocess(const bool first_call) const
{
    auto push = [] {
        return KeybrdEventer::is_pushup(VKC_LEFT) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return push() ;
    }

    if(!pimpl->calcer.is_moveable()) {
        return true ;
    }

    return push() ;
}


//EdiMoveCaretRight
struct EdiMoveCaretRight::Impl
{
    MoveableCalculator calcer{} ;
} ;

EdiMoveCaretRight::EdiMoveCaretRight()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretRight::~EdiMoveCaretRight() = default ;

EdiMoveCaretRight::EdiMoveCaretRight(EdiMoveCaretRight&&) = default ;
EdiMoveCaretRight& EdiMoveCaretRight::operator=(EdiMoveCaretRight&&) = default ;

const string EdiMoveCaretRight::sname() noexcept
{
    return "edi_move_caret_right" ;
}

bool EdiMoveCaretRight::sprocess(const bool first_call) const
{
    auto push = [] {
        return KeybrdEventer::is_pushup(VKC_RIGHT) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return push() ;
    }

    if(!pimpl->calcer.is_moveable()) {
        return true ;
    }

    return push() ;
}



//EdiMoveCaretUp
struct EdiMoveCaretUp::Impl
{
    MoveableCalculator calcer{} ;
} ;

EdiMoveCaretUp::EdiMoveCaretUp()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretUp::~EdiMoveCaretUp() = default ;

EdiMoveCaretUp::EdiMoveCaretUp(EdiMoveCaretUp&&) = default ;
EdiMoveCaretUp& EdiMoveCaretUp::operator=(EdiMoveCaretUp&&) = default ;

const string EdiMoveCaretUp::sname() noexcept
{
    return "edi_move_caret_up" ;
}

bool EdiMoveCaretUp::sprocess(const bool first_call) const
{
    auto push = [] {
        return KeybrdEventer::is_pushup(VKC_UP) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return push() ;
    }

    if(!pimpl->calcer.is_moveable()) {
        return true ;
    }

    return push() ;
}



//EdiMoveCaretDown
struct EdiMoveCaretDown::Impl
{
    MoveableCalculator calcer{} ;
} ;

EdiMoveCaretDown::EdiMoveCaretDown()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretDown::~EdiMoveCaretDown() = default ;

EdiMoveCaretDown::EdiMoveCaretDown(EdiMoveCaretDown&&) = default ;
EdiMoveCaretDown& EdiMoveCaretDown::operator=(EdiMoveCaretDown&&) = default ;

const string EdiMoveCaretDown::sname() noexcept
{
    return "edi_move_caret_down" ;
}

bool EdiMoveCaretDown::sprocess(const bool first_call) const
{
    auto push = [] {
        return KeybrdEventer::is_pushup(VKC_DOWN) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return push() ;
    }

    if(!pimpl->calcer.is_moveable()) {
        return true ;
    }

    return push() ;
}


//EdiMoveCaretNwordsForward
const string EdiMoveCaretNwordsForward::sname() noexcept
{
    return "edi_move_caret_Nwords_forward" ;
}

bool EdiMoveCaretNwordsForward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return KeybrdEventer::is_pushup(VKC_LCTRL, VKC_RIGHT) ;
}


//EdiMoveCaretNwordsBackward
const string EdiMoveCaretNwordsBackward::sname() noexcept
{
    return "edi_move_caret_Nwords_backward" ;
}

bool EdiMoveCaretNwordsBackward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return KeybrdEventer::is_pushup(VKC_LCTRL, VKC_LEFT) ;
}


//EdiMoveCaretNWORDSForward
const string EdiMoveCaretNWORDSForward::sname() noexcept
{
    return "edi_move_caret_NWORDS_forward" ;
}

bool EdiMoveCaretNWORDSForward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return KeybrdEventer::is_pushup(VKC_LCTRL, VKC_RIGHT) ;
}


//EdiMoveCaretNWORDSBackward
const string EdiMoveCaretNWORDSBackward::sname() noexcept
{
    return "edi_move_caret_NWORDS_backward" ;
}

bool EdiMoveCaretNWORDSBackward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return KeybrdEventer::is_pushup(VKC_LCTRL, VKC_LEFT) ;
}