#include "split_open_window.hpp"

#include "io/screen_metrics.hpp"
#include "proc/open_window.hpp"
#include "utility.hpp"
#include "window_utility.hpp"

namespace vind
{
    //OpenNewCurWinWithHorizontalSplit
    const std::string OpenNewCurWinWithHorizontalSplit::sname() noexcept {
        return "open_new_current_window_with_hsplit" ;
    }

    void OpenNewCurWinWithHorizontalSplit::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        windowutil::ForegroundInfo fginfo ;

        const auto w = screen::width(fginfo.rect) ;
        const auto h = screen::height(fginfo.rect) ;

        //snap a original window to top
        windowutil::resize(
                fginfo.hwnd,
                fginfo.rect.left, fginfo.rect.top,
                w, h / 2) ;
        OpenNewCurrentWindow::sprocess(true, 1, nullptr, nullptr) ;

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


    //OpenNewCurWinWithVerticalSplit
    const std::string OpenNewCurWinWithVerticalSplit::sname() noexcept {
        return "open_new_current_window_with_vsplit" ;
    }

    void OpenNewCurWinWithVerticalSplit::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        windowutil::ForegroundInfo fginfo ;

        const auto w = screen::width(fginfo.rect) ;
        const auto h = screen::height(fginfo.rect) ;

        //snap a original window to left
        windowutil::resize(
                fginfo.hwnd,
                fginfo.rect.left, fginfo.rect.top,
                w / 2, h) ;
        OpenNewCurrentWindow::sprocess(true, 1, nullptr, nullptr) ;

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
}
