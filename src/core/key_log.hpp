#ifndef _KEY_LOG_HPP
#define _KEY_LOG_HPP

#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_set>

#include "keycode_def.hpp"

namespace vind
{
    namespace core
    {
        class KeyLog {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            using Data = std::unordered_set<KeyCode> ;
            explicit KeyLog() ;
            explicit KeyLog(const Data& codes) ;
            explicit KeyLog(Data&& codes) ;
            explicit KeyLog(std::initializer_list<KeyCode>&& codes) ;

            template <typename Iterator>
            explicit KeyLog(Iterator begin, Iterator end)
            : KeyLog(Data(begin, end))
            {}

            virtual ~KeyLog() noexcept ;

            KeyLog(KeyLog&&) ;
            KeyLog& operator=(KeyLog&&) ;
            KeyLog& operator=(KeyLog::Data&&) ;

            KeyLog(const KeyLog&) ;
            KeyLog& operator=(const KeyLog&) ;
            KeyLog& operator=(const KeyLog::Data&) ;

            const Data& get() const & noexcept ;
            Data&& get() && noexcept ;

            Data::const_iterator begin() const noexcept ;
            Data::const_iterator end() const noexcept ;

            Data::const_iterator cbegin() const noexcept ;
            Data::const_iterator cend() const noexcept ;

            std::size_t size() const noexcept ;
            bool empty() const noexcept ;
            bool is_containing(KeyCode key) const ;

            bool operator==(const KeyLog& rhs) const ;
            bool operator==(KeyLog&& rhs) const ;
            bool operator==(const Data& rhsraw) const ;
            bool operator==(Data&& rhsraw) const ;

            bool operator!=(const KeyLog& rhs) const ;
            bool operator!=(KeyLog&& rhs) const ;
            bool operator!=(const Data& rhs) const ;
            bool operator!=(Data&& rhsw) const ;

            const KeyLog operator-(const KeyLog& rhs) const ;
            const KeyLog operator-(KeyLog&& rhs) const ;
            const KeyLog operator-(const Data& rhs) const ;
            const KeyLog operator-(Data&& rhs) const ;

            KeyLog& operator-=(const KeyLog& rhs) ;
            KeyLog& operator-=(KeyLog&& rhs) ;
            KeyLog& operator-=(const Data& rhs) ;
            KeyLog& operator-=(Data&& rhs) ;
        } ;
    }
}

#endif
