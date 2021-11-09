#ifndef _MESSAGE_HPP
#define _MESSAGE_HPP

#include <chrono>
#include <memory>
#include <string>


namespace vind
{
    namespace opt
    {
        enum MessageFlag : unsigned char {
            NONE          = 0b0000'0000,
            FADING        = 0b1000'0000,
            DUMPING       = 0b0100'0000,
            PRIORITY_MASK = 0x0f,
        } ;

        class Message {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit Message(
                    const std::string& str,
                    MessageFlag flags=MessageFlag::NONE) ;

            explicit Message(
                    std::string&& str="",
                    MessageFlag flags=MessageFlag::NONE) ;

            virtual ~Message() noexcept ;

            Message(Message&&) ;
            Message& operator=(Message&&) ;

            Message(const Message&) ;
            Message& operator=(const Message&) ;

            std::chrono::system_clock::duration lifetime() const noexcept ;
            bool fadeoutable() const noexcept ;
            bool dumpable() const noexcept ;
            unsigned char priority() const noexcept ;

            void dump() const noexcept ;

            void clear() noexcept ;

            bool empty() const noexcept ;

            std::size_t size() const noexcept ;

            void set(const std::string& str) ;
            void set(std::string&& str) ;

            operator std::string() const noexcept ;

            const Message operator+(const std::string& rhs) const ;
            const Message operator+(std::string&& rhs) const ;

            Message& operator+=(const std::string& rhs) ;
            Message& operator+=(std::string&& rhs) ;

            Message& operator<<(const std::string& rhs) ;
            Message& operator<<(std::string&& rhs) ;
        } ;
    }
}

#endif
