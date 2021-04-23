#include "bind/base/ntype_logger.hpp"

#include "bind/base/key_logger_base.hpp"
#include "coreio/err_logger.hpp"
#include "key/key_absorber.hpp"
#include "key/keycodecvt.hpp"
#include "time/keystroke_repeater.hpp"

#include <stack>


namespace
{
    using namespace vind ;

    using LoggerStateRawType = unsigned char ;

    enum LoggerState : LoggerStateRawType{
        INITIAL,
        WAITING,
        PRESSING,
        WAITING_HEADNUM,
    } ;

    template <typename KeyLogType>
    const KeyLog extract_number(const KeyLog& log, KeyLogType&& ignore_keys) {
        auto to_ascii_func = log.is_containing(KEYCODE_SHIFT) ?
            keycodecvt::get_shifted_ascii : keycodecvt::get_ascii ;

        KeyLog::data_t nums{} ;
        for(const unsigned char& keycode : log) {
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


    using LoggerStateStack = std::stack<LoggerState, std::vector<LoggerState>> ;
}


namespace vind
{
    struct NTypeLogger::Impl {
        KeyLog prelog_{} ;
        KeyLog prelog_without_headnum_{} ;
        KeyStrokeRepeater ksr_{} ;
        bool pressing_ = false ;
        bool accepted_ = false ;
        unsigned int head_num_ = 0 ;
        LoggerState state_ = LoggerState::INITIAL ;
        LoggerStateStack state_hist_{} ;

        void concatenate_repeating_number(unsigned char keycode) {
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

    int NTypeLogger::do_initial_state(const KeyLog& log) {
        if(log.empty()) {
            return 0 ;
        }
        pimpl->head_num_ = 0 ;
        pimpl->accepted_ = false ;

        auto nums = extract_number(log, KeyLog{KEYCODE_0}) ;
        if(!nums.empty()) {
            pimpl->ksr_.reset() ;
            pimpl->head_num_ = keycodecvt::to_number<decltype(pimpl->head_num_)>(*nums.cbegin()) ;
            pimpl->state_ = LoggerState::WAITING_HEADNUM ;
            return -1 ;
        }

        pimpl->state_ = LoggerState::WAITING ;
        logging(log) ;
        return static_cast<int>(log.size()) ;
    }

    int NTypeLogger::do_waiting_state(const KeyLog& log) {
        if(pimpl->accepted_) {
            pimpl->state_ = LoggerState::PRESSING ;
            return do_long_pressing_state(log) ;
        }

        if((log - pimpl->prelog_).empty()) {
            return 0 ;
        }
        // If increase a variety of the inputted keys, only then call logging.
        logging(log) ;
        return static_cast<int>(log.size()) ;
    }

    int NTypeLogger::do_long_pressing_state(const KeyLog& log) {
        if(log != pimpl->prelog_) {
            KeyLoggerBase::clear() ;
            pimpl->state_ = LoggerState::INITIAL ;
            return do_initial_state(log) ;
        }
        return static_cast<int>(log.size()) ;
    }

    int NTypeLogger::do_waiting_repeat_num_state(const KeyLog& log) {
        if(log.empty()) {
            return 0 ;
        }

        auto nums = extract_number(log, KeyLog{}) ;
        if(nums.size() != log.size()) {
            pimpl->state_ = LoggerState::WAITING ;
            return do_waiting_state(log) ;
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

    bool NTypeLogger::parse_head_number(KeyLog& log) {
        auto to_ascii = log.is_containing(KEYCODE_SHIFT) ?
            keycodecvt::get_shifted_ascii : keycodecvt::get_ascii ;

        KeyLog::data_t remove_nums{} ;
        if(pimpl->head_num_ == 0) {
            if((log - pimpl->prelog_).empty()) {
                return false ;
            }

            for(const unsigned char& keycode : log) {
                // The repeat number isn't begun with zero.
                // 01 or 02 are invalid syntax.
                if(keycode == KEYCODE_0) {
                    continue ;
                }
                // Once convert inputted keycode to ASCII to distinguish if
                // a numeric keycode is typed in order to write a number. 
                if(keycodecvt::is_number_ascii(to_ascii(keycode))) {
                    remove_nums.insert(keycode) ;
                }
            }
        }
        else {
            for(const unsigned char& keycode : log) {
                if(keycodecvt::is_number_ascii(to_ascii(keycode))) {
                    remove_nums.insert(keycode) ;
                }
            }
        }

        if(remove_nums.empty()) {
            return false ;
        }

        bool num_pressing = true ;
        if(log != pimpl->prelog_) {
            num_pressing = false ;
            pimpl->ksr_.reset() ;
        }

        log -= remove_nums ;

        for(const unsigned char& keycode : remove_nums) {
            if(num_pressing) {
                if(!pimpl->ksr_.is_pressed()) {
                    return false ;
                }
            }
            else {
                if(pimpl->prelog_.is_containing(keycode)) {
                    continue ;
                }
            }
            auto num = keycodecvt::to_number<unsigned int>(keycode) ;
            constexpr auto max = std::numeric_limits<unsigned int>::max() / 10 ;
            if(pimpl->head_num_ < max) {
                pimpl->head_num_ = pimpl->head_num_ * 10 + num ;
            }
            return true ;
        }
        return false ;
    }

    int NTypeLogger::logging_first_log(const KeyLog& log) {
        if(log.empty()) {
            return 0 ;
        }
        logging(log) ;
        pimpl->prelog_without_headnum_ = log ;
        return static_cast<int>(latest().size()) ;
    }

    int NTypeLogger::logging_pressing_log(const KeyLog& log) {
        if(!log.empty()) {
            //auto diff = pimpl->prelog_without_headnum_ - log ;
            //auto diff = log - pimpl->prelog_ ;
            if(log == pimpl->prelog_without_headnum_) {
            //if(pimpl->prelog_ == log) {
                pimpl->pressing_ = true ;
                return static_cast<int>(latest().size()) ;
            }
        }

        KeyLoggerBase::clear() ;
        pimpl->accepted_ = false ;
        pimpl->pressing_ = false ;
        return logging_first_log(log) ;
    }

    int NTypeLogger::logging_sequence_log(const KeyLog& log) {
        auto diff = log - pimpl->prelog_ ; //remove same keys as prelog
        if(diff.empty()) {
            // regards the reducing of the inputted keys as the same empty state.
            return 0 ;
        }
        else {
            //inputting keys are changed.
            logging(log) ;
            return static_cast<int>(latest().size()) ;
        }
    }

    int NTypeLogger::do_logging_state(const KeyLog& log) {
        if(empty()) { //does not have any log
            pimpl->pressing_ = false ;
            pimpl->accepted_ = false ;

            if(log.empty()) {
                return 0 ;
            }

            auto buf = log ;
            if(parse_head_number(buf)) {
                //pimpl->prelog_without_headnum_ = std::move(buf) ;
                return -1 ;
            }

            //It does not include numeric key code.
            auto result = logging_first_log(buf) ;
            return result ;
        }

        if(pimpl->accepted_) {
            pimpl->head_num_ = 0 ;
            auto result = logging_pressing_log(log) ;
            pimpl->prelog_without_headnum_ = log ;
            return result ;
        }
        auto result = logging_sequence_log(log) ;
        pimpl->prelog_without_headnum_ = log ;
        return result ;
    }

    int NTypeLogger::logging_state() {
        //pre process
        static const KeyLog cl_toggles(keycodecvt::get_toggle_keys()) ;
        const auto log = keyabsorber::get_pressed_list() - cl_toggles ; //ignore toggle keys

        pimpl->state_hist_.push(pimpl->state_) ;

        int result ;
        switch(pimpl->state_) {
            case LoggerState::INITIAL:
                std::cout << "Initial\n" ;
                result = do_initial_state(log) ;
                break ;

            case LoggerState::WAITING:
                std::cout << "Waiting\n" ;
                result = do_waiting_state(log) ;
                break ;

            case LoggerState::PRESSING:
                std::cout << "Pressing\n" ;
                result = do_long_pressing_state(log) ;
                break ;

            case LoggerState::WAITING_HEADNUM:
                std::cout << "Waiting Head num\n" ;
                result = do_waiting_repeat_num_state(log) ;
                break ;

            default:
                std::cout << "nothing\n" ;
                result = do_initial_state(log) ;
                break ;
        }

        //post process
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
        //return pimpl->pressing_ ;
        return pimpl->state_ == LoggerState::PRESSING ;
    }

    void NTypeLogger::clear() noexcept {
        pimpl->accepted_ = false ;
        pimpl->pressing_ = false ;
        pimpl->head_num_ =  0 ;
        pimpl->state_ = LoggerState::INITIAL ;
        LoggerStateStack().swap(pimpl->state_hist_) ;
        KeyLoggerBase::clear() ;
    }

    void NTypeLogger::accept() noexcept {
        pimpl->accepted_ = true ;
    }

    void NTypeLogger::ignore() noexcept {
        /*
        if(size() > 1) {
            pimpl->head_num_ = 0 ;
        }
        KeyLoggerBase::clear() ;
        */
        if(pimpl->state_hist_.empty()) {
            pimpl->state_ = LoggerState::INITIAL ;
        }
        else {
            pimpl->state_ = pimpl->state_hist_.top() ;
            pimpl->state_hist_.pop() ;
        }

        remove_from_back(1) ;
    }
}
