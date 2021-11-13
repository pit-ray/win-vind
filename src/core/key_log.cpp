#include "key_log.hpp"

#include <algorithm>

namespace vind
{
    namespace core
    {
        struct KeyLog::Impl {
            Data log_ ;
            explicit Impl()
            : log_()
            {}

            virtual ~Impl() noexcept         = default ;
            explicit Impl(const Impl& rhs)   = default ;
            Impl& operator=(const Impl& rhs) = default ;
            explicit Impl(Impl&& rhs)        = default ;
            Impl& operator=(Impl&& rhs)      = default ;

            explicit Impl(const Data& codes)
            : log_(codes)
            {}

            explicit Impl(Data&& codes)
            : log_(codes)
            {}

            explicit Impl(std::initializer_list<KeyCode>&& codes)
            : log_(codes)
            {}

            template <typename T>
            KeyLog::Data erased_diff(T&& rhs) const {
                auto diff = log_ ;
                for(auto& k : rhs) diff.erase(k) ;
                return diff ;
            }
        } ;

        KeyLog::KeyLog()
        : pimpl(std::make_unique<Impl>())
        {}

        KeyLog::KeyLog(const Data& codes)
        : pimpl(std::make_unique<Impl>(codes))
        {}

        KeyLog::KeyLog(Data&& codes)
            : pimpl(std::make_unique<Impl>(std::move(codes)))
        {}

        KeyLog::KeyLog(std::initializer_list<KeyCode>&& codes)
        : pimpl(std::make_unique<Impl>(std::move(codes)))
        {}

        KeyLog::~KeyLog() noexcept          = default ;

        KeyLog::KeyLog(const KeyLog& rhs)
        : pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
        {}

        KeyLog::KeyLog(KeyLog&&)            = default ;
        KeyLog& KeyLog::operator=(KeyLog&&) = default ;

        KeyLog& KeyLog::operator=(KeyLog::Data&& rhs) {
            pimpl->log_ = std::move(rhs) ;
            return *this ;
        }

        KeyLog& KeyLog::operator=(const KeyLog& rhs) {
            if(rhs.pimpl) *pimpl = *rhs.pimpl ;
            return *this ;
        }

        KeyLog& KeyLog::operator=(const KeyLog::Data& rhs) {
            pimpl->log_ = rhs ;
            return *this ;
        }

        const KeyLog::Data& KeyLog::get() const & noexcept {
            return pimpl->log_ ;
        }

        KeyLog::Data&& KeyLog::get() && noexcept {
            return std::move(pimpl->log_) ;
        }

        KeyLog::Data::const_iterator KeyLog::begin() const noexcept {
            return pimpl->log_.begin() ;
        }

        KeyLog::Data::const_iterator KeyLog::end() const noexcept {
            return pimpl->log_.end() ;
        }

        KeyLog::Data::const_iterator KeyLog::cbegin() const noexcept {
            return pimpl->log_.cbegin() ;
        }

        KeyLog::Data::const_iterator KeyLog::cend() const noexcept {
            return pimpl->log_.cend() ;
        }

        std::size_t KeyLog::size() const noexcept {
            return pimpl->log_.size() ;
        }

        bool KeyLog::empty() const noexcept {
            return pimpl->log_.empty() ;
        }

        bool KeyLog::is_containing(KeyCode key) const
        {
            return pimpl->log_.find(key) != pimpl->log_.end() ;
        }

        //equal
        bool KeyLog::operator==(const KeyLog& rhs) const {
            if(!(rhs.pimpl)) return false ;
            return pimpl->log_ == rhs.pimpl->log_ ;
        }
        bool KeyLog::operator==(KeyLog&& rhs) const {
            if(!(rhs.pimpl)) return false ;
            return pimpl->log_ == rhs.pimpl->log_ ;
        }
        bool KeyLog::operator==(const KeyLog::Data& rhs) const {
            return pimpl->log_ == rhs ;
        }
        bool KeyLog::operator==(KeyLog::Data&& rhs) const {
            return pimpl->log_ == rhs ;
        }

        //not equal
        bool KeyLog::operator!=(const KeyLog& rhs) const {
            if(!(rhs.pimpl)) return false ; //moved
            return pimpl->log_ != rhs.pimpl->log_ ;
        }
        bool KeyLog::operator!=(KeyLog&& rhs) const {
            if(!(rhs.pimpl)) return false ; //moved
            return pimpl->log_ != rhs.pimpl->log_ ;
        }
        bool KeyLog::operator!=(const KeyLog::Data& rhs) const {
            return pimpl->log_ != rhs ;
        }
        bool KeyLog::operator!=(KeyLog::Data&& rhs) const {
            return pimpl->log_ != rhs ;
        }

        //sub
        const KeyLog KeyLog::operator-(const KeyLog& rhs) const {
            if(!(rhs.pimpl)) return *this ;
            return KeyLog(pimpl->erased_diff(rhs.pimpl->log_)) ;
        }
        const KeyLog KeyLog::operator-(KeyLog&& rhs) const {
            if(!(rhs.pimpl)) return *this ;
            return KeyLog(pimpl->erased_diff(std::move(rhs.pimpl->log_))) ;
        }
        const KeyLog KeyLog::operator-(const KeyLog::Data& rhs) const {
            return KeyLog(pimpl->erased_diff(rhs)) ;
        }
        const KeyLog KeyLog::operator-(KeyLog::Data&& rhs) const {
            return KeyLog(pimpl->erased_diff(std::move(rhs))) ;
        }

        //sub assign
        KeyLog& KeyLog::operator-=(const KeyLog& rhs) {
            if(rhs.pimpl) {
                for(const auto& k : rhs) pimpl->log_.erase(k) ;
            }
            return *this ;
        }
        KeyLog& KeyLog::operator-=(KeyLog&& rhs) {
            if(rhs.pimpl) {
                for(const auto& k : rhs) pimpl->log_.erase(k) ;
            }
            return *this ;
        }
        KeyLog& KeyLog::operator-=(const KeyLog::Data& rhs) {
            for(const auto& k : rhs) pimpl->log_.erase(k) ;
            return *this ;
        }
        KeyLog& KeyLog::operator-=(KeyLog::Data&& rhs) {
            for(const auto& k : rhs) pimpl->log_.erase(k) ;
            return *this ;
        }
    }
}
