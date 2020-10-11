#ifndef _KEY_MATCHER_HPP
#define _KEY_MATCHER_HPP

#include <memory>
#include <vector>

class KeyLogger ;

class KeyMatcher
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    using keyset_t = std::vector<unsigned char> ;
    using cmd_t = std::vector<keyset_t> ;
    using cmdlist_t = std::vector<cmd_t> ;
    using shp_t = std::shared_ptr<KeyMatcher> ;

    explicit KeyMatcher(cmdlist_t&& keyset) ;
    virtual ~KeyMatcher() noexcept ;

    //return most matched quantity of key in log
    unsigned int compare2latestlog(const KeyLogger& logger) const ;
    bool is_callable() const noexcept ;

    KeyMatcher(KeyMatcher&&) ;
    KeyMatcher& operator=(KeyMatcher&&) ;

    KeyMatcher(const KeyMatcher&)               = delete ;
    KeyMatcher& operator=(const KeyMatcher&)    = delete ;

} ;


#endif
