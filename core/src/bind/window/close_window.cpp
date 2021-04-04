#include "bind/window/close_window.hpp"

#include "io/keybrd.hpp"
#include "util/def.hpp"

namespace vind
{
    //CloseCurrentWindow
    const std::string CloseCurrentWindow::sname() noexcept {
        return "close_current_window" ;
    }

    void CloseCurrentWindow::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            keybrd::pushup(KEYCODE_LALT, KEYCODE_F4) ;
        }
    }
}
