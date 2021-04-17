#include "bind/base/ntype_logger.hpp"

#include "bind/base/key_logger_base.hpp"
#include "coreio/err_logger.hpp"
#include "key/key_absorber.hpp"
#include "key/keycodecvt.hpp"
#include "time/keystroke_repeater.hpp"


namespace vind
{
    struct NTypeLogger::Impl {
        KeyLog prelog_{} ;
        KeyLog prelog_without_headnum_{} ;
        KeyStrokeRepeater ksr_{} ;
        bool pressing_ = false ;
        bool accepted_ = false ;
        unsigned int head_num_ = 0 ;
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

    bool NTypeLogger::parse_head_number(KeyLog& log) {
        auto to_ascii = log.is_containing(KEYCODE_SHIFT) ?
            keycodecvt::get_shifted_ascii : keycodecvt::get_ascii ;

        KeyLog::data_t remove_nums{} ;
        if(pimpl->head_num_ == 0) {
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
        if(log.empty() || log == pimpl->prelog_without_headnum_) {
            return 0 ;
        }
        logging(log) ;
        return static_cast<int>(latest().size()) ;
    }

    int NTypeLogger::logging_pressing_log(const KeyLog& log) {
        if(pimpl->prelog_without_headnum_ == log) {
            pimpl->pressing_ = true ;
            return static_cast<int>(latest().size()) ;
        }

        KeyLoggerBase::clear() ;
        pimpl->accepted_ = false ;
        pimpl->pressing_ = false ;
        return logging_first_log(log) ;
    }

    int NTypeLogger::logging_sequence_log(const KeyLog& log) {
        auto diff = log - pimpl->prelog_without_headnum_ ; //remove same keys as prelog
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
                pimpl->prelog_without_headnum_ = log ;
                return 0 ;
            }

            auto buf = log ;
            if(parse_head_number(buf)) {
                pimpl->prelog_without_headnum_ = std::move(buf) ;
                return -1 ;
            }

            //It does not include numeric key code.
            auto result = logging_first_log(buf) ;
            pimpl->prelog_without_headnum_ = std::move(buf) ;
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

        auto result = do_logging_state(log) ;

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
        return pimpl->pressing_ ;
    }

    void NTypeLogger::clear() noexcept {
        pimpl->accepted_ = false ;
        pimpl->pressing_ = false ;
        pimpl->head_num_ =  0 ;
        KeyLoggerBase::clear() ;
    }

    void NTypeLogger::accept() noexcept {
        pimpl->accepted_ = true ;
    }

    void NTypeLogger::ignore() noexcept {
        if(size() > 1) {
            pimpl->head_num_ = 0 ;
        }
        KeyLoggerBase::clear() ;
    }

    void NTypeLogger::update() {
    }

    bool NTypeLogger::is_changed() const noexcept {
        //return pimpl->keycode_changed ;
        return false ;
    }
}
