#ifndef _KEY_MATCHER_HPP
#define _KEY_MATCHER_HPP

#include <memory>
#include <vector>

#include "key_logger.hpp"

class KeyMatcher
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    unsigned int compare_onelog(const KeyLog& log, const size_t seqidx) const ;

public:
    using keyset_t = std::vector<unsigned char> ;
    using cmd_t = std::vector<keyset_t> ;
    using cmdlist_t = std::vector<cmd_t> ;
    using shp_t = std::shared_ptr<KeyMatcher> ;

    explicit KeyMatcher(cmdlist_t&& keyset) ;
    explicit KeyMatcher(const cmdlist_t& keyset) ;
    virtual ~KeyMatcher() noexcept ;

    //return most matched quantity of key in log
    unsigned int compare_to_latestlog(const KeyLogger& logger) const ;
    unsigned int compare_to_alllog(const KeyLogger& logger) const ;
    bool is_accepted() const noexcept ;

    KeyMatcher(KeyMatcher&&) ;
    KeyMatcher& operator=(KeyMatcher&&) ;

    KeyMatcher(const KeyMatcher&)               = delete ;
    KeyMatcher& operator=(const KeyMatcher&)    = delete ;

} ;


#endif
