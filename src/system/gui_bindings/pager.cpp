#include "pager.hpp"
#include "keybrd_eventer.hpp"
#include "utility.hpp"

using namespace std ;

//Move2NextPage
const string Move2NextPage::sname() noexcept
{
    return "move_to_next_page" ;
}

void Move2NextPage::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(first_call) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++)
            KeybrdEventer::pushup(VKC_LALT, VKC_RIGHT) ;
    }
}


//Move2PrevPage
const string Move2PrevPage::sname() noexcept
{
    return "move_to_prev_page" ;
}

void Move2PrevPage::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(first_call) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++)
            KeybrdEventer::pushup(VKC_LALT, VKC_LEFT) ;
    }
}
