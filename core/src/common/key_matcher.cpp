#include "key_matcher.hpp"

#include <iostream>
#include <limits>
#include <mutex>

#include "msg_logger.hpp"
#include "virtual_key_fwd.hpp"
#include "vkc_converter.hpp"
#include "key_log.hpp"

struct KeyMatcher::Impl
{
    cmdlist_t cmdlist ;
    bool code_existed ;
    bool accepted ;
    std::size_t optional_idx ;
    std::size_t optnum_begin_idx ;
    std::size_t optnum_end_idx ;
    bool is_optnum_last ;
    std::mutex mtx ;

    explicit Impl(cmdlist_t&& cl)
    : cmdlist(std::move(cl)),
      code_existed(true),
      accepted(false),
      optional_idx(std::numeric_limits<std::size_t>::max()),
      optnum_begin_idx(std::numeric_limits<std::size_t>::max()),
      optnum_end_idx(std::numeric_limits<std::size_t>::max()),
      is_optnum_last(false),
      mtx()
    {}

    explicit Impl(const cmdlist_t& cl)
    : cmdlist(cl),
      code_existed(true),
      accepted(false),
      optional_idx(std::numeric_limits<std::size_t>::max()),
      optnum_begin_idx(std::numeric_limits<std::size_t>::max()),
      optnum_end_idx(std::numeric_limits<std::size_t>::max()),
      is_optnum_last(false),
      mtx()
    {}

    virtual ~Impl() noexcept {
        cmdlist.clear() ;
    }

    Impl(Impl&&)                 = delete ;
    Impl& operator=(Impl&&)      = delete ;
    Impl(const Impl&)            = delete ;
    Impl& operator=(const Impl&) = delete ;
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
unsigned int KeyMatcher::compare_onelog(const KeyLog& log, size_t seqidx) const
{
    //mutex is already applied in a parent scope

    if(seqidx >= pimpl->optional_idx) {
        pimpl->accepted     = true ;
        pimpl->code_existed = true ;

        return std::numeric_limits<unsigned int>::max() ;
    }

    //Whether there are some numbers in the latest log?
    auto is_num_only = [&log] {
        for(auto& key : log) {
            if(!VKCConverter::is_number(key)) {
                return false ;
            }
        }
        return true ;
    } ;

    //If the latest log is in a range of <num>?
    if(pimpl->optnum_begin_idx <= seqidx && seqidx <= pimpl->optnum_end_idx) {
        if(is_num_only()) {
            pimpl->code_existed = true ;
            pimpl->accepted     = pimpl->is_optnum_last ;
            return 1 ;
        }
        pimpl->optnum_end_idx = seqidx - 1 ;
    }

    seqidx -= (pimpl->optnum_end_idx - pimpl->optnum_begin_idx) ;

    //
    // Ex)
    // command: AB<num>C
    //  ______________________________________________________________
    // |              |     |     |              |              |     |
    // |  latest_log  |  A  |  B  |       1      |      2       |  C  |
    // |--------------|-----|-----|--------------|--------------|-----|
    // |    seqidx    |  0  |  1  |       2      |      3       |  4  |
    // |--------------|-----|-----|--------------|--------------|-----|
    // | optnum_begin | max | max |       2      |      2       |  2  |
    // |--------------|-----|-----|--------------|--------------|-----|
    // | optnum_end   | max | max |      max     |     max      |  3  |
    // |--------------|-----|-----|--------------|--------------|-----|
    // | fixed seqidx |  0  |  1  |  2(in range) |  3(in range) |  3  |
    // |______________|_____|_____|______________|______________|_____|
    //

    unsigned int most_matched_num = 0 ;
    auto at_least_exist = false ;

    for(const auto& cmd : pimpl->cmdlist) { 
        try {
            unsigned int matched_num = 0 ;
            const auto& keyset = cmd.at(seqidx) ;

            for(const auto& key : keyset) {
                if(key == VKC_OPTIONAL) {
                    pimpl->optional_idx = seqidx ;
                    pimpl->accepted     = true ;
                    pimpl->code_existed = true ;
                    return std::numeric_limits<unsigned int>::max() ;
                }
                if(key == VKC_OPTNUMBER) {
                    if(is_num_only()) {
                        pimpl->optnum_begin_idx = seqidx ;
                        pimpl->is_optnum_last = (cmd.size() - seqidx - 1) == 0 ;
                        matched_num ++ ;
                        continue ;
                    }
                }
                if(log.is_containing(key)) {
                    matched_num ++ ;
                }
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
    if(seqidx == 0) {
        pimpl->code_existed = true ;
    }

    if(seqidx < pimpl->optional_idx) {
        pimpl->optional_idx = std::numeric_limits<std::size_t>::max() ;
    }

    if(seqidx < pimpl->optnum_begin_idx) {
        pimpl->is_optnum_last   = false ; 
        pimpl->optnum_begin_idx = std::numeric_limits<std::size_t>::max() ;
    }

    if(seqidx < pimpl->optnum_end_idx) {
        pimpl->optnum_end_idx = std::numeric_limits<std::size_t>::max() ;
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

    pimpl->code_existed     = true ;
    pimpl->optional_idx     = std::numeric_limits<std::size_t>::max() ;
    pimpl->is_optnum_last   = false ;
    pimpl->optnum_begin_idx = std::numeric_limits<unsigned int>::max() ;
    pimpl->optnum_end_idx   = std::numeric_limits<unsigned int>::max() ;

    unsigned int result = 0 ;
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
