#include "switch_vdesktop.hpp"
#include "keybrd_eventer.hpp"

using namespace std ;

//SwitchVDesktop2Left
const string SwitchVDesktop2Left::sname() noexcept
{
    return "switch_vdesktop_to_left" ;
}

bool SwitchVDesktop2Left::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_LWIN, VKC_LEFT)) {
        return false ;
    }
    return true ;
}


//SwitchVDesktop2Right
const string SwitchVDesktop2Right::sname() noexcept
{
    return "switch_vdesktop_to_right" ;
}

bool SwitchVDesktop2Right::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_LWIN, VKC_RIGHT)) {
        return false ;
    }
    return true ;
}