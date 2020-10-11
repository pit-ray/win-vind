#include "key_matcher.hpp"

#include <mutex>
#include <iostream>

#include "Key_logger.hpp"
#include "virtual_key_fwd.hpp"

#define OPTIONAL_MAX_INDEX  (99999999999)

struct KeyMatcher::Impl
{
    cmdlist_t cmdlist ;
    bool code_existed ;
    bool callable ;
    std::size_t optional_index ;
    std::mutex mtx ;

    explicit Impl(cmdlist_t&& cl)
    : cmdlist(std::move(cl)),
      code_existed(true),
      callable(false),
      optional_index(OPTIONAL_MAX_INDEX),
      mtx()
    {}
    virtual ~Impl() noexcept {
        cmdlist.clear() ;
    }

    Impl(Impl&&)            = default ;
    Impl& operator=(Impl&&) = default ;
    Impl(const Impl&)            = delete ;
    Impl& operator=(const Impl&) = delete ;
} ;

KeyMatcher::KeyMatcher(KeyMatcher::cmdlist_t&& keyset)
: pimpl(std::make_unique<Impl>(std::move(keyset)))
{}

KeyMatcher::~KeyMatcher() noexcept                  = default ;
KeyMatcher::KeyMatcher(KeyMatcher&&)                = default ;
KeyMatcher& KeyMatcher::operator=(KeyMatcher&&)     = default ;

unsigned int KeyMatcher::compare2latestlog(const KeyLogger& logger) const
{
    std::lock_guard<std::mutex> lock(pimpl->mtx) ;

    pimpl->callable = false ;
    if(pimpl->cmdlist.empty() || logger.is_empty()) {
        return 0 ;
    }

    const auto seq_index = logger.size() - 1 ;
    if(seq_index == 0) {
        pimpl->code_existed = true ;
    }

    if(seq_index < pimpl->optional_index) {
        pimpl->optional_index = OPTIONAL_MAX_INDEX ;
    }
    else {
        pimpl->callable     = true ;
        pimpl->code_existed = true ;
        return 0x9999 ;
    }

    if(!pimpl->code_existed) { //The search is not needed anymore.
        return 0 ;
    }

    unsigned int most_matched_num = 0 ;
    auto at_least_exist = false ;

    for(const auto& cmd : pimpl->cmdlist) { 
        try {
            std::size_t matched_num = 0 ;
            const auto& keyset = cmd.at(seq_index) ;

            for(const auto& key : keyset) {
                if(key == VKC_OPTIONAL) {
                    pimpl->optional_index = seq_index ;
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
            if(seq_index == (cmd.size() - 1)) {
                pimpl->callable = true ;
            }
        }
        catch(const std::out_of_range&) {
            continue ;
        }
        catch(const unsigned char code) {
            pimpl->callable     = true ;
            pimpl->code_existed = true ;
            return 0xffff ;
        }
    }

    pimpl->code_existed = at_least_exist ;
    return most_matched_num ;
}

bool KeyMatcher::is_callable() const noexcept
{
    return pimpl->callable ;
}
