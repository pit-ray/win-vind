#include "key_matcher.hpp"

#include <iostream>
#include <limits>
#include <mutex>

#include "msg_logger.hpp"
#include "virtual_key_fwd.hpp"

struct KeyMatcher::Impl
{
    cmdlist_t cmdlist ;
    bool code_existed = true ;
    bool safisfied    = false ;
    std::size_t optional_index = std::numeric_limits<std::size_t>::max() ;
    std::mutex mtx ;

    explicit Impl(cmdlist_t&& cl)
    : cmdlist(std::move(cl))
    {}

    explicit Impl(const cmdlist_t& cl)
    : cmdlist(cl)
    {}

    virtual ~Impl() noexcept {
        cmdlist.clear() ;
    }
} ;

KeyMatcher::KeyMatcher(KeyMatcher::cmdlist_t&& keyset)
: pimpl(std::make_unique<Impl>(std::move(keyset)))
{}

KeyMatcher::KeyMatcher(const cmdlist_t& keyset)
: pimpl(std::make_unique<Impl>(keyset))
{}

KeyMatcher::~KeyMatcher() noexcept              = default ;
KeyMatcher::KeyMatcher(KeyMatcher&&)            = default ;
KeyMatcher& KeyMatcher::operator=(KeyMatcher&&) = default ;

unsigned int KeyMatcher::compare_to_latestlog(const KeyLogger& logger) const
{
    std::lock_guard<std::mutex> lock(pimpl->mtx) ;

    pimpl->safisfied = false ;
    if(logger.empty()) return 0 ;

    const auto seqidx = logger.size() - 1 ;
    if(seqidx == 0) {
        pimpl->code_existed = true ;
    }

    if(seqidx < pimpl->optional_index) {
        pimpl->optional_index = std::numeric_limits<std::size_t>::max() ;
    }
    else {
        pimpl->safisfied    = true ;
        pimpl->code_existed = true ;

        return std::numeric_limits<unsigned int>::max() ;
    }

    //The search is not needed anymore.
    if(!pimpl->code_existed) {
        return 0 ;
    }

    unsigned int most_matched_num = 0 ;
    auto at_least_exist = false ;

    for(const auto& cmd : pimpl->cmdlist) { 
        try {
            std::size_t matched_num = 0 ;
            const auto& keyset = cmd.at(seqidx) ;

            for(const auto& key : keyset) {
                if(key == VKC_OPTIONAL) {
                    pimpl->optional_index = seqidx ;
                    throw VKC_OPTIONAL ;
                }
                if(logger.back().is_containing(key)) {
                    matched_num ++ ;
                }
            }

            if(matched_num != keyset.size()) {
                continue ; //not matched, so search next command
            }
            at_least_exist = true ;

            if(most_matched_num < matched_num) {
                most_matched_num = matched_num ;
            }
            if(seqidx == (cmd.size() - 1)) {
                pimpl->safisfied = true ;
            }
        }
        catch(const std::out_of_range&) {
            continue ;
        }
        catch(const unsigned char code) {
            pimpl->safisfied    = true ;
            pimpl->code_existed = true ;
            return std::numeric_limits<unsigned int>::max() ;
        }
    }

    pimpl->code_existed = at_least_exist ;
    return most_matched_num ;
}

bool KeyMatcher::is_safisfied() const noexcept
{
    return pimpl->safisfied ;
}
