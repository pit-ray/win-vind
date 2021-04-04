#include "mybinding.hpp"
#include "virtual_cmd_line.hpp"

namespace vind
{
    const std::string MyBinding::sname() noexcept {
        return "my_binding" ; //Give the unique identifier.
    }

    void MyBinding::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const parent_keycodelgr,
            const CharLogger* const parent_charlgr) {
        if(first_call) {
            mouse::click(KEYCODE_MOUSE_LEFT) ; //left click
        
            keybrd::pushup(KEYCODE_LWIN, KEYCODE_D) ; //minimize all window
        
            VirtualCmdLine::msgout("Hello World !") ;
        }
    }
}
