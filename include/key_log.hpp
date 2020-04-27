#ifndef _KEY_LOG_HPP
#define _KEY_LOG_HPP

#include <memory>
#include <vector>
#include <string>

class KeyLog
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    using data_t = std::vector<unsigned char> ;
    explicit KeyLog() ;
    explicit KeyLog(const data_t& codes) ;
    explicit KeyLog(data_t&& codes) ;

    ~KeyLog() ;

    KeyLog(KeyLog&&) ;
    KeyLog& operator=(KeyLog&&) ;

    KeyLog(const KeyLog&) ;
    KeyLog& operator=(const KeyLog&) ;

    data_t::const_iterator begin() const noexcept ;
    data_t::const_iterator end() const noexcept ;

    data_t::const_iterator cbegin() const noexcept ;
    data_t::const_iterator cend() const noexcept ;

    unsigned char back() const noexcept ;

    std::size_t size() const noexcept ;
    bool is_empty() const noexcept ;

    bool is_included(const unsigned char key) const noexcept ;

    bool operator==(const KeyLog& rhs) const noexcept ;
    bool operator!=(const KeyLog& rhs) const noexcept ;
    const KeyLog operator-(const KeyLog& rhs) const ;
} ;

#endif