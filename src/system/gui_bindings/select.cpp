#include "select.hpp"
#include "keybrd_eventer.hpp"
#include "mouse_eventer.hpp"

using namespace std ;

//SelectAll
const string SelectAll::sname() noexcept
{
    return "select_all" ;
}

void SelectAll::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(first_call) {
        MouseEventer::click(VKC_MOUSE_LEFT) ;
        KeybrdEventer::pushup(VKC_LCTRL, VKC_A) ;
    }
}
