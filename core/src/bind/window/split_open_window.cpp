#include "bind/window/split_open_window.hpp"

#include "io/screen_metrics.hpp"
#include "bind/proc/open_window.hpp"
#include "util/def.hpp"
#include "bind/window/window_utility.hpp"
#include "bind/base/ntype_logger.hpp"

namespace vind
{
    //OpenNewCurWinWithHorizontalSplit
    OpenNewCurWinWithHorizontalSplit::OpenNewCurWinWithHorizontalSplit()
    : BindedFuncCreator("open_new_current_window_with_hsplit")
    {}

    void OpenNewCurWinWithHorizontalSplit::sprocess() {
        windowutil::ForegroundInfo fginfo ;

        const auto w = screenmetrics::width(fginfo.rect) ;
        const auto h = screenmetrics::height(fginfo.rect) ;

        //snap a original window to top
        windowutil::resize(
                fginfo.hwnd,
                fginfo.rect.left, fginfo.rect.top,
                w, h / 2) ;
        OpenNewCurrentWindow::sprocess() ;

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
                fginfo.rect.left, fginfo.rect.top + h / 2,
                w, h / 2) ;
    }
    void OpenNewCurWinWithHorizontalSplit::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void OpenNewCurWinWithHorizontalSplit::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //OpenNewCurWinWithVerticalSplit
    OpenNewCurWinWithVerticalSplit::OpenNewCurWinWithVerticalSplit()
    : BindedFuncCreator("open_new_current_window_with_vsplit")
    {}
    void OpenNewCurWinWithVerticalSplit::sprocess() {
        windowutil::ForegroundInfo fginfo ;

        const auto w = screenmetrics::width(fginfo.rect) ;
        const auto h = screenmetrics::height(fginfo.rect) ;

        //snap a original window to left
        windowutil::resize(
                fginfo.hwnd,
                fginfo.rect.left, fginfo.rect.top,
                w / 2, h) ;
        OpenNewCurrentWindow::sprocess() ;

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
                fginfo.rect.left + w / 2, fginfo.rect.top,
                w / 2, h) ;
    }
    void OpenNewCurWinWithVerticalSplit::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void OpenNewCurWinWithVerticalSplit::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
