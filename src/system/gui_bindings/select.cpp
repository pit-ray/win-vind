#include "keybrd_eventer.hpp"
#include "mouse_eventer.hpp"
#include "select.hpp"
#include "utility.hpp"

using namespace std ;

//SelectAll
const string SelectAll::sname() noexcept
{
    return "select_all" ;
}

void SelectAll::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(first_call) {
        MouseEventer::click(VKC_MOUSE_LEFT) ;
        KeybrdEventer::pushup(VKC_LCTRL, VKC_A) ;
    }
}
