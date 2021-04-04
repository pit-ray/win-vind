#include "bind/window/reload_window.hpp"

#include "io/keybrd.hpp"
#include "util/def.hpp"

namespace vind
{
    //ReloadCurrentWindow
    const std::string ReloadCurrentWindow::sname() noexcept {
        return "reload_current_window" ;
    }

    void ReloadCurrentWindow::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            keybrd::pushup(KEYCODE_F5) ;
        }
    }
}
