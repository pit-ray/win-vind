#define NOMINMAX

#include "ntypelogger.hpp"

#include "bind/bindedfunc.hpp"
#include "errlogger.hpp"
#include "inputgate.hpp"
#include "keycode.hpp"
#include "keylgrbase.hpp"
#include "lgrparser.hpp"
#include "mode.hpp"
#include "util/keystroke_repeater.hpp"

#include <limits>
#include <stack>


namespace
{
    using namespace vind::core ;

    bool only_having_shift(const KeyLog& log) {
        for(const auto& keycode : log) {
            if(!keycode.is_shift()) {
                return false ;
            }
        }

        return true ;
    }

    template <
        typename KeyCodeSet,
        typename ShiftSubSet,
        typename IgnoreKeyCodeSet>
    KeyLog extract_numbers(
            KeyCodeSet&& log,
            ShiftSubSet&& shift_subset,
            IgnoreKeyCodeSet&& ignore_keys) {

        KeyLog::Data nums{} ;
        for(const auto& keycode : log) {
            // The repeat number isn't begun with zero.
            // 01 or 02 are invalid syntax.
            if(std::find(
                    ignore_keys.begin(),
                    ignore_keys.end(),
                    keycode) != ignore_keys.end()) {

                continue ;
            }

            auto uni = keycode_to_unicode(
                    keycode,
                    std::forward<ShiftSubSet>(shift_subset)) ;
            if(!uni.empty()) {
                auto ascii = uni.front() ;
                if('0' <= ascii && ascii <= '9') {
                    nums.insert(keycode) ;
                }
            }
        }

        return KeyLog(nums) ;
    }
}


namespace vind
{
    namespace core
    {
        struct NTypeLogger::Impl {
            using LoggerStateRawType = unsigned char ;

            enum LoggerState : LoggerStateRawType{
                INITIAL         = 0b0000'0001,
                INITIAL_WAITING = 0b0000'0010,
                WAITING         = 0b0000'0100,
                ACCEPTED        = 0b0000'1000,
                PARSING_NUM     = 0b0001'0000,

                STATE_MASK      = 0b0001'1111,
                LONG_PRESSING   = 0b1000'0000,
            } ;

            KeyLog prelog_{} ;
            KeyLog prelog_without_headnum_{} ;

            util::KeyStrokeRepeater ksr_{} ;
            unsigned int head_num_ = 0 ;
            LoggerStateRawType state_ = LoggerState::INITIAL ;

            void concat_repeat_num(unsigned int num) {
                constexpr auto max = std::numeric_limits<unsigned int>::max() / 10 ;
                if(head_num_ < max) {
                    head_num_ = head_num_ * 10 + num ;
                }
            }
        } ;

        NTypeLogger::NTypeLogger()
        : KeyLoggerBase(),
          pimpl(std::make_unique<Impl>())
        {}

        NTypeLogger::~NTypeLogger() noexcept = default ;

        NTypeLogger::NTypeLogger(const NTypeLogger& rhs)
        : KeyLoggerBase(rhs),
          pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
        {}
        NTypeLogger& NTypeLogger::operator=(const NTypeLogger& rhs) {
            if(rhs.pimpl) {
                KeyLoggerBase::operator=(rhs) ;
                *pimpl = *(rhs.pimpl) ;
            }
            return *this ;
        }

        NTypeLogger::NTypeLogger(NTypeLogger&&)            = default ;
        NTypeLogger& NTypeLogger::operator=(NTypeLogger&&) = default ;


        int NTypeLogger::transition_to_parsing_num_state(const KeyLog& num_only_log) {
            pimpl->ksr_.reset() ;
            pimpl->head_num_ = static_cast<decltype(pimpl->head_num_)>( \
                    keycode_to_number(*(num_only_log.cbegin()))) ;
            pimpl->state_ = Impl::LoggerState::PARSING_NUM ;
            return -1 ;
        }

        int NTypeLogger::do_initial_state(const KeyLog& log) {
            if(log.empty() || only_having_shift(log)) {
                return 0 ;
            }

            if(core::InputGate::get_instance().is_absorbed()) {
                auto nums = extract_numbers(log, log, KeyLog{KEYCODE_0}) ;
                if(!nums.empty()) {
                    return transition_to_parsing_num_state(nums) ;
                }
            }

            logging(log) ;
            pimpl->state_ = Impl::LoggerState::WAITING ;
            return static_cast<int>(log.size()) ;
        }

