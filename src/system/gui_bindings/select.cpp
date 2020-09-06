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
    if(!MouseEventer::click(VKC_MOUSE_LEFT)) {
        return false ;
    }
    if(!KeybrdEventer::pushup(VKC_LCTRL, VKC_A)) {
        return false ;
    }
    return true ;
}