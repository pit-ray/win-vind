#include "bind/window/close_window.hpp"

#include "bind/base/ntype_logger.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

namespace vind
{
    //CloseCurrentWindow
    CloseCurrentWindow::CloseCurrentWindow()
    : BindedFuncCreator("close_current_window")
    {}
    void CloseCurrentWindow::sprocess() {
        keybrd::pushup(KEYCODE_LALT, KEYCODE_F4) ;
    }
    void CloseCurrentWindow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void CloseCurrentWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
