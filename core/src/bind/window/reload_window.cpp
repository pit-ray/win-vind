#include "bind/window/reload_window.hpp"

#include "bind/base/ntype_logger.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

namespace vind
{
    //ReloadCurrentWindow
    const std::string ReloadCurrentWindow::sname() noexcept {
        return "reload_current_window" ;
    }
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
