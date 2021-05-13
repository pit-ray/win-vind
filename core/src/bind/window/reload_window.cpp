#include "bind/window/reload_window.hpp"

#include "io/keybrd.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"

namespace vind
{
    //ReloadCurrentWindow
    ReloadCurrentWindow::ReloadCurrentWindow()
    : BindedFuncCreator("reload_current_window")
    {}
    void ReloadCurrentWindow::sprocess() {
        keybrd::pushup(KEYCODE_F5) ;
    }
    void ReloadCurrentWindow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void ReloadCurrentWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
