#include "mybinding.hpp"
#include "virtual_cmd_line.hpp"

namespace vind
{
    const std::string MyBinding::sname() noexcept {
        return "my_binding" ; //Give the unique identifier.
    }

    void MyBinding::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const parent_vkclgr,
            const CharLogger* const parent_charlgr) {
        if(first_call) {
            mouse::click(VKC_MOUSE_LEFT) ; //left click
        
            keybrd::pushup(VKC_LWIN, VKC_D) ; //minimize all window
        
            VirtualCmdLine::msgout("Hello World !") ;
        }
    }
}
