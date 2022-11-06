#include "split_win.hpp"

#include "winutil.hpp"

#include "bind/proc/openwin.hpp"
#include "util/def.hpp"
#include "util/screen_metrics.hpp"
#include "util/winwrap.hpp"

namespace vind
{
    namespace bind
    {
        //OpenNewWindowWithHorizontalSplit
        OpenNewWindowWithHorizontalSplit::OpenNewWindowWithHorizontalSplit()
        : BindedFuncVoid("open_new_window_with_hsplit")
        {}
        void OpenNewWindowWithHorizontalSplit::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            ForegroundInfo fginfo ;

            auto w = fginfo.rect.width() ;
            auto h = fginfo.rect.height() ;

            //snap a original window to top
            resize_window(
                    fginfo.hwnd,
                    fginfo.rect.left(), fginfo.rect.top(),
                    w, h / 2) ;
            OpenNewWindow::sprocess(1, "") ;

            auto new_hwnd = util::get_foreground_window() ;
            if(new_hwnd == fginfo.hwnd) {
                Sleep(500) ;
                new_hwnd = util::get_foreground_window() ;
            }

            //snap a new window to bottom
            resize_window(
                    new_hwnd,
                    fginfo.rect.left(), fginfo.rect.top() + h / 2,
                    w, h / 2) ;
        }

        //OpenNewWindowWithVerticalSplit
        OpenNewWindowWithVerticalSplit::OpenNewWindowWithVerticalSplit()
        : BindedFuncVoid("open_new_window_with_vsplit")
        {}
        void OpenNewWindowWithVerticalSplit::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            ForegroundInfo fginfo ;

            auto w = fginfo.rect.width() ;
            auto h = fginfo.rect.height() ;

            //snap a original window to left
            resize_window(
                    fginfo.hwnd,
                    fginfo.rect.left(), fginfo.rect.top(),
                    w / 2, h) ;
            OpenNewWindow::sprocess(1, "") ;

            auto new_hwnd = util::get_foreground_window() ;
            if(new_hwnd == fginfo.hwnd) {
                Sleep(500) ;
                new_hwnd = util::get_foreground_window() ;
            }

            //snap a new window to right
            resize_window(
                    new_hwnd,
                    fginfo.rect.left() + w / 2, fginfo.rect.top(),
                    w / 2, h) ;
        }
    }
}
