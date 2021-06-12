#include "bind/dev/mybindings.hpp"

#include "bind/binded_func_creator.hpp"
#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "key/ntype_logger.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"

namespace vind
{
    MyBinding::MyBinding()
    : BindedFuncCreator("my_binding") //Give the unique identifier.
    {}
    void MyBinding::sprocess() {
        mouse::click(KEYCODE_MOUSE_LEFT) ; //left click

        keybrd::pushup(KEYCODE_LWIN, KEYCODE_D) ; //minimize all window

        VirtualCmdLine::msgout("Hello World !") ;
    }
    void MyBinding::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void MyBinding::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
