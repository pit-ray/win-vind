#include "vkc_logger.hpp"

#include "key_absorber.hpp"
#include "key_logger_base.hpp"
#include "msg_logger.hpp"
#include "vkc_converter.hpp"

struct VKCLogger::Impl
{
    KeyLog prelog{} ;
    bool vkc_changed = false ;
} ;

VKCLogger::VKCLogger()
: KeyLoggerBase(),
  pimpl(std::make_unique<Impl>())
{}

VKCLogger::~VKCLogger() noexcept = default ;

VKCLogger::VKCLogger(const VKCLogger& rhs)
: KeyLoggerBase(rhs),
  pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
{}
VKCLogger& VKCLogger::operator=(const VKCLogger& rhs)
{
    if(rhs.pimpl) {
        KeyLoggerBase::operator=(rhs) ;
        *pimpl = *(rhs.pimpl) ;
    }
    return *this ;
}

VKCLogger::VKCLogger(VKCLogger&&)            = default ;
VKCLogger& VKCLogger::operator=(VKCLogger&&) = default ;

void VKCLogger::update()
{
    static const KeyLog cl_toggles(VKCConverter::get_toggle_keys()) ;

    auto log = KeyAbsorber::get_pressed_list() - cl_toggles ;
    logging(log) ;

    if(pimpl->prelog == log) {
        pimpl->vkc_changed = false ;
        return ;
    }

    if(size() == 1) {
        pimpl->prelog = log ;
        pimpl->vkc_changed = true ;
        return ;
    }

    auto diff = log - pimpl->prelog ; //remove same keys as prelog
    pimpl->prelog = log ;
    pimpl->vkc_changed = !diff.empty() ;
    //
    // If the number of logs in logger is bigger than 1,
    // some functions are matched, so save only differences.
    //
    // Ex)
    //  _______________________________________________________
    // |              |          |        |          |         |
    // | logger-index |   [1]    |  [2]   |    [3]   |   [4]   |
    // |--------------|----------|--------|----------|---------|
    // |     log      | Ctrl + w |  Ctrl  |          |    L    |
    // |--------------|----------|--------|----------|---------|
    // |    return    |   true   | false  |   false  |   true  |
    // |______________|__________|________|__________|_________|
    //
}

bool VKCLogger::is_changed() const noexcept
{
    return pimpl->vkc_changed ;
}
