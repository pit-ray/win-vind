#include "binded_func.hpp"

#include <array>

#include "msg_logger.hpp"

struct BindedFunc::Impl
{
    std::array<KeyMatcher::shp_t, static_cast<int>(ModeManager::Mode::NUM)> mtrs ;
    KeyMatcher::shp_t current_mtr ;

    explicit Impl()
    : mtrs(),
      current_mtr(nullptr)
    {
        mtrs.fill(nullptr) ;
    }
} ;

BindedFunc::BindedFunc()
: pimpl(std::make_unique<Impl>())
{}

BindedFunc::~BindedFunc() noexcept                          = default ;
BindedFunc::BindedFunc(BindedFunc&&) noexcept               = default ;
BindedFunc& BindedFunc::operator=(BindedFunc&&) noexcept    = default ;

void BindedFunc::process(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const
{
    if(!repeat_num) return ;
    if(!do_process(first_call, repeat_num, parent_logger)) {
        ERROR_PRINT("do_process is failed. (" + name() + ")") ;
    }
}

void BindedFunc::register_matcher(const ModeManager::Mode mode, const KeyMatcher::shp_t matcher) const noexcept
{
    if(!matcher) return ;
    pimpl->mtrs[static_cast<unsigned char>(mode)] = matcher ;
}

void BindedFunc::register_matcher(const unsigned char mode, const KeyMatcher::shp_t matcher) const
{
    if(!matcher) return ;
    pimpl->mtrs.at(mode) = matcher ;
}

unsigned int BindedFunc::get_matched_num(const KeyLogger& logger, ModeManager::Mode mode) const
{
    pimpl->current_mtr = pimpl->mtrs[static_cast<unsigned char>(mode)] ;
    if(!pimpl->current_mtr) return 0 ;
    return pimpl->current_mtr->compare2latestlog(logger) ;
}

bool BindedFunc::is_callable() const noexcept
{
    if(!pimpl->current_mtr) return false ;
    return pimpl->current_mtr->is_callable() ;
}

bool BindedFunc::is_for_moving_caret() const noexcept
{
    return false ;
}