        int NTypeLogger::do_initial_waiting_state(const KeyLog& log) {
            if(log.empty()) {
                pimpl->state_ = Impl::LoggerState::INITIAL ;
                return 0 ;
            }

            auto nums = extract_numbers(log, log, KeyLog{KEYCODE_0}) ;
            auto nonum = log - nums ;
            if(nonum.empty()) {
                return 0 ;
            }

            logging(nonum) ;
            pimpl->state_ = Impl::LoggerState::WAITING ;
            return static_cast<int>(nonum.size()) ;
        }

        int NTypeLogger::do_waiting_state(const KeyLog& log) {
            if((log - pimpl->prelog_).empty()) {
                return 0 ;
            }
            // If increase a variety of the inputted keys, only then call logging.
            logging(log) ;
            return static_cast<int>(log.size()) ;
        }

        int NTypeLogger::do_accepted_state(const KeyLog& log) {
            if(log.empty()) {
                clear() ;
                return 0 ;
            }

            if(latest() != log) {
                clear() ;
                pimpl->state_ = Impl::LoggerState::INITIAL_WAITING ;
                return do_initial_waiting_state(log) ; //Epsilon Transition
            }

            pimpl->state_ |= Impl::LoggerState::LONG_PRESSING ;
            return static_cast<int>(log.size()) ;
        }

        int NTypeLogger::do_parsing_num_state(const KeyLog& log) {
            if(log.empty()) {
                return 0 ;
            }

            KeyLog::Data target_subset{} ;
            KeyLog::Data shift_subset{} ;
            for(const auto& keycode : log) {
                if(keycode.is_shift()) {
                    shift_subset.insert(keycode) ;
                }
                else {
                    target_subset.insert(keycode) ;
                }
            }

            // Wait for input of numbers until shift and non-numbers are input.
            auto nums = extract_numbers(target_subset, shift_subset, KeyLog{}) ;
            if(nums.empty()) {
                // Consider the case where use shift to enter
                // numbers, such as in the French keyboard layout,
                // and continue when shift and numbers only.
                if(!shift_subset.empty() && nums == target_subset) {
                    return 0 ;
                }

                pimpl->state_ = Impl::LoggerState::WAITING ;
                return do_waiting_state(log) ;
            }

            if(log == pimpl->prelog_) {
                if(pimpl->ksr_.is_passed()) {
                    pimpl->concat_repeat_num(pimpl->head_num_ % 10) ;
                    return -1 ;
                }
                return 0 ;
            }

            // To prevent overlapping character input, use only the
            // newly entered ones. For example, when you input `10`,
            // it may generate some merged messages, such as <1><1-0><0>.
            auto diff_nums = nums - pimpl->prelog_ ;
            if(diff_nums.empty()) {
                return 0 ;
            }

            pimpl->concat_repeat_num(
                    keycode_to_number(*(diff_nums.cbegin()))) ;
            pimpl->ksr_.reset() ;
            return -1 ;
        }

        int NTypeLogger::logging_state(const KeyLog& log) {
            int result ;
            switch(pimpl->state_ & Impl::LoggerState::STATE_MASK) {
                case Impl::LoggerState::INITIAL:
                    result = do_initial_state(log) ;
                    break ;

                case Impl::LoggerState::INITIAL_WAITING:
                    result = do_initial_waiting_state(log) ;
                    break ;

                case Impl::LoggerState::WAITING:
                    result = do_waiting_state(log) ;
                    break ;

                case Impl::LoggerState::ACCEPTED:
                    result = do_accepted_state(log) ;
                    break ;

                case Impl::LoggerState::PARSING_NUM:
                    result = do_parsing_num_state(log) ;
                    break ;

                default:
                    clear() ;
                    result = do_initial_state(log) ;
                    break ;
            }
            pimpl->prelog_ = std::move(log) ;
            return result ;
        }

        unsigned int NTypeLogger::get_head_num() const noexcept {
            //
            // If head_num_ is zero, regards that not inputted repeat number.
            // And, the default number is one.
            //
            return pimpl->head_num_ == 0 ? 1 : pimpl->head_num_ ;
        }

        void NTypeLogger::set_head_num(unsigned int num) noexcept {
            pimpl->head_num_ = num ;
        }

        bool NTypeLogger::has_head_num() const noexcept {
            return pimpl->head_num_ != 0 ;
        }

        bool NTypeLogger::is_long_pressing() const noexcept {
            return pimpl->state_ & Impl::LoggerState::LONG_PRESSING ;
        }

        void NTypeLogger::reject() noexcept {
            clear() ;
        }

        void NTypeLogger::clear() noexcept {
            pimpl->head_num_ =  0 ;
            pimpl->state_ = Impl::LoggerState::INITIAL ;
            KeyLoggerBase::clear() ;
        }

        void NTypeLogger::accept() noexcept {
            pimpl->state_ = Impl::LoggerState::ACCEPTED ;
        }
    }
}
