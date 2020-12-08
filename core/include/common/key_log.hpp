#ifndef _KEY_LOG_HPP
#define _KEY_LOG_HPP

#include <memory>
#include <unordered_set>
#include <string>
#include <initializer_list>

class KeyLog
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    using data_t = std::unordered_set<unsigned char> ;
    explicit KeyLog() ;
    explicit KeyLog(const data_t& codes) ;
    explicit KeyLog(data_t&& codes) ;
    explicit KeyLog(std::initializer_list<unsigned char>&& codes) ;

    virtual ~KeyLog() noexcept ;

    KeyLog(KeyLog&&) ;
    KeyLog& operator=(KeyLog&&) ;
    KeyLog(const KeyLog&) ;
    KeyLog& operator=(const KeyLog&) ;

    const data_t& get() const & noexcept ;
    data_t&& get() && noexcept ;

    data_t::const_iterator begin() const noexcept ;
    data_t::const_iterator end() const noexcept ;

    data_t::const_iterator cbegin() const noexcept ;
    data_t::const_iterator cend() const noexcept ;

    std::size_t size() const noexcept ;
    bool empty() const noexcept ;
    bool is_containing(const unsigned char key) const ;

    bool operator==(const KeyLog& rhs) const ;
    bool operator!=(const KeyLog& rhs) const ;
    const KeyLog operator-(const KeyLog& rhs) const ;
} ;

#endif
