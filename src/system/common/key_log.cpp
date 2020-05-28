#include "key_log.hpp"

#include <algorithm>

using namespace std ;

struct KeyLog::Impl
{
    data_t once_log ;
    explicit Impl() noexcept
    : once_log()
    {}

    explicit Impl(const data_t& codes)
    : once_log(codes)
    {}

    explicit Impl(data_t&& codes)
    : once_log(codes)
    {}

    explicit Impl(initializer_list<unsigned char>&& codes)
    : once_log(codes)
    {}

    ~Impl() noexcept {
        once_log.clear() ;
    }

    Impl(Impl&&) = default ;
    Impl& operator=(Impl&&) = default ;

    Impl(const Impl&) = default ;
    Impl& operator=(const Impl&) =  default ;
} ;

KeyLog::KeyLog()
: pimpl(make_unique<Impl>())
{}

KeyLog::KeyLog(const data_t& codes)
: pimpl(make_unique<Impl>(codes))
{}

KeyLog::KeyLog(data_t&& codes)
: pimpl(make_unique<Impl>(std::move(codes)))
{}

KeyLog::KeyLog(initializer_list<unsigned char>&& codes)
: pimpl(make_unique<Impl>(std::move(codes)))
{}

KeyLog::~KeyLog() = default ;

//move
KeyLog::KeyLog(KeyLog&&) noexcept = default ;
KeyLog& KeyLog::operator=(KeyLog&&) noexcept = default ;

KeyLog::KeyLog(const KeyLog& rhs)
: pimpl(rhs.pimpl ? make_unique<Impl>(*rhs.pimpl) : make_unique<Impl>())
{}

KeyLog& KeyLog::operator=(const KeyLog& rhs) noexcept
{
    if(!rhs.pimpl) return *this ; //if already moved, not copy
    *pimpl = *rhs.pimpl ;
    return *this ;
}

const KeyLog::data_t& KeyLog::get() const & noexcept
{
    return pimpl->once_log ;
}

KeyLog::data_t&& KeyLog::get() && noexcept
{
    return std::move(pimpl->once_log) ;
}

KeyLog::data_t::const_iterator KeyLog::begin() const noexcept
{
    return pimpl->once_log.begin() ;
}

KeyLog::data_t::const_iterator KeyLog::end() const noexcept
{
    return pimpl->once_log.end() ;
}

KeyLog::data_t::const_iterator KeyLog::cbegin() const noexcept
{
    return pimpl->once_log.cbegin() ;
}

KeyLog::data_t::const_iterator KeyLog::cend() const noexcept
{
    return pimpl->once_log.cend() ;
}

unsigned char KeyLog::back() const noexcept
{
    return pimpl->once_log.back() ;
}

size_t KeyLog::size() const noexcept
{
    return pimpl->once_log.size() ;
}

bool KeyLog::is_empty() const noexcept
{
    return pimpl->once_log.empty() ;
}

bool KeyLog::is_including(const unsigned char key) const noexcept
{
    return find(pimpl->once_log.cbegin(),
        pimpl->once_log.cend(), key) != pimpl->once_log.cend() ;
}

bool KeyLog::operator==(const KeyLog& rhs) const noexcept
{
    if(!rhs.pimpl) return false ; //moved
    return pimpl->once_log == rhs.pimpl->once_log ;
}

bool KeyLog::operator!=(const KeyLog& rhs) const noexcept
{
    if(!rhs.pimpl) return false ; //moved
    return pimpl->once_log != rhs.pimpl->once_log ;
}

const KeyLog KeyLog::operator-(const KeyLog& rhs) const
{
    data_t diff{} ;
    set_difference(cbegin(), cend(), rhs.cbegin(), rhs.cend(), inserter(diff, diff.end())) ;

    return KeyLog(std::move(diff)) ;
}