#ifndef _SEARCH_PATTERN_HPP
#define _SEARCH_PATTERN_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct SearchPattern : public BindedFuncVoid<SearchPattern> {
            explicit SearchPattern() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
