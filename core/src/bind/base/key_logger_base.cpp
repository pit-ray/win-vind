#include "bind/base/key_logger_base.hpp"

#include "util/container.hpp"

#include "key/key_absorber.hpp"
#include "key/keycodecvt.hpp"

#include <sstream>

namespace vind
{
    struct KeyLoggerBase::Impl {
        data_t data{} ;
    } ;

    KeyLoggerBase::KeyLoggerBase()
    : pimpl(std::make_unique<Impl>())
    {}

    KeyLoggerBase::~KeyLoggerBase() noexcept = default ;

    KeyLoggerBase::KeyLoggerBase(const KeyLoggerBase& rhs)
    : pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
    {}

    KeyLoggerBase& KeyLoggerBase::operator=(const KeyLoggerBase& rhs) {
        if(rhs.pimpl) *pimpl = *(rhs.pimpl) ;
        return *this ;
    }

    KeyLoggerBase::KeyLoggerBase(KeyLoggerBase&& rhs)            = default ;
    KeyLoggerBase& KeyLoggerBase::operator=(KeyLoggerBase&& rhs) = default ;

    KeyLoggerBase::data_t::iterator KeyLoggerBase::begin() noexcept {
        return pimpl->data.begin() ;
    }
    KeyLoggerBase::data_t::iterator KeyLoggerBase::end() noexcept {
        return pimpl->data.end() ;
    }

    KeyLoggerBase::data_t::const_iterator KeyLoggerBase::begin() const noexcept {
        return pimpl->data.begin() ;
    }
    KeyLoggerBase::data_t::const_iterator KeyLoggerBase::end() const noexcept {
        return pimpl->data.end() ;
    }

    KeyLoggerBase::data_t::const_iterator KeyLoggerBase::cbegin() const noexcept {
        return pimpl->data.cbegin() ;
    }
    KeyLoggerBase::data_t::const_iterator KeyLoggerBase::cend() const noexcept {
        return pimpl->data.cend() ;
    }

    KeyLoggerBase::data_t::reference KeyLoggerBase::latest() {
        return pimpl->data.back() ;
    }
    KeyLoggerBase::data_t::const_reference KeyLoggerBase::latest() const {
        return pimpl->data.back() ;
    }

    std::size_t KeyLoggerBase::size() const noexcept {
        return pimpl->data.size() ;
    }

    bool KeyLoggerBase::empty() const noexcept {
        return pimpl->data.empty() ;
    }

    void KeyLoggerBase::clear() noexcept {
        return pimpl->data.clear() ;
    }

    void KeyLoggerBase::logging(const KeyLog& log) {
        pimpl->data.push_back(log) ;
    }
    void KeyLoggerBase::logging(KeyLog&& log) {
        pimpl->data.push_back(std::move(log)) ;
    }

    void KeyLoggerBase::logging(const KeyLog::data_t& lograw) {
        pimpl->data.emplace_back(lograw) ;
    }
    void KeyLoggerBase::logging(KeyLog::data_t&& lograw) {
        pimpl->data.emplace_back(std::move(lograw)) ;
    }

    void KeyLoggerBase::remove_from_back(std::size_t num_from_back) {
        util::remove_from_back(pimpl->data, num_from_back) ;
    }

    KeyLoggerBase::data_t::const_reference KeyLoggerBase::at(std::size_t n) const {
        return pimpl->data.at(n) ;
    }

    bool KeyLoggerBase::operator==(const KeyLoggerBase& rhs) const {
        return pimpl->data == rhs.pimpl->data ;
    }

    bool KeyLoggerBase::operator!=(const KeyLoggerBase& rhs) const {
        return pimpl->data != rhs.pimpl->data ;
    }

    int KeyLoggerBase::logging_state() {
        logging(keyabsorber::get_pressed_list()) ;
        return latest().size() ;
    }

    namespace keyloggerutil
    {
        unsigned int extract_num(const std::string str) {
            constexpr auto c_nums = "0123456789" ;

            auto bpos = str.find_first_of(c_nums) ;
            if(bpos == std::string::npos) return 0 ;

            auto epos = str.find_first_not_of(c_nums, bpos) ;

            return std::stoi(str.substr(bpos, epos)) ;
        }

        namespace debug {
            std::string print_log(const KeyLoggerBase& lgr) {
                std::stringstream ss ;
                if(lgr.empty()) {
                    return "<Empty>\n" ;
                }
                for(const auto& log : lgr) {
                    for(const auto& key : log) {
                        if(key == KEYCODE_SPACE) {
                            ss << "<space> " ;
                            continue ;
                        }

                        if(auto a = keycodecvt::get_ascii(key)) {
                            ss << a << " " ;
                            continue ;
                        }
                        if(auto a = keycodecvt::get_shifted_ascii(key)) {
                            ss << a << " " ;
                            continue ;
                        }
                        auto name = keycodecvt::get_name(key) ;
                        ss << "<" << name << "> " ;
                    }
                    ss << "| " ;
                }
                ss << std::endl ;
                return ss.str() ;
            }
        }
    }
}
