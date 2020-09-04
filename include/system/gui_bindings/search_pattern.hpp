#ifndef _SEARCH_PATTERN_HPP
#define _SEARCH_PATTERN_HPP

#include "key_binding_with_creator.hpp"

struct SearchPattern : public KeyBindingWithCreator<SearchPattern>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


#endif