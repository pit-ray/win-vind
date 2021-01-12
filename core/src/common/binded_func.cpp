#include "binded_func.hpp"

#include <array>
#include <atomic>

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "key_absorber.hpp"
#include "vkc_converter.hpp"

struct BindedFunc::Impl
{
    std::array<KeyMatcher::shp_t, static_cast<int>(ModeManager::Mode::NUM)> mtrs ;
    unsigned char modeidx ;
    std::atomic_bool running_now ;

    explicit Impl()
    : mtrs(),
      modeidx(0),
      running_now(false)
    {
        mtrs.fill(nullptr) ;
    }
} ;

BindedFunc::BindedFunc()
: pimpl(std::make_unique<Impl>())
{}

BindedFunc::~BindedFunc() noexcept              = default ;
BindedFunc::BindedFunc(BindedFunc&&)            = default ;
BindedFunc& BindedFunc::operator=(BindedFunc&&) = default ;

void BindedFunc::process(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* parent_vkclgr,
        const KeyLogger* const parent_charlgr) const
{
    if(repeat_num == 0) return ;

    pimpl->running_now.store(true) ;
    try {
        do_process(first_call, repeat_num, parent_vkclgr, parent_charlgr) ;

        //correct the state
        //to avoid cases that a virtual key is judged to be pressed,
        //though a real key is released.
        for(auto& key : KeyAbsorber::get_pressed_list()) {
            if(!KeyAbsorber::is_really_pressed(key)) {
                KeyAbsorber::release_virtually(key) ;
            }
        }
    }
    catch(const std::runtime_error& e) {
        ERROR_PRINT(name() + " failed. " + e.what()) ;
        try {
            const auto buf = KeyAbsorber::get_pressed_list() ;
            if(!buf.empty()) {
                if(KeyAbsorber::is_absorbed()) {
                    KeyAbsorber::open_some_ports(buf.get()) ;
                }
                for(auto& key : buf) {
                    KeybrdEventer::release_keystate(key) ;
                }
                if(KeyAbsorber::is_absorbed()) {
                    KeyAbsorber::close_all_ports() ;
                    KeyAbsorber::absorb() ;
                }
                else {
                    KeyAbsorber::open_all_ports() ;
                    KeyAbsorber::unabsorb() ;
                }
            }
        }
        catch(const std::runtime_error& e2) {
            ERROR_PRINT(name()
                    + " failed. Cannot refresh all key state. "
                    + e2.what()) ;
        }
    }
    pimpl->running_now.store(false) ;
}

void BindedFunc::register_matcher(
        const ModeManager::Mode mode,
        const KeyMatcher::shp_t matcher) const
{
    if(!matcher) return ;
    pimpl->mtrs.at(static_cast<unsigned char>(mode)) = matcher ;
}

void BindedFunc::register_matcher(
        const unsigned char mode,
        const KeyMatcher::shp_t matcher) const
{
    if(!matcher) return ;
    pimpl->mtrs.at(mode) = matcher ;
}

unsigned int BindedFunc::validate_if_match(
        const KeyLogger& logger,
        ModeManager::Mode mode) const
{
    if(pimpl->running_now.load()) return 0 ;

    pimpl->modeidx = static_cast<unsigned char>(mode) ;
    if(auto& ptr = pimpl->mtrs.at(pimpl->modeidx))
        return ptr->compare_to_latestlog(logger) ;

    return 0 ;
}

unsigned int BindedFunc::validate_if_fullmatch(
        const KeyLogger& logger,
        ModeManager::Mode mode) const
{
    if(pimpl->running_now.load()) return 0 ;

    pimpl->modeidx = static_cast<unsigned char>(mode) ;
    if(auto& ptr = pimpl->mtrs.at(pimpl->modeidx))
        return ptr->compare_to_alllog(logger) ;

    return 0 ;
}

bool BindedFunc::is_callable() const noexcept
{
    if(pimpl->running_now.load()) return false ;
    if(auto& ptr = pimpl->mtrs.at(pimpl->modeidx))
        return ptr->is_accepted() ;
    return false ;
}

bool BindedFunc::is_for_moving_caret() const noexcept
{
    return false ;
}
