#ifndef _SEARCH_PATTERN_HPP
#define _SEARCH_PATTERN_HPP

#include "bind/binded_func.hpp"

namespace vind
{
    namespace bind
    {
        struct SearchPattern : public BindedFuncVoid<SearchPattern> {
            explicit SearchPattern() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
