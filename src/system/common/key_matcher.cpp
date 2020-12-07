#include "key_matcher.hpp"

#include <iostream>
#include <limits>
#include <mutex>

#include "msg_logger.hpp"
#include "virtual_key_fwd.hpp"

struct KeyMatcher::Impl
{
    cmdlist_t cmdlist ;
    bool code_existed ;
    bool accepted ;
    std::size_t optional_index ;
    std::mutex mtx ;

    explicit Impl(cmdlist_t&& cl)
    : cmdlist(std::move(cl)),
      code_existed(true),
      accepted(false),
      optional_index(std::numeric_limits<std::size_t>::max()),
      mtx()
    {}

    explicit Impl(const cmdlist_t& cl)
    : cmdlist(cl),
      code_existed(true),
      accepted(false),
      optional_index(std::numeric_limits<std::size_t>::max()),
      mtx()
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

/*
 * -- Key matching system like Automata --
 * [code_existed == true] state transitioned successfully
 * [accepted     == true] reached accepting state
 *
 */
unsigned int KeyMatcher::compare_onelog(const KeyLog& log, const size_t seqidx) const
{
    //mutex is applied

    if(seqidx >= pimpl->optional_index) {
        pimpl->accepted     = true ;
        pimpl->code_existed = true ;

        return std::numeric_limits<unsigned int>::max() ;
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
                if(log.is_containing(key))
                    matched_num ++ ;
            }

            //not matched, so search next command
            if(matched_num != keyset.size()) continue ;
            at_least_exist = true ;

            if(most_matched_num < matched_num)
                most_matched_num = matched_num ;

            if(seqidx == (cmd.size() - 1))
                pimpl->accepted = true ;
        }
        catch(const std::out_of_range&) {
            continue ;
        }
        catch(const unsigned char code) {
            pimpl->accepted    = true ;
            pimpl->code_existed = true ;
            return std::numeric_limits<unsigned int>::max() ;
        }
    }

    pimpl->code_existed = at_least_exist ;
    return most_matched_num ;

}

unsigned int KeyMatcher::compare_to_latestlog(const KeyLogger& logger) const
{
    std::lock_guard<std::mutex> lock(pimpl->mtx) ;

    pimpl->accepted = false ;

    if(logger.empty()) return 0 ;

    const auto seqidx = logger.size() - 1 ;
    if(seqidx == 0)
        pimpl->code_existed = true ;

    if(seqidx < pimpl->optional_index) {
        pimpl->optional_index = std::numeric_limits<std::size_t>::max() ;
    }

    //The search is not needed anymore.
    if(!pimpl->code_existed) return 0 ;

    return compare_onelog(logger.back(), seqidx) ;
}

unsigned int KeyMatcher::compare_to_alllog(const KeyLogger& logger) const
{
    std::lock_guard<std::mutex> lock(pimpl->mtx) ;

    pimpl->accepted = false ;

    if(logger.empty()) return 0 ;

    pimpl->code_existed   = true ;
    pimpl->optional_index = std::numeric_limits<std::size_t>::max() ;

    unsigned int result ;
    for(std::size_t i = 0 ; i < logger.size() ; i ++) {
        result = compare_onelog(logger[i], i) ;

        if(pimpl->accepted || !pimpl->code_existed) break ;
    }
    return result ;
}

bool KeyMatcher::is_accepted() const noexcept
{
    return pimpl->accepted ;
}
