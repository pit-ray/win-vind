#include "switch_taskview.hpp"
#include "keybrd_eventer.hpp"

using namespace std ;

//SwitchTaskview2Left
const string SwitchTaskview2Left::sname() noexcept
{
    return "switch_taskview_to_left" ;
}

bool SwitchTaskview2Left::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_LWIN, VKC_LEFT)) {
        return false ;
    }
    return true ;
}


//SwitchTaskview2Right
const string SwitchTaskview2Right::sname() noexcept
{
    return "switch_taskview_to_right" ;
}

bool SwitchTaskview2Right::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_LWIN, VKC_RIGHT)) {
        return false ;
    }
    return true ;
}