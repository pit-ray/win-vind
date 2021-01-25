#include "mybinding.hpp"
#include "virtual_cmd_line.hpp"

const std::string MyBinding::sname() noexcept
{
    return "my_binding" ; //Give the unique identifier.
}

void MyBinding::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* parent_vkclgr,
        const KeyLogger* const parent_charlgr)
{
    if(first_call) {
        MouseEventer::click(VKC_MOUSE_LEFT) ; //left click
    
        KeybrdEventer::pushup(VKC_LWIN, VKC_D) ; //minimize all window
    
        VirtualCmdLine::msgout("Hello World !") ;
    }
}
