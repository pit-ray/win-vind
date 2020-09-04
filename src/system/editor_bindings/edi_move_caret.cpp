#include "edi_move_caret.hpp"

#include <chrono>
#include <random>

#include "keybrd_eventer.hpp"
#include "interval_timer.hpp"
#include "msg_logger.hpp"
#include "key_absorber.hpp"
#include "simpl_text_selecter.hpp"
#include "mode_manager.hpp"

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
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            return KeybrdEventer::pressup(VKC_LSHIFT, VKC_LEFT) ;
        }
        return KeybrdEventer::pressup(VKC_LEFT) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return press() ;
    }

    if(!pimpl->calcer.is_moveable()) {
        return true ;
    }

    return press() ;
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
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            return KeybrdEventer::pressup(VKC_LSHIFT, VKC_RIGHT) ;
        }
        return KeybrdEventer::pressup(VKC_RIGHT) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return press() ;
    }

    if(!pimpl->calcer.is_moveable()) {
        return true ;
    }
    return press() ;
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
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            if(SimplTextSelecter::is_first_line_selection()) {
                if(!SimplTextSelecter::select_line_EOL2BOL()) {
                    return false ;
                }
            }

            if(!KeybrdEventer::pressup(VKC_LSHIFT, VKC_UP)) {
                return false ;
            }
            return SimplTextSelecter::moving_update() ;
        }
        return KeybrdEventer::pressup(VKC_UP) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return press() ;
    }
    if(!pimpl->calcer.is_moveable()) {
        return true ;
    }
    return press() ;
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
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            if(SimplTextSelecter::is_first_line_selection()) {
                if(!SimplTextSelecter::select_line_BOL2EOL()) {
                    return false ;
                }
            }

            if(!KeybrdEventer::pressup(VKC_LSHIFT, VKC_DOWN)) {
                return false ;
            }
            return SimplTextSelecter::moving_update() ;
        }
        return KeybrdEventer::pressup(VKC_DOWN) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return press() ;
    }
    if(!pimpl->calcer.is_moveable()) {
        return true ;
    }
    return press() ;
}


//EdiMoveCaretNwordsForward
const string EdiNMoveCaretwordsForward::sname() noexcept
{
    return "edi_n_move_caret_words_forward" ;
}

bool EdiNMoveCaretwordsForward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(ModeManager::is_edi_visual()) {
        return KeybrdEventer::pressup(VKC_LSHIFT, VKC_LCTRL, VKC_RIGHT) ;
    }
    return KeybrdEventer::pressup(VKC_LCTRL, VKC_RIGHT) ;
}


//EdiMoveCaretNwordsBackward
const string EdiNMoveCaretwordsBackward::sname() noexcept
{
    return "edi_n_move_caret_words_backward" ;
}

bool EdiNMoveCaretwordsBackward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(ModeManager::is_edi_visual()) {
        return KeybrdEventer::pressup(VKC_LSHIFT, VKC_LCTRL, VKC_LEFT) ;
    }
    return KeybrdEventer::pressup(VKC_LCTRL, VKC_LEFT) ;
}


//EdiMoveCaretNWORDSForward
const string EdiNMoveCaretWORDSForward::sname() noexcept
{
    return "edi_n_move_caret_WORDS_forward" ;
}

bool EdiNMoveCaretWORDSForward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(ModeManager::is_edi_visual()) {
        return KeybrdEventer::pressup(VKC_LSHIFT, VKC_LCTRL, VKC_RIGHT) ;
    }
    return KeybrdEventer::pressup(VKC_LCTRL, VKC_RIGHT) ;
}


//EdiMoveCaretNWORDSBackward
const string EdiNMoveCaretWORDSBackward::sname() noexcept
{
    return "edi_n_move_caret_WORDS_backward" ;
}

bool EdiNMoveCaretWORDSBackward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(ModeManager::is_edi_visual()) {
        return KeybrdEventer::pressup(VKC_LSHIFT, VKC_LCTRL, VKC_LEFT) ;
    }
    return KeybrdEventer::pressup(VKC_LCTRL, VKC_LEFT) ;
}