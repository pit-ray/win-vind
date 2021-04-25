#include "key/key_log.hpp"

#include <algorithm>

namespace vind
{
    struct KeyLog::Impl {
        Data once_log ;
        explicit Impl()
        : once_log()
        {}

        virtual ~Impl() noexcept         = default ;
        explicit Impl(const Impl& rhs)   = default ;
        Impl& operator=(const Impl& rhs) = default ;
        explicit Impl(Impl&& rhs)        = default ;
        Impl& operator=(Impl&& rhs)      = default ;

        explicit Impl(const Data& codes)
        : once_log(codes)
        {}

        explicit Impl(Data&& codes)
        : once_log(codes)
        {}

        explicit Impl(std::initializer_list<unsigned char>&& codes)
        : once_log(codes)
        {}

        template <typename T>
        KeyLog::Data erased_diff(T&& rhs) const {
            auto diff = once_log ;
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

    KeyLog::KeyLog(std::initializer_list<unsigned char>&& codes)
    : pimpl(std::make_unique<Impl>(std::move(codes)))
    {}

    KeyLog::~KeyLog() noexcept          = default ;
    KeyLog::KeyLog(KeyLog&&)            = default ;
    KeyLog& KeyLog::operator=(KeyLog&&) = default ;

    KeyLog::KeyLog(const KeyLog& rhs)
    : pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
    {}

    KeyLog& KeyLog::operator=(const KeyLog& rhs) {
        if(rhs.pimpl) *pimpl = *rhs.pimpl ;
        return *this ;
    }

    const KeyLog::Data& KeyLog::get() const & noexcept {
        return pimpl->once_log ;
    }

    KeyLog::Data&& KeyLog::get() && noexcept {
        return std::move(pimpl->once_log) ;
    }

    KeyLog::Data::const_iterator KeyLog::begin() const noexcept {
        return pimpl->once_log.begin() ;
    }

    KeyLog::Data::const_iterator KeyLog::end() const noexcept {
        return pimpl->once_log.end() ;
    }

    KeyLog::Data::const_iterator KeyLog::cbegin() const noexcept {
        return pimpl->once_log.cbegin() ;
    }

    KeyLog::Data::const_iterator KeyLog::cend() const noexcept {
        return pimpl->once_log.cend() ;
    }

    std::size_t KeyLog::size() const noexcept {
        return pimpl->once_log.size() ;
    }

    bool KeyLog::empty() const noexcept {
        return pimpl->once_log.empty() ;
    }

    bool KeyLog::is_containing(unsigned char key) const
    {
        return pimpl->once_log.find(key) != pimpl->once_log.end() ;
    }

    //equel
    bool KeyLog::operator==(const KeyLog& rhs) const {
        if(!(rhs.pimpl)) return false ;
        return pimpl->once_log == rhs.pimpl->once_log ;
    }
    bool KeyLog::operator==(KeyLog&& rhs) const {
        if(!(rhs.pimpl)) return false ;
        return pimpl->once_log == rhs.pimpl->once_log ;
    }
    bool KeyLog::operator==(const KeyLog::Data& rhs) const {
        return pimpl->once_log == rhs ;
    }
    bool KeyLog::operator==(KeyLog::Data&& rhs) const {
        return pimpl->once_log == rhs ;
    }

    //not equel
    bool KeyLog::operator!=(const KeyLog& rhs) const {
        if(!(rhs.pimpl)) return false ; //moved
        return pimpl->once_log != rhs.pimpl->once_log ;
    }
    bool KeyLog::operator!=(KeyLog&& rhs) const {
        if(!(rhs.pimpl)) return false ; //moved
        return pimpl->once_log != rhs.pimpl->once_log ;
    }
    bool KeyLog::operator!=(const KeyLog::Data& rhs) const {
        return pimpl->once_log != rhs ;
    }
    bool KeyLog::operator!=(KeyLog::Data&& rhs) const {
        return pimpl->once_log != rhs ;
    }

    //sub
    const KeyLog KeyLog::operator-(const KeyLog& rhs) const {
        if(!(rhs.pimpl)) return *this ;
        return KeyLog(pimpl->erased_diff(rhs.pimpl->once_log)) ;
    }
    const KeyLog KeyLog::operator-(KeyLog&& rhs) const {
        if(!(rhs.pimpl)) return *this ;
        return KeyLog(pimpl->erased_diff(std::move(rhs.pimpl->once_log))) ;
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
            for(const auto& k : rhs) pimpl->once_log.erase(k) ;
        }
        return *this ;
    }
    KeyLog& KeyLog::operator-=(KeyLog&& rhs) {
        if(rhs.pimpl) {
            for(const auto& k : rhs) pimpl->once_log.erase(k) ;
        }
        return *this ;
    }
    KeyLog& KeyLog::operator-=(const KeyLog::Data& rhs) {
        for(const auto& k : rhs) pimpl->once_log.erase(k) ;
        return *this ;
    }
    KeyLog& KeyLog::operator-=(KeyLog::Data&& rhs) {
        for(const auto& k : rhs) pimpl->once_log.erase(k) ;
        return *this ;
    }
}
