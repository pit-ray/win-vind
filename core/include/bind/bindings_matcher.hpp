#ifndef _BINDINGS_MATCHER_HPP
#define _BINDINGS_MATCHER_HPP

#include <memory>
#include <vector>

#include "key/key_log.hpp"

namespace vind
{
    class KeyLoggerBase ;

    class BindingsMatcher {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        unsigned int compare_onelog(const KeyLog& log, size_t seqidx) const ;

    public:
        using keyset_t = std::vector<unsigned char> ;
        using cmd_t = std::vector<keyset_t> ;
        using cmdlist_t = std::vector<cmd_t> ;
        using shp_t = std::shared_ptr<BindingsMatcher> ;

        explicit BindingsMatcher(cmdlist_t&& keyset) ;
        explicit BindingsMatcher(const cmdlist_t& keyset) ;
        virtual ~BindingsMatcher() noexcept ;

        //return most matched quantity of key in log
        unsigned int compare_to_latestlog(const KeyLoggerBase& lgr) const ;
        unsigned int compare_to_alllog(const KeyLoggerBase& lgr) const ;
        bool is_accepted() const noexcept ;

        BindingsMatcher(BindingsMatcher&&) ;
        BindingsMatcher& operator=(BindingsMatcher&&) ;

        BindingsMatcher(const BindingsMatcher&)               = delete ;
        BindingsMatcher& operator=(const BindingsMatcher&)    = delete ;
    } ;
}

#endif
