#include "minmax_win.hpp"

#include "bind/saferepeat.hpp"
#include "core/charlogger.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind
    {
        //MinimizeCurrentWindow
        MinimizeCurrentWindow::MinimizeCurrentWindow()
        : BindedFuncVoid("minimize_current_window")
        {}
        void MinimizeCurrentWindow::sprocess(unsigned int count) {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LWIN, KEYCODE_DOWN) ;
            }) ;
        }
        void MinimizeCurrentWindow::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MinimizeCurrentWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //MaximizeCurrentWindow
        MaximizeCurrentWindow::MaximizeCurrentWindow()
        : BindedFuncVoid("maximize_current_window")
        {}
        void MaximizeCurrentWindow::sprocess(unsigned int count) {
            auto& igate = core::InputGate::get_instance() ;

            if(count == 1) {
                auto hwnd = util::get_foreground_window() ;

                auto before_rect = util::get_window_rect(hwnd) ;

                igate.pushup(KEYCODE_LWIN, KEYCODE_UP) ;
                Sleep(50) ;

                auto after_rect = util::get_window_rect(hwnd) ;

                //If not changed, regard it as a full screen and deal with it.
                if(util::is_equal(before_rect, after_rect)) {
                    igate.pushup(KEYCODE_LWIN, KEYCODE_DOWN) ;
                    igate.pushup(KEYCODE_LWIN, KEYCODE_UP) ;
                }
            }
            else {
                safe_for(count, [&igate] {
                    igate.pushup(KEYCODE_LWIN, KEYCODE_UP) ;
                }) ;
            }
        }
        void MaximizeCurrentWindow::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MaximizeCurrentWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
