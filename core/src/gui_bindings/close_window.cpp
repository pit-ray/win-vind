#include "close_window.hpp"

#include "keybrd_eventer.hpp"
#include "utility.hpp"

//CloseCurrentWindow
const std::string CloseCurrentWindow::sname() noexcept
{
    return "close_current_window" ;
}

void CloseCurrentWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(first_call) {
        KeybrdEventer::pushup(VKC_LALT, VKC_F4) ;
    }
}
