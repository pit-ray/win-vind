#include "char_logger.hpp"

#include "key_absorber.hpp"
#include "key_logger_base.hpp"
#include "keycodecvt.hpp"
#include "time/keystroke_repeater.hpp"

namespace
{
    enum FlagBits : unsigned char
    {
        ALL_FLASE         = 0x00,
        CHAR_CHANGED      = 0x01,
        KEYSTROKE_CHANGED = 0x10,
    } ;
}

namespace vind
{
    struct CharLogger::Impl {
        KeyLog prelog{} ;
        unsigned char flags = FlagBits::KEYSTROKE_CHANGED ;
        KeyStrokeRepeater ksr{} ; 
    } ;

    CharLogger::CharLogger()
    : KeyLoggerBase(),
      pimpl(std::make_unique<Impl>())
    {}

    CharLogger::~CharLogger() noexcept = default ;

    CharLogger::CharLogger(const CharLogger& rhs)
    : KeyLoggerBase(rhs),
      pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
    {}

    CharLogger& CharLogger::operator=(const CharLogger& rhs) {
        if(rhs.pimpl) {
            KeyLoggerBase::operator=(rhs) ;
            *pimpl = *(rhs.pimpl) ;
        }
        return *this ;
    }

    CharLogger::CharLogger(CharLogger&&)            = default ;
    CharLogger& CharLogger::operator=(CharLogger&&) = default ;

    void CharLogger::update() {
        static const KeyLog cl_toggles(keycodecvt::get_toggle_keys()) ;

        //ignore all toggle keys
        auto log = keyabsorber::get_pressed_list() - cl_toggles ;

        if(log != pimpl->prelog) { //type is changed
            const auto diff = log - pimpl->prelog ;
            pimpl->prelog = log ;

            if(log.is_containing(KEYCODE_SHIFT)) { //shfited
                auto data = diff.get() ;
                data.insert(KEYCODE_SHIFT) ;

                //construct KeyLog inside logs directly from std::vector
                logging(std::move(data)) ;
            }
            else {
                logging(std::move(diff)) ;
            }

            //the key stroke is new and logger is changed
            pimpl->flags = FlagBits::KEYSTROKE_CHANGED | FlagBits::CHAR_CHANGED ;
        }
        else { //long pressing
            logging(log) ;

            if(log.empty()) {
                pimpl->flags = FlagBits::ALL_FLASE ;
                return ;
            }

            if(pimpl->flags & FlagBits::KEYSTROKE_CHANGED) {
                pimpl->flags = FlagBits::ALL_FLASE ;
                pimpl->ksr.reset() ;
                return ;
            }

            //emulate key stroke
            if(pimpl->ksr.is_pressed())
                pimpl->flags = FlagBits::CHAR_CHANGED ;
            else
                pimpl->flags = FlagBits::ALL_FLASE ;
        }
    }

    bool CharLogger::is_changed() const noexcept {
        return pimpl->flags & FlagBits::CHAR_CHANGED ;
    }

    const std::string CharLogger::to_str() const {
        if(empty()) return "" ;

        std::string str{} ;
        for(auto itr = cbegin() ; itr != cend() ; itr ++) {
            if(itr->is_containing(KEYCODE_SHIFT)) {
                //shifted ascii
                for(const auto vkc : *itr) {
                    const auto c = keycodecvt::get_shifted_ascii(vkc) ;
                    if(c != 0) str.push_back(c) ;
                }
                continue ;
            }
            for(const auto vkc : *itr) {
                const auto c = keycodecvt::get_ascii(vkc) ;
                if(c != 0) str.push_back(c) ;
            }
        }
        return str ;
    }
}
