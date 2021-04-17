#ifndef _KEY_LOG_HPP
#define _KEY_LOG_HPP

#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_set>

namespace vind
{
    class KeyLog {
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
        bool operator==(KeyLog&& rhs) const ;
        bool operator==(const data_t& rhsraw) const ;
        bool operator==(data_t&& rhsraw) const ;

        bool operator!=(const KeyLog& rhs) const ;
        bool operator!=(KeyLog&& rhs) const ;
        bool operator!=(const data_t& rhs) const ;
        bool operator!=(data_t&& rhsw) const ;

        const KeyLog operator-(const KeyLog& rhs) const ;
        const KeyLog operator-(KeyLog&& rhs) const ;
        const KeyLog operator-(const data_t& rhs) const ;
        const KeyLog operator-(data_t&& rhs) const ;

        KeyLog& operator-=(const KeyLog& rhs) ;
        KeyLog& operator-=(KeyLog&& rhs) ;
        KeyLog& operator-=(const data_t& rhs) ;
        KeyLog& operator-=(data_t&& rhs) ;
    } ;
}

#endif
