#include "close_window.hpp"

#include "io/keybrd.hpp"
#include "utility.hpp"

namespace vind
{
    //CloseCurrentWindow
    const std::string CloseCurrentWindow::sname() noexcept {
        return "close_current_window" ;
    }

    void CloseCurrentWindow::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            keybrd::pushup(KEYCODE_LALT, KEYCODE_F4) ;
        }
    }
}
