#include "split_open_window.hpp"

#include "window_utility.hpp"

#include "bind/proc/open_window.hpp"
#include "core/ntypelogger.hpp"
#include "util/def.hpp"
#include "util/screen_metrics.hpp"

namespace vind
{
    namespace bind
    {
        //OpenNewWindowWithHorizontalSplit
        OpenNewWindowWithHorizontalSplit::OpenNewWindowWithHorizontalSplit()
        : BindedFuncVoid("open_new_window_with_hsplit")
        {}

        void OpenNewWindowWithHorizontalSplit::sprocess() {
            ForegroundInfo fginfo ;

            auto w = fginfo.rect.width() ;
            auto h = fginfo.rect.height() ;

            //snap a original window to top
            resize_window(
                    fginfo.hwnd,
                    fginfo.rect.left(), fginfo.rect.top(),
                    w, h / 2) ;
            OpenNewWindow::sprocess() ;

            auto new_hwnd = GetForegroundWindow() ;
            if(new_hwnd == NULL) {
                throw RUNTIME_EXCEPT("There is not the foreground window.") ;
            }
            if(new_hwnd == fginfo.hwnd) {
                Sleep(500) ;
                new_hwnd = GetForegroundWindow() ;
            }

            //snap a new window to bottom
            resize_window(
                    new_hwnd,
                    fginfo.rect.left(), fginfo.rect.top() + h / 2,
                    w, h / 2) ;
        }
        void OpenNewWindowWithHorizontalSplit::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void OpenNewWindowWithHorizontalSplit::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //OpenNewWindowWithVerticalSplit
        OpenNewWindowWithVerticalSplit::OpenNewWindowWithVerticalSplit()
        : BindedFuncVoid("open_new_window_with_vsplit")
        {}
        void OpenNewWindowWithVerticalSplit::sprocess() {
            ForegroundInfo fginfo ;

            auto w = fginfo.rect.width() ;
            auto h = fginfo.rect.height() ;

            //snap a original window to left
            resize_window(
                    fginfo.hwnd,
                    fginfo.rect.left(), fginfo.rect.top(),
                    w / 2, h) ;
            OpenNewWindow::sprocess() ;

            auto new_hwnd = GetForegroundWindow() ;
            if(new_hwnd == NULL) {
                throw RUNTIME_EXCEPT("There is not the foreground window.") ;
            }
            if(new_hwnd == fginfo.hwnd) {
                Sleep(500) ;
                new_hwnd = GetForegroundWindow() ;
            }

            //snap a new window to right
            resize_window(
                    new_hwnd,
                    fginfo.rect.left() + w / 2, fginfo.rect.top(),
                    w / 2, h) ;
        }
        void OpenNewWindowWithVerticalSplit::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void OpenNewWindowWithVerticalSplit::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
