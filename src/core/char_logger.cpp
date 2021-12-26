#include "char_logger.hpp"

#include "core/inputgate.hpp"
#include "key_logger_base.hpp"
#include "keycode.hpp"
#include "keycode_def.hpp"
#include "util/debug.hpp"
#include "util/keystroke_repeater.hpp"


namespace 
{
    using namespace vind ;
    bool is_including_ascii(const core::KeyLog& log) {
        for(const auto& keycode : log) {
            if(keycode.is_ascii()) {
                return true ;
            }
        }
        return false ;
    }
}

namespace vind
{
    namespace core
    {
        struct CharLogger::Impl {
            KeyLog prelog_{} ;
            util::KeyStrokeRepeater ksr_{} ; 
            std::set<KeyCode> non_chars_{} ;

            explicit Impl()
            : prelog_(),
              ksr_(),
              non_chars_()
            {}
            explicit Impl(const std::initializer_list<KeyCode>& non_chars)
            : prelog_(),
              ksr_(),
              non_chars_(non_chars)
            {}
            explicit Impl(std::initializer_list<KeyCode>&& non_chars)
            : prelog_(),
              ksr_(),
              non_chars_(std::move(non_chars))
            {}
            explicit Impl(const std::set<KeyCode>& non_chars)
            : prelog_(),
              ksr_(),
              non_chars_(non_chars)
            {}
            explicit Impl(std::set<KeyCode>&& non_chars)
            : prelog_(),
              ksr_(),
              non_chars_(std::move(non_chars))
            {}

            bool is_including_enabled_chars(const KeyLog& log) {
                for(const auto& keycode : non_chars_) {
                    if(log.is_containing(keycode)) {
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
        CharLogger::CharLogger(const std::initializer_list<KeyCode>& enabled_non_chars)
        : KeyLoggerBase(),
          pimpl(std::make_unique<Impl>(enabled_non_chars))
        {}
        CharLogger::CharLogger(std::initializer_list<KeyCode>&& enabled_non_chars)
        : KeyLoggerBase(),
          pimpl(std::make_unique<Impl>(std::move(enabled_non_chars)))
        {}
        CharLogger::CharLogger(const std::set<KeyCode>& enabled_non_chars)
        : KeyLoggerBase(),
          pimpl(std::make_unique<Impl>(enabled_non_chars))
        {}
        CharLogger::CharLogger(std::set<KeyCode>&& enabled_non_chars)
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

        //
        // This function splits and logs as different logs
        // when multiple ASCII characters are included in one log.
        //
        void CharLogger::logging_without_multi_ascii(const KeyLog& log) {
            KeyLog::Data not_ascii{} ;

            const auto num_log = size() ;
            if(log.is_containing(KEYCODE_SHIFT)) {
                not_ascii.insert(KEYCODE_SHIFT) ;

                for(const auto& key : log) {
                    if(key.is_ascii()) {
                        logging(KeyLog{KEYCODE_SHIFT, key}) ;
                    }
                    else {
                        not_ascii.insert(key) ;
                    }
                }
            }
            else {
                for(const auto& key : log) {
                    if(key.is_ascii()) {
                        logging(KeyLog{key}) ;
                    }
                    else {
                        not_ascii.insert(key) ;
                    }
                }
            }

            if(size() > num_log) { // log contains at least one ASCII
                if(!latest().empty()) {
                    for(const auto& key : latest()) {
                        not_ascii.insert(key) ;
                    }
                }
                latest() = std::move(not_ascii) ;
            }
            else { // First logging
                logging(std::move(not_ascii)) ;
            }
        }

        void CharLogger::enable_non_character(KeyCode keycode) {
            pimpl->non_chars_.insert(keycode) ;
        }
        void CharLogger::disable_non_character(KeyCode keycode) {
            pimpl->non_chars_.erase(keycode) ;
        }

        void CharLogger::sync_state_with(const CharLogger& rhs) {
            if(rhs.pimpl) {
                pimpl->prelog_ = rhs.pimpl->prelog_ ;
                pimpl->ksr_    = rhs.pimpl->ksr_ ;
            }
        }

        int CharLogger::logging_state(const KeyLog& log) {
            if(log != pimpl->prelog_) { //type is changed
                auto diff = log - pimpl->prelog_ ;
                pimpl->prelog_ = log ;

                if(diff.empty()) {
                    return 0 ;
                }

                if(!pimpl->is_including_enabled_chars(log) \
                        && !is_including_ascii(log)) {
                    return 0 ;
                }

                if(log.is_containing(KEYCODE_SHIFT)) { //shfited
                    auto data = diff.get() ;
                    data.insert(KEYCODE_SHIFT) ;

                    //construct KeyLog inside logs directly from std::vector
                    logging_without_multi_ascii(KeyLog(std::move(data))) ;
                }
                else {
                    logging_without_multi_ascii(std::move(diff)) ;
                }

                pimpl->ksr_.reset() ;
                return static_cast<int>(latest().size()) ;
            }

            //long pressing
            if(log.empty()) {
                return 0 ;
            }

            if(!pimpl->is_including_enabled_chars(log) \
                    && !is_including_ascii(log)) {
                return 0 ;
            }

            //emulate key stroke
            if(pimpl->ksr_.is_passed()) {
                logging_without_multi_ascii(std::move(log)) ;
                return static_cast<int>(latest().size()) ;
            }
            return 0 ;
        }

        std::string CharLogger::to_str() const {
            if(empty()) {
                return "" ;
            }

            std::string str{} ;
            for(auto itr = cbegin() ; itr != cend() ; itr ++) {
                if(itr->is_containing(KEYCODE_SHIFT)) {
                    //shifted ascii
                    for(const auto& keycode : *itr) {
                        if(auto c = keycode.to_shifted_ascii()) {
                            str.push_back(c) ;
                        }
                    }
                }
                else {
                    for(const auto& keycode : *itr) {
                        if(auto c = keycode.to_ascii()) {
                            str.push_back(c) ;
                        }
                    }
                }
            }
            return str ;
        }
    }
}
