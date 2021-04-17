#ifndef _SEARCH_PATTERN_HPP
#define _SEARCH_PATTERN_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    struct SearchPattern : public BindedFuncCreator<SearchPattern> {
        explicit SearchPattern() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
