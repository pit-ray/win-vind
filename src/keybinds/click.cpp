#include "click.hpp"
#include "mouse_eventer.hpp"
#include <windows.h>
#include <iostream>

using namespace std ;

//ClickLeft
const string ClickLeft::sname() noexcept
{
    return "click_left" ;
}

bool ClickLeft::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return MouseEventer::is_click(MouseEventer::Button::LEFT) ;
}


//ClickRight
const string ClickRight::sname() noexcept
{
    return "click_right" ;
}

bool ClickRight::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return MouseEventer::is_click(MouseEventer::Button::RIGHT) ;
}