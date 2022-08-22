#include "reload_win.hpp"

#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "util/def.hpp"

namespace vind
{
    namespace bind
    {
        //ReloadCurrentWindow
        ReloadCurrentWindow::ReloadCurrentWindow()
        : BindedFuncVoid("reload_current_window")
        {}
        void ReloadCurrentWindow::sprocess(
                std::uint16_t count, const std::string& args) {
            core::InputGate::get_instance().pushup(KEYCODE_F5) ;
        }
    }
}
