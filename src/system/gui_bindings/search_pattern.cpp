#include "search_pattern.hpp"
#include "keybrd_eventer.hpp"

#include <iostream>
using namespace std ;

//SearchPattern
const string SearchPattern::sname() noexcept
{
    return "search_pattern" ;
}

bool SearchPattern::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(!KeybrdEventer::pressup(VKC_F3)) {
        return false ;
    }
    return true ;
}