#include "bind/dev/mybindings.hpp"

#include "opt/virtual_cmd_line.hpp"
#include "io/mouse.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

namespace vind
{
    const std::string MyBinding::sname() noexcept {
        return "my_binding" ; //Give the unique identifier.
    }

    void MyBinding::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            mouse::click(KEYCODE_MOUSE_LEFT) ; //left click
        
            keybrd::pushup(KEYCODE_LWIN, KEYCODE_D) ; //minimize all window
        
            VirtualCmdLine::msgout("Hello World !") ;
        }
    }
}
