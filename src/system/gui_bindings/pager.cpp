#include "pager.hpp"
#include "keybrd_eventer.hpp"

using namespace std ;

//Move2NextPage
const string Move2NextPage::sname() noexcept
{
    return "move_to_next_page" ;
}

bool Move2NextPage::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_LALT, VKC_RIGHT) ;
}


//Move2PrevPage
const string Move2PrevPage::sname() noexcept
{
    return "move_to_prev_page" ;
}

bool Move2PrevPage::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_LALT, VKC_LEFT) ;
}
