#include "min_max_window.hpp"

#include "io/keybrd.hpp"
#include "utility.hpp"

namespace vind
{
    //MinimizeCurrentWindow
    const std::string MinimizeCurrentWindow::sname() noexcept {
        return "minimize_current_window" ;
    }

    void MinimizeCurrentWindow::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        keybrd::pushup(KEYCODE_LWIN, KEYCODE_DOWN) ;
    }

    //MaximizeCurrentWindow
    const std::string MaximizeCurrentWindow::sname() noexcept {
        return "maximize_current_window" ;
    }

    void MaximizeCurrentWindow::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_LWIN, KEYCODE_UP) ;
        }
    }
}
