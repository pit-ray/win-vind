#include "charlogger.hpp"

#include "core/cmdunit.hpp"
#include "core/inputgate.hpp"
#include "keycode.hpp"
#include "keycodedef.hpp"
#include "keylgrbase.hpp"
#include "keylog.hpp"
#include "util/container.hpp"
#include "util/debug.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/string.hpp"

#include <sstream>
#include <unordered_set>
#include <vector>


namespace vind
{
    namespace core
    {
        struct CharLogger::Impl {
            KeyLog::Data prelog_ ;
            util::KeyStrokeRepeater ksr_ ; 
            KeyLog::Data non_chars_ ;
            std::vector<std::string> strlog_ ;

            KeyCode last_keycode_ ;
            std::string laststr_ ;

            explicit Impl()
            : prelog_(),
              ksr_(),
              non_chars_(),
              strlog_(),
              last_keycode_(),
              laststr_()
            {}
            explicit Impl(const std::initializer_list<KeyCode>& non_chars)
            : prelog_(),
              ksr_(),
              non_chars_(non_chars),
              strlog_(),
              last_keycode_(),
              laststr_()
            {}
            explicit Impl(std::initializer_list<KeyCode>&& non_chars)
            : prelog_(),
              ksr_(),
              non_chars_(std::move(non_chars)),
              strlog_(),
              last_keycode_(),
              laststr_()
            {}
            explicit Impl(const KeyLog::Data& non_chars)
            : prelog_(),
              ksr_(),
              non_chars_(non_chars),
              strlog_(),
              last_keycode_(),
              laststr_()
            {}
            explicit Impl(KeyLog::Data&& non_chars)
            : prelog_(),
              ksr_(),
              non_chars_(std::move(non_chars)),
              strlog_(),
              last_keycode_(),
              laststr_()
            {}
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
        CharLogger::CharLogger(const KeyLog::Data& enabled_non_chars)
        : KeyLoggerBase(),
          pimpl(std::make_unique<Impl>(enabled_non_chars))
        {}
        CharLogger::CharLogger(KeyLog::Data&& enabled_non_chars)
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

        void CharLogger::clear() noexcept {
            pimpl->prelog_.clear() ;

            pimpl->ksr_.reset() ;
            pimpl->last_keycode_ = KeyCode() ;
            pimpl->laststr_.clear() ;

            pimpl->strlog_.clear() ;
            KeyLoggerBase::clear() ;
        }

        void CharLogger::remove_from_back(std::size_t num_from_back) {
            util::remove_from_back(pimpl->strlog_, num_from_back) ;
            KeyLoggerBase::remove_from_back(num_from_back) ;
        }

        void CharLogger::remove_from_top(std::size_t num_from_top) {
            util::remove_from_top(pimpl->strlog_, num_from_top) ;
            KeyLoggerBase::remove_from_top(num_from_top) ;
        }

        void CharLogger::enable_non_character(KeyCode keycode) {
            pimpl->non_chars_.insert(keycode) ;
        }
        void CharLogger::disable_non_character(KeyCode keycode) {
            pimpl->non_chars_.erase(keycode) ;
        }

        void CharLogger::sync_state_with(const CharLogger& rhs) {
            if(rhs.pimpl) {
                pimpl->ksr_ = rhs.pimpl->ksr_ ;
                pimpl->last_keycode_ = rhs.pimpl->last_keycode_ ;
                pimpl->laststr_ = rhs.pimpl->laststr_ ;
                pimpl->prelog_ = rhs.pimpl->prelog_ ;
            }
        }

        int CharLogger::logging_state(const KeyLog& log) {
            if(log == pimpl->prelog_) {
                if(log.empty()) {
                    return 0 ;
                }

                if(!pimpl->ksr_.is_passed()) {
                    return 0 ;
                }

                if(!log.is_containing(pimpl->last_keycode_)) {
                    return 0 ;
                }

                KeyLog::Data subset{} ;
                for(const auto& keycode : log) {
                    if(keycode.is_shift()) {
                        subset.insert(keycode) ;
                    }
                }
                subset.insert(pimpl->last_keycode_) ;

                pimpl->strlog_.push_back(pimpl->laststr_) ;
                logging(std::move(subset)) ;

                return static_cast<int>(latest().size()) ;
            }

            auto diff = log - pimpl->prelog_ ;
            pimpl->prelog_ = log.data() ;

            if(diff.empty()) {
                return 0 ;
            }

            KeyLog::Data target_subset{} ;
            for(const auto& keycode : diff) {
                if(!keycode.is_major_system()) {
                    target_subset.insert(keycode) ;
                }
            }

            KeyLog::Data allow_shift_subset{} ;
            KeyLog::Data shift_subset{} ;
            for(const auto& keycode : log) {
                if(!keycode.is_major_system()) {
                    continue ;
                }

                if(pimpl->non_chars_.find(keycode) \
                        != pimpl->non_chars_.end()) {
                    allow_shift_subset.insert(keycode) ;
                }

                if(keycode.is_shift()) {
                    shift_subset.insert(keycode) ;
                }
            }

            std::size_t before_num = size() ;

            if(!target_subset.empty()) {
                for(const auto& keycode : target_subset) {
                    auto uni = keycode_to_unicode(keycode, shift_subset) ;
                    if(uni.empty()) {
                        continue ;
                    }

                    pimpl->last_keycode_ = keycode ;
                    pimpl->laststr_ = uni ;

                    pimpl->strlog_.push_back(uni) ;

                    auto onechar_inc = shift_subset ;
                    onechar_inc.insert(keycode) ;
                    logging(std::move(onechar_inc)) ;
                }
            }

            // Specially passed system keys are kept as the last log,
            // and the caller is obligated to deal with them.
            if(!allow_shift_subset.empty()) {

                pimpl->last_keycode_ = *(allow_shift_subset.begin()) ;
                pimpl->laststr_ = "<" + pimpl->last_keycode_.name() + ">" ;

                std::stringstream ss ;
                ss << allow_shift_subset ;
                pimpl->strlog_.push_back(ss.str()) ;

                // Reuse shift_subset as the last to use it.
                shift_subset.merge(allow_shift_subset) ;

                logging(std::move(shift_subset)) ;
            }

            if(size() == before_num) {
                return 0 ;
            }

            pimpl->ksr_.reset() ;

            return static_cast<int>(latest().size()) ;
        }

        std::string CharLogger::to_str() const noexcept {
            std::string out ;
            out.reserve(pimpl->strlog_.size()) ;
            for(const auto& str : pimpl->strlog_) {
                out += str ;
            }
            return out ;
        }
    }
}
