#ifndef _KEY_LOGGER_HPP
#define _KEY_LOGGER_HPP

#include <vector>
#include "key_log.hpp"

class KeyLogger
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    explicit KeyLogger() noexcept ;
    virtual ~KeyLogger() noexcept ;

    std::size_t size() const noexcept ;
    bool is_empty() const noexcept ;
    void clear() noexcept ;

    void remove_from_top(const std::size_t num_from_top) noexcept ;
    void remove_from_back(std::size_t num_from_back) noexcept ;

    using data_t = std::vector<KeyLog> ;
    data_t::const_iterator begin() const noexcept ;
    data_t::const_iterator end() const noexcept ;
    data_t::const_iterator cbegin() const noexcept ;
    data_t::const_iterator cend() const noexcept ;

    const KeyLog back() const noexcept ;
    const KeyLog at(const std::size_t i) const ;

    //if not changed, return false
    bool is_changed_code() ; //logging direct
    bool is_changed_char() ; //regard inputed key log as ascii charactor

    const std::string get_str() const noexcept ;

    KeyLogger(KeyLogger&&) = delete ;
    KeyLogger& operator=(KeyLogger&&) = delete ;
    KeyLogger(const KeyLogger&) = delete ;
    KeyLogger& operator=(const KeyLogger&) = delete ;
} ;

#endif
