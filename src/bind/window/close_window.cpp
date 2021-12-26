#include "close_window.hpp"

#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "util/def.hpp"

namespace vind
{
    namespace bind
    {
        //CloseCurrentWindow
        CloseCurrentWindow::CloseCurrentWindow()
        : BindedFuncVoid("close_current_window")
        {}
        void CloseCurrentWindow::sprocess() {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LALT, KEYCODE_F4) ;
        }
        void CloseCurrentWindow::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void CloseCurrentWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
