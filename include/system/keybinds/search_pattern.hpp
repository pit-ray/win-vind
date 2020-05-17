#ifndef _SEARCH_PATTERN_HPP
#define _SEARCH_PATTERN_HPP

#include "binded_function_with_creator.hpp"

struct SearchPattern : public BindedFunctionWithCreator<SearchPattern>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


#endif