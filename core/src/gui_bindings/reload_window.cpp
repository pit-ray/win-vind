#include "reload_window.hpp"

#include "keybrd_eventer.hpp"
#include "utility.hpp"

namespace vind
{
    //ReloadCurrentWindow
    const std::string ReloadCurrentWindow::sname() noexcept {
        return "reload_current_window" ;
    }

    void ReloadCurrentWindow::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            KeybrdEventer::pushup(VKC_F5) ;
        }
    }
}
