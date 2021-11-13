#if defined(DEBUG)

#include "mybindings.hpp"

#include "bind/binded_func_creator.hpp"
#include "core/ntype_logger.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"
#include "util/mouse.hpp"

namespace vind
{
    MyBinding::MyBinding()
    : BindedFuncCreator("my_binding") //Give the unique identifier.
    {}
    void MyBinding::sprocess() {
        util::click(KEYCODE_MOUSE_LEFT) ; //left click

        util::pushup(KEYCODE_LWIN, KEYCODE_D) ; //minimize all window

        opt::VCmdLine::print(opt::GeneralMessage("Hello World !")) ;
    }
    void MyBinding::sprocess(core::NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void MyBinding::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}

#endif
