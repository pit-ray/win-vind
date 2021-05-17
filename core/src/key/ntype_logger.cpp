#include "key/ntype_logger.hpp"

#include "bind/binded_func.hpp"
#include "bind/logger_parser.hpp"
#include "err_logger.hpp"
#include "key/key_absorber.hpp"
#include "key/key_logger_base.hpp"
#include "key/keycodecvt.hpp"
#include "key/log_map.hpp"
#include "mode.hpp"
#include "time/keystroke_repeater.hpp"

#include <stack>


namespace
{
    using namespace vind ;

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

    template <typename KeyLogType>
    KeyLog extract_numbers(const KeyLog& log, KeyLogType&& ignore_keys) {
        auto to_ascii_func = log.is_containing(KEYCODE_SHIFT) ?
            keycodecvt::get_shifted_ascii : keycodecvt::get_ascii ;

        KeyLog::Data nums{} ;
        for(const KeyCode& keycode : log) {
            // The repeat number isn't begun with zero.
            // 01 or 02 are invalid syntax.
            if(ignore_keys.is_containing(keycode)) {
                continue ;
            }

            // Once convert inputted keycode to ASCII to distinguish if
            // a numeric keycode is typed in order to write a number. 
            if(keycodecvt::is_number_ascii(to_ascii_func(keycode))) {
                nums.insert(keycode) ;
            }
        }

        return KeyLog(nums) ;
    }
}


namespace vind
{
    struct NTypeLogger::Impl {
        KeyLog prelog_{} ;
        KeyLog prelog_without_headnum_{} ;

        KeyStrokeRepeater ksr_{} ;
        unsigned int head_num_ = 0 ;
        LoggerStateRawType state_ = LoggerState::INITIAL ;

        void concatenate_repeating_number(KeyCode keycode) {
            auto num = keycodecvt::to_number<unsigned int>(keycode) ;
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
        pimpl->head_num_ = keycodecvt::to_number<decltype(pimpl->head_num_)>(*num_only_log.cbegin()) ;
        pimpl->state_ = LoggerState::PARSING_NUM ;
        return -1 ;
    }

    int NTypeLogger::do_initial_state(const KeyLog& log) {
        if(log.empty()) {
            return 0 ;
        }

        if(!mode::is_insert()) {
            auto nums = extract_numbers(log, KeyLog{KEYCODE_0}) ;
            if(!nums.empty()) {
                return transition_to_parsing_num_state(nums) ;
            }
        }

        logging(log) ;
        pimpl->state_ = LoggerState::WAITING ;
        return static_cast<int>(log.size()) ;
    }

    int NTypeLogger::do_initial_waiting_state(const KeyLog& log) {
        if(log.empty()) {
            pimpl->state_ = LoggerState::INITIAL ;
            return 0 ;
        }

        auto nums = extract_numbers(log, KeyLog{KEYCODE_0}) ;
        auto nonum = log - nums ;
        if(nonum.empty()) {
            return 0 ;
        }

        logging(nonum) ;
        pimpl->state_ = LoggerState::WAITING ;
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
            pimpl->state_ = LoggerState::INITIAL_WAITING ;
            return do_initial_waiting_state(log) ; //Epsilon Transition
        }

        pimpl->state_ |= LoggerState::LONG_PRESSING ;
        return static_cast<int>(log.size()) ;
    }

    int NTypeLogger::do_parsing_num_state(const KeyLog& log) {
        if(log.empty()) {
            return 0 ;
        }

        auto nums = extract_numbers(log, KeyLog{}) ;
        if(nums.size() != log.size()) {
            pimpl->state_ = LoggerState::WAITING ;
            return do_waiting_state(log - nums) ;
        }

        if(log != pimpl->prelog_) {
            pimpl->concatenate_repeating_number(*nums.cbegin()) ;
            pimpl->ksr_.reset() ;
            return -1 ;
        }
        else {
            if(pimpl->ksr_.is_pressed()) {
                pimpl->concatenate_repeating_number(*nums.cbegin()) ;
                return -1 ;
            }
        }
        return 0 ;
    }

    int NTypeLogger::logging_state() {
        static const KeyLog cl_toggles(keycodecvt::get_toggle_keys()) ;
        auto log = keyabsorber::get_pressed_list() - cl_toggles ; //ignore toggle keys

        log = logmap::do_noremap(log) ;

        int result ;
        switch(pimpl->state_ & LoggerState::STATE_MASK) {
            case LoggerState::INITIAL:
                result = do_initial_state(log) ;
                break ;

            case LoggerState::INITIAL_WAITING:
                result = do_initial_waiting_state(log) ;
                break ;

            case LoggerState::WAITING:
                result = do_waiting_state(log) ;
                break ;

            case LoggerState::ACCEPTED:
                result = do_accepted_state(log) ;
                break ;

            case LoggerState::PARSING_NUM:
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

    bool NTypeLogger::is_long_pressing() const noexcept {
        return pimpl->state_ & LoggerState::LONG_PRESSING ;
    }

    void NTypeLogger::reject() noexcept {
        clear() ;
    }

    void NTypeLogger::clear() noexcept {
        pimpl->head_num_ =  0 ;
        pimpl->state_ = LoggerState::INITIAL ;
        KeyLoggerBase::clear() ;
    }

    void NTypeLogger::accept() noexcept {
        pimpl->state_ = LoggerState::ACCEPTED ;
    }
}
