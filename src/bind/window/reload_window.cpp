#include "reload_window.hpp"

#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"

namespace vind
{
    namespace bind
    {
        //ReloadCurrentWindow
        ReloadCurrentWindow::ReloadCurrentWindow()
        : BindedFuncVoid("reload_current_window")
        {}
        void ReloadCurrentWindow::sprocess() {
            util::pushup(KEYCODE_F5) ;
        }
        void ReloadCurrentWindow::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void ReloadCurrentWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
