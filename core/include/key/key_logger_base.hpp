#ifndef _KEY_LOGGER_BASE_HPP
#define _KEY_LOGGER_BASE_HPP

#include <memory>
#include <vector>
#include "key/key_log.hpp"

namespace vind
{
    class KeyLoggerBase {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        using Data = std::vector<KeyLog> ;

        explicit KeyLoggerBase() ;
        virtual ~KeyLoggerBase() noexcept ;

        KeyLoggerBase(const KeyLoggerBase&) ;
        KeyLoggerBase& operator=(const KeyLoggerBase&) ;

        KeyLoggerBase(KeyLoggerBase&&) ;
        KeyLoggerBase& operator=(KeyLoggerBase&&) ;

        Data::iterator begin() noexcept ;
        Data::iterator end() noexcept ;

        Data::const_iterator begin() const noexcept ;
        Data::const_iterator end() const noexcept ;

        Data::const_iterator cbegin() const noexcept ;
        Data::const_iterator cend() const noexcept ;

        Data::reference latest() ;
        Data::const_reference latest() const ;

        std::size_t size() const noexcept ;
        bool empty() const noexcept ;

        virtual void clear() noexcept ;

        virtual void logging(const KeyLog& log) ;
        virtual void logging(KeyLog&& log) ;

        virtual void logging(const KeyLog::Data& lograw) ;
        virtual void logging(KeyLog::Data&& lograw) ;

        void remove_from_back(std::size_t num_from_back) ;

        Data::const_reference at(std::size_t n) const ;

        bool operator==(const KeyLoggerBase& rhs) const ;
        bool operator!=(const KeyLoggerBase& rhs) const ;

        virtual int logging_state() = 0 ;
    } ;

    namespace keyloggerutil {
        // Extracts a number from a string.
        // Its range is from the appearance of the number to a non-numeric character.
        // Example:
        //      std::string: hBah0784hlk503
        //      -> std::stoi(0784)
        //      uint: 784
        //
        unsigned int extract_num(const std::string& str) ;

        namespace debug {
            //for debug
            std::string print_log(const KeyLoggerBase& lgr) ;
        }
    }
}

#endif
