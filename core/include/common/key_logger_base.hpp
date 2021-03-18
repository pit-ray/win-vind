#ifndef _KEY_LOGGER_BASE_HPP
#define _KEY_LOGGER_BASE_HPP

#include <vector>
#include <memory>
#include "key_log.hpp"

class KeyLoggerBase
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    using data_t = std::vector<KeyLog> ;

    explicit KeyLoggerBase() ;
    virtual ~KeyLoggerBase() noexcept ;

    KeyLoggerBase(const KeyLoggerBase&) ;
    KeyLoggerBase& operator=(const KeyLoggerBase&) ;

    KeyLoggerBase(KeyLoggerBase&&) ;
    KeyLoggerBase& operator=(KeyLoggerBase&&) ;

    data_t::iterator begin() noexcept ;
    data_t::iterator end() noexcept ;

    data_t::const_iterator begin() const noexcept ;
    data_t::const_iterator end() const noexcept ;

    data_t::const_iterator cbegin() const noexcept ;
    data_t::const_iterator cend() const noexcept ;

    data_t::reference latest() ;
    data_t::const_reference latest() const ;

    std::size_t size() const noexcept ;
    bool empty() const noexcept ;

    void clear() ;

    void logging(const KeyLog& log) ;
    void logging(KeyLog&& log) ;

    void logging(const KeyLog::data_t& lograw) ;
    void logging(KeyLog::data_t&& lograw) ;

    void remove_from_back(std::size_t num_from_back) ;

    data_t::const_reference at(std::size_t n) const ;

    bool operator==(const KeyLoggerBase& rhs) const ;
    bool operator!=(const KeyLoggerBase& rhs) const ;

    virtual void update() = 0 ;
    virtual bool is_changed() const noexcept = 0 ;
} ;

namespace KeyLoggerUtility
{
    // Extracts a number from a string.
    // Its range is from the appearance of the number to a non-numeric character.
    // Example:
    //      std::string: hBah0784hlk503
    //      -> std::stoi(0784)
    //      uint: 784
    //
    unsigned int extract_num(const std::string str) ;

    namespace Debug {
        //for debug
        std::string print_log(const KeyLoggerBase& lgr) ;
    }
}

#endif
