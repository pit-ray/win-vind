#include "bind/base/char_logger.hpp"

#include "bind/base/key_logger_base.hpp"
#include "key/key_absorber.hpp"
#include "key/keycodecvt.hpp"
#include "time/keystroke_repeater.hpp"

namespace 
{
    using namespace vind ;
    bool is_including_ascii(const KeyLog& log) {
        for(auto itr = log.cbegin() ; itr != log.cend() ; itr ++) {
            if(keycodecvt::get_ascii(*itr)) {
                return true ;
            }
        }
        return false ;
    }
}

namespace vind
{
    struct CharLogger::Impl {
        KeyLog prelog_{} ;
        KeyStrokeRepeater ksr_{} ; 
        std::set<unsigned char> non_chars_{} ;

        explicit Impl()
        : prelog_(),
          ksr_(),
          non_chars_()
        {}
        explicit Impl(const std::initializer_list<unsigned char>& non_chars)
        : prelog_(),
          ksr_(),
          non_chars_(non_chars)
        {}
        explicit Impl(std::initializer_list<unsigned char>&& non_chars)
        : prelog_(),
          ksr_(),
          non_chars_(std::move(non_chars))
        {}
        explicit Impl(const std::set<unsigned char>& non_chars)
        : prelog_(),
          ksr_(),
          non_chars_(non_chars)
        {}
        explicit Impl(std::set<unsigned char>&& non_chars)
        : prelog_(),
          ksr_(),
          non_chars_(std::move(non_chars))
        {}

        bool is_including_enabled_chars(const KeyLog& log) {
            for(auto itr = non_chars_.cbegin() ; itr != non_chars_.cend() ; itr ++) {
                if(log.is_containing(*itr)) {
                    return true ;
                }
            }
            return false ;
        }
    } ;

    CharLogger::CharLogger()
    : KeyLoggerBase(),
      pimpl(std::make_unique<Impl>())
    {}
    CharLogger::CharLogger(const std::initializer_list<unsigned char>& enabled_non_chars)
    : KeyLoggerBase(),
      pimpl(std::make_unique<Impl>(enabled_non_chars))
    {}
    CharLogger::CharLogger(std::initializer_list<unsigned char>&& enabled_non_chars)
    : KeyLoggerBase(),
      pimpl(std::make_unique<Impl>(std::move(enabled_non_chars)))
    {}
    CharLogger::CharLogger(const std::set<unsigned char>& enabled_non_chars)
    : KeyLoggerBase(),
      pimpl(std::make_unique<Impl>(enabled_non_chars))
    {}
    CharLogger::CharLogger(std::set<unsigned char>&& enabled_non_chars)
    : KeyLoggerBase(),
      pimpl(std::make_unique<Impl>(std::move(enabled_non_chars)))
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

    void CharLogger::enable_non_character(unsigned char keycode) {
        pimpl->non_chars_.insert(keycode) ;
    }
    void CharLogger::disable_non_character(unsigned char keycode) {
        pimpl->non_chars_.erase(keycode) ;
    }

    int CharLogger::logging_state() {
        static const KeyLog cl_toggles(keycodecvt::get_toggle_keys()) ;

        //ignore all toggle keys
        auto log = keyabsorber::get_pressed_list() - cl_toggles ;

        if(log != pimpl->prelog_) { //type is changed
            const auto diff = log - pimpl->prelog_ ;
            pimpl->prelog_ = log ;

            if(!pimpl->is_including_enabled_chars(log) && !is_including_ascii(diff)) {
                return 0 ;
            }

            if(log.is_containing(KEYCODE_SHIFT)) { //shfited
                auto data = diff.get() ;
                data.insert(KEYCODE_SHIFT) ;

                //construct KeyLog inside logs directly from std::vector
                logging(std::move(data)) ;
            }
            else {
                logging(std::move(diff)) ;
            }

            pimpl->ksr_.reset() ;
            return static_cast<int>(latest().size()) ;
        }
        else { //long pressing
            if(!pimpl->is_including_enabled_chars(log) && !is_including_ascii(log)) {
                return 0 ;
            }

            //emulate key stroke
            if(pimpl->ksr_.is_pressed()) {
                logging(std::move(log)) ;
                return static_cast<int>(latest().size()) ;
            }
            else {
                return 0 ;
            }
        }
    }

    const std::string CharLogger::to_str() const {
        if(empty()) return "" ;

        std::string str{} ;
        for(auto itr = cbegin() ; itr != cend() ; itr ++) {
            if(itr->is_containing(KEYCODE_SHIFT)) {
                //shifted ascii
                for(const auto keycode : *itr) {
                    const auto c = keycodecvt::get_shifted_ascii(keycode) ;
                    if(c != 0) str.push_back(c) ;
                }
                continue ;
            }
            for(const auto keycode : *itr) {
                const auto c = keycodecvt::get_ascii(keycode) ;
                if(c != 0) str.push_back(c) ;
            }
        }
        return str ;
    }
}
