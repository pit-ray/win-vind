#include "search_pattern.hpp"
#include "keybrd_eventer.hpp"

#include <iostream>
using namespace std ;

//SearchPattern
const string SearchPattern::sname() noexcept
{
    return "search_pattern" ;
}

void SearchPattern::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(first_call) KeybrdEventer::pushup(VKC_F3) ;
}
