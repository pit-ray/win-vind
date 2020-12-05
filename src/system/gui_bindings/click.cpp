#include "click.hpp"
#include "mouse_eventer.hpp"
#include "mode_manager.hpp"
#include <windows.h>
#include <iostream>

using namespace std ;

//ClickLeft
const string ClickLeft::sname() noexcept
{
    return "click_left" ;
}

void ClickLeft::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    using namespace ModeManager ;
    if(get_mode() == Mode::Visual) {
        change_mode(Mode::Normal) ;
    }

    for(unsigned int i = 0 ; i < repeat_num ; i ++)
        MouseEventer::click(VKC_MOUSE_LEFT) ;
}


//ClickRight
const string ClickRight::sname() noexcept
{
    return "click_right" ;
}

void ClickRight::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    using namespace ModeManager ;
    if(get_mode() == Mode::Visual) {
        change_mode(Mode::Normal) ;
    }

    for(unsigned int i = 0 ; i < repeat_num ; i ++)
        MouseEventer::click(VKC_MOUSE_RIGHT) ;
}
