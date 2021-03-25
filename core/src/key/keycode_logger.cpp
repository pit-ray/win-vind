#include "keycode_logger.hpp"

#include "key_absorber.hpp"
#include "key_logger_base.hpp"
#include "err_logger.hpp"
#include "keycodecvt.hpp"

namespace vind
{
    struct KeycodeLogger::Impl {
        KeyLog prelog{} ;
        bool vkc_changed = false ;
    } ;

    KeycodeLogger::KeycodeLogger()
    : KeyLoggerBase(),
      pimpl(std::make_unique<Impl>())
    {}

    KeycodeLogger::~KeycodeLogger() noexcept = default ;

    KeycodeLogger::KeycodeLogger(const KeycodeLogger& rhs)
    : KeyLoggerBase(rhs),
      pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
    {}
    KeycodeLogger& KeycodeLogger::operator=(const KeycodeLogger& rhs) {
        if(rhs.pimpl) {
            KeyLoggerBase::operator=(rhs) ;
            *pimpl = *(rhs.pimpl) ;
        }
        return *this ;
    }

    KeycodeLogger::KeycodeLogger(KeycodeLogger&&)            = default ;
    KeycodeLogger& KeycodeLogger::operator=(KeycodeLogger&&) = default ;

    void KeycodeLogger::update() {
        static const KeyLog cl_toggles(keycodecvt::get_toggle_keys()) ;

        auto log = keyabsorber::get_pressed_list() - cl_toggles ;
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

    bool KeycodeLogger::is_changed() const noexcept {
        return pimpl->vkc_changed ;
    }
}
