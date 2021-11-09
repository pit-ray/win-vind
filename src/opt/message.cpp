#include "opt/message.hpp"


namespace vind
{
    namespace opt
    {
        struct Message::Impl {
            std::string str_ ;
            MessageFlag flags_ ;
            std::chrono::system_clock::time_point timestamp_ ;

            template <typename T>
            Impl(T&& str, MessageFlag flags)
            : str_(std::forward<T>(str)),
              flags_(flags),
              timestamp_(std::chrono::system_clock::now())
            {}

            Impl()
            : Impl("", MessageFlag::NONE)
            {}
        } ;

        Message::Message(const std::string& str, MessageFlag flags)
        : pimpl(std::make_unique<Impl>(str, flags))
        {}
        Message::Message(std::string&& str, MessageFlag flags)
        : pimpl(std::make_unique<Impl>(str, flags))
        {}

        Message::~Message() noexcept               = default ;
        Message::Message(Message&& rhs)            = default ;
        Message& Message::operator=(Message&& rhs) = default ;

        Message::Message(const Message& rhs)
        : pimpl(rhs.pimpl ?
                std::make_unique<Impl>(rhs.pimpl->str_, rhs.pimpl->flags_) :
                std::make_unique<Impl>())
        {}
        Message& Message::operator=(const Message& rhs) {
            if(rhs.pimpl) {
                *pimpl = *(rhs.pimpl) ;
            }
            return *this ;
        }

        std::chrono::system_clock::duration Message::lifetime() const noexcept {
            return std::chrono::system_clock::now() - pimpl->timestamp_ ;
        }

        bool Message::fadeoutable() const noexcept {
            return (pimpl->flags_ & MessageFlag::FADING) != 0 ;
        }

        bool Message::dumpable() const noexcept {
            return (pimpl->flags_ & MessageFlag::DUMPING) != 0 ;
        }

        unsigned char Message::priority() const noexcept {
            return pimpl->flags_ & MessageFlag::PRIORITY_MASK ;
        }

        void Message::clear() noexcept {
            pimpl->flags_ = MessageFlag::NONE ;
            pimpl->str_.clear() ;
            pimpl->timestamp_ = std::chrono::system_clock::time_point() ;
        }

        bool Message::empty() const noexcept {
            return pimpl->str_.empty() ;
        }

        std::size_t Message::size() const noexcept {
            return pimpl->str_.size() ;
        }

        void Message::set(const std::string& str) {
            pimpl->str_ = str ;
        }

        void Message::set(std::string&& str) {
            pimpl->str_ = std::move(str) ;
        }

        Message::operator std::string() const noexcept {
            return pimpl->str_ ;
        }

        const Message Message::operator+(const std::string& rhs) const {
            return Message(pimpl->str_ + rhs) ;
        }
        const Message Message::operator+(std::string&& rhs) const {
            return Message(pimpl->str_ + rhs) ;
        }

        Message& Message::operator+=(const std::string& rhs) {
            pimpl->str_ += rhs ;
            return *this ;
        }
        Message& Message::operator+=(std::string&& rhs) {
            pimpl->str_ += rhs ;
            return *this ;
        }

        Message& Message::operator<<(const std::string& rhs) {
            pimpl->str_ += rhs ;
            return *this ;
        }
        Message& Message::operator<<(std::string&& rhs) {
            pimpl->str_ += rhs ;
            return *this ;
        }
    }
}
