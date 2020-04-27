#include "select.hpp"
#include "keybrd_eventer.hpp"
#include "mouse_eventer.hpp"

using namespace std ;

//SelectAll
const string SelectAll::sname() noexcept
{
    return "select_all" ;
}

bool SelectAll::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(!MouseEventer::is_click(MouseEventer::Button::LEFT)) {
        return false ;
    }

    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_A)) {
        return false ;
    }

    return true ;
}