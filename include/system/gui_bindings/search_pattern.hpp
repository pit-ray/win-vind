#ifndef _SEARCH_PATTERN_HPP
#define _SEARCH_PATTERN_HPP

#include "binded_func_with_creator.hpp"

struct SearchPattern : public BindedFuncWithCreator<SearchPattern>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;


#endif
