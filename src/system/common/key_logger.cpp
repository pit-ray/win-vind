#include "key_logger.hpp"

#include <iostream>

#include "key_absorber.hpp"
#include "vkc_converter.hpp"
#include "utility.hpp"
#include "virtual_key_fwd.hpp"
#include "keystroke_repeater.hpp"

using namespace std ;

struct KeyLogger::Impl
{
    static KeyLog past_log ;

    data_t logs ;
    KeyStrokeRepeater ksr ;
    bool cmd_changed ;

    explicit Impl()
    : logs(),
      ksr(),
      cmd_changed(true)
    {}

    virtual ~Impl() noexcept {
        logs.clear() ;
    }

    Impl(Impl&&)                    = delete ;
    Impl& operator=(Impl&&)         = delete ;
    Impl(const Impl&)               = delete ;
    Impl& operator=(const Impl&)    = delete ;
} ;

KeyLog KeyLogger::Impl::past_log{} ;

KeyLogger::KeyLogger()
: pimpl(make_unique<Impl>())
{}

KeyLogger::~KeyLogger() noexcept = default ;

std::size_t KeyLogger::size() const noexcept
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

void KeyLogger::remove_from_top(const std::size_t num_from_top)
{
    if(pimpl->logs.empty()) return ;
    pimpl->logs.erase(pimpl->logs.begin(), pimpl->logs.begin() + num_from_top) ;
}

void KeyLogger::remove_from_back(std::size_t num_from_back)
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

const KeyLog KeyLogger::back() const
{
    if(pimpl->logs.empty()) {
        return std::move(KeyLog{}) ;
    }

    return pimpl->logs.back() ;
}

const KeyLog KeyLogger::at(const std::size_t i) const
{
    return pimpl->logs.at(i) ;
}

bool KeyLogger::is_changed_code()
{
    auto log = KeyAbsorber::get_pressed_list() ;
    pimpl->logs.push_back(log) ;

    if(pimpl->past_log == log) {
        return false ;
    }

    pimpl->past_log = std::move(log) ; //update past log
    return true ;
}

//regard inputed key log as ascii charactor
bool KeyLogger::is_changed_char()
{
    auto log = KeyAbsorber::get_pressed_list() ;

    if(pimpl->past_log != log) { //changed
        pimpl->cmd_changed = true ;
        const auto diff = log - pimpl->past_log ;
        pimpl->past_log = log ;

        if(!log.is_containing(VKC_SHIFT)) {
            pimpl->logs.push_back(std::move(diff)) ;
            return true ;
        }
        //shfited
        auto data = diff.get() ;
        data.insert(VKC_SHIFT) ;
        if(log.is_containing(VKC_LSHIFT)) data.insert(VKC_LSHIFT) ;
        if(log.is_containing(VKC_RSHIFT)) data.insert(VKC_RSHIFT) ;

        //construct KeyLog inside logs directly from std::vector
        pimpl->logs.emplace_back(std::move(data)) ;
        return true ;
    }
    else { //unchanged
        pimpl->logs.push_back(log) ;

        if(pimpl->cmd_changed) {
            pimpl->cmd_changed = false ;
            pimpl->ksr.reset() ;
            return false ;
        }
        return pimpl->ksr.is_pressed() ;
    }
}

const string KeyLogger::get_as_str() const
{
    string str{} ;
    if(pimpl->logs.empty()) {
        return str ;
    }

    for(const auto& log : pimpl->logs) {
        if(log.is_containing(VKC_SHIFT)) {
            //shifted ascii
            for(const auto vkc : log) {
                const auto c = VKCConverter::get_shifted_ascii(vkc) ;
                if(c != 0) str.push_back(c) ;
            }
            continue ;
        }
        for(const auto vkc : log) {
            const auto c = VKCConverter::get_ascii(vkc) ;
            if(c != 0) str.push_back(c) ;
        }
    }
    return str ;
}
