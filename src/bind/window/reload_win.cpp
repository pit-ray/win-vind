#include "reload_win.hpp"

#include "core/inputgate.hpp"
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
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            core::InputGate::get_instance().pushup(KEYCODE_F5) ;
        }
    }
}
