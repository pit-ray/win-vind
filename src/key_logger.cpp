#include "key_logger.hpp"
#include "key_absorber.hpp"
#include "vkc_converter.hpp"
#include "utility.hpp"
#include "virtual_key_fwd.hpp"

#include <iostream>

using namespace std ;

struct KeyLogger::Impl
{
    data_t logs ;
    KeyLog past_log ;

    explicit Impl() : logs(), past_log() {}

    ~Impl() noexcept {
        logs.clear() ;
    }

    Impl(Impl&&) = delete ;
    Impl& operator=(Impl&&) = delete ;
    Impl(const Impl&) = delete ;
    Impl& operator=(const Impl&) = delete ;
} ;


KeyLogger::KeyLogger() noexcept
: pimpl(make_unique<Impl>())
{}

KeyLogger::~KeyLogger() noexcept = default ;

size_t KeyLogger::size() const noexcept
{
    return pimpl->logs.size() ;
}

bool KeyLogger::is_empty() const noexcept
{
    return pimpl->logs.empty() ;
}

void KeyLogger::clear() noexcept
{
    pimpl->logs.clear() ;
}

void KeyLogger::remove_from_top(const std::size_t num_from_top) noexcept
{
    if(pimpl->logs.empty()) return ;
    pimpl->logs.erase(pimpl->logs.begin(), pimpl->logs.begin() + num_from_top) ;
}

void KeyLogger::remove_from_back(std::size_t num_from_back) noexcept
{
    if(pimpl->logs.empty()) return ;
    if(pimpl->logs.size() < num_from_back) {
        num_from_back = pimpl->logs.size() ;
    }
    pimpl->logs.erase(pimpl->logs.end() - num_from_back, pimpl->logs.end()) ;
}

KeyLogger::data_t::const_iterator KeyLogger::begin() const noexcept
{
    return pimpl->logs.begin() ;
}

KeyLogger::data_t::const_iterator KeyLogger::end() const noexcept
{
    return pimpl->logs.end() ;
}

KeyLogger::data_t::const_iterator KeyLogger::cbegin() const noexcept
{
    return pimpl->logs.cbegin() ;
}

KeyLogger::data_t::const_iterator KeyLogger::cend() const noexcept
{
    return pimpl->logs.cend() ;
}

const KeyLog KeyLogger::back() const noexcept
{
    if(pimpl->logs.empty()) {
        return std::move(KeyLog{}) ;
    }

    return pimpl->logs.back() ;
}

const KeyLog KeyLogger::at(const size_t i) const
{
    return pimpl->logs.at(i) ;
}

bool KeyLogger::is_changed_and_update()
{
    KeyLog log{KeyAbsorber::get_downed_list()} ;
    pimpl->logs.push_back(log) ;

    if(pimpl->past_log == log) {
        return false ;
    }

    pimpl->past_log = std::move(log) ; //update past log
    return true ;
}

//regard inputed key log as ascii charactor
bool KeyLogger::is_changed_and_inputc()
{
    KeyLog log{KeyAbsorber::get_downed_list()} ;

    const auto diff = log - pimpl->past_log ;

    const auto result = pimpl->past_log != log ;
    pimpl->past_log = log ;

    KeyLog::data_t data{} ;
    if(log.is_included(VKC_LSHIFT)) {
        data.push_back(VKC_LSHIFT) ;
    }

    if(log.is_included(VKC_RSHIFT)) {
        data.push_back(VKC_RSHIFT) ;
    }

    if(!data.empty()) {
        data.insert(data.end(), diff.cbegin(), diff.cend()) ;
        Utility::remove_deplication(data) ;
        pimpl->logs.push_back(KeyLog(std::move(data))) ;
        return result ;
    }

    pimpl->logs.push_back(std::move(diff)) ;
    return result ;
}

const string KeyLogger::get_str() const noexcept
{
    string str{} ;

    if(pimpl->logs.empty()) {
        return std::move(str + '\0') ;
    }

    for(const auto& log : pimpl->logs) {
        if(log.is_included(VKC_LSHIFT) || log.is_included(VKC_RSHIFT)) {
            //shifted ascii
            for(const auto&vkc : log) {
                const auto c = VKCConverter::get_shifted_ascii(vkc) ;
                if(c != 0) str.push_back(c) ;
            }
            continue ;
        }

        for(const auto& vkc : log) {
            const auto c = VKCConverter::get_ascii(vkc) ;
            if(c != 0) str.push_back(c) ;
        }
    }
    return str ;
}