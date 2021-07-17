#include "bind/window/split_open_window.hpp"

#include "bind/proc/open_window.hpp"
#include "bind/window/window_utility.hpp"
#include "io/screen_metrics.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"

namespace vind
{
    //OpenNewWindowWithHorizontalSplit
    OpenNewWindowWithHorizontalSplit::OpenNewWindowWithHorizontalSplit()
    : BindedFuncCreator("open_new_window_with_hsplit")
    {}

    void OpenNewWindowWithHorizontalSplit::sprocess() {
        windowutil::ForegroundInfo fginfo ;

        auto w = fginfo.rect.width() ;
        auto h = fginfo.rect.height() ;

        //snap a original window to top
        windowutil::resize(
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
        windowutil::resize(
                new_hwnd,
                fginfo.rect.left(), fginfo.rect.top() + h / 2,
                w, h / 2) ;
    }
    void OpenNewWindowWithHorizontalSplit::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void OpenNewWindowWithHorizontalSplit::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //OpenNewWindowWithVerticalSplit
    OpenNewWindowWithVerticalSplit::OpenNewWindowWithVerticalSplit()
    : BindedFuncCreator("open_new_window_with_vsplit")
    {}
    void OpenNewWindowWithVerticalSplit::sprocess() {
        windowutil::ForegroundInfo fginfo ;

        auto w = fginfo.rect.width() ;
        auto h = fginfo.rect.height() ;

        //snap a original window to left
        windowutil::resize(
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
        windowutil::resize(
                new_hwnd,
                fginfo.rect.left() + w / 2, fginfo.rect.top(),
                w / 2, h) ;
    }
    void OpenNewWindowWithVerticalSplit::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void OpenNewWindowWithVerticalSplit::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
