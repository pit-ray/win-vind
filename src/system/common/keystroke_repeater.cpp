#include "keystroke_repeater.hpp"

#include <chrono>
#include <random>

#include "interval_timer.hpp"

using namespace std::chrono ;

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

struct KeyStrokeRepeater::Impl {
    IntervalTimer timer{REPEAT_SAMPLING_DELTA_US} ;
    float v = INITIAL_VELOCITY ;
    system_clock::time_point start_time{system_clock::now()} ;
} ;

KeyStrokeRepeater::KeyStrokeRepeater()
: pimpl(std::make_unique<Impl>())
{}

KeyStrokeRepeater::~KeyStrokeRepeater() noexcept                                   = default ;
KeyStrokeRepeater::KeyStrokeRepeater(KeyStrokeRepeater&&) noexcept                 = default ;
KeyStrokeRepeater& KeyStrokeRepeater::operator=(KeyStrokeRepeater&&) noexcept      = default ;

void KeyStrokeRepeater::reset() noexcept {
    pimpl->v = INITIAL_VELOCITY ;
    pimpl->start_time = system_clock::now() ;
}

bool KeyStrokeRepeater::is_pressed() noexcept {
    const auto dt = compute_deltat(pimpl->start_time) ;

    if(dt < WAIT_TIME_FOR_STARTING_REPEAT_KEYSTROKE) {
        return false ;
    }
    if(!pimpl->timer.is_passed()) { //sampling
        return false ;
    }

    const auto rate = const_accelerate(pimpl->v, static_cast<float>(dt.count())) ; //speed
    if(rate < generate_uniform()) {
        return false ;
    }

    return true ;
}