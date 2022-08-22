#include "resize_win.hpp"

#include "winutil.hpp"

#include "core/charlogger.hpp"
#include "core/keylgrbase.hpp"
#include "core/ntypelogger.hpp"
#include "core/settable.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"
#include "util/screen_metrics.hpp"
#include "util/string.hpp"

namespace
{
    using namespace vind ;

    //
    // Resize the width of a foreground window in a foreground monitor.
    // [1] Desired width
    // [2] Foreground Window Structure
    //
    inline void resize_in_monitor_width(
            LONG desired_width,
            const bind::ForegroundInfo& fginfo) {

        util::MonitorInfo minfo ;
        util::get_monitor_metrics(fginfo.hwnd, minfo) ;

        auto window_height = fginfo.rect.height() ;
        auto monitor_width = minfo.work_rect.width() ;

        if(desired_width >= monitor_width) {
            //a maximum width is a width of a foreground monitor.
            bind::resize_window(
                    fginfo.hwnd,
                    minfo.work_rect.left(), fginfo.rect.top(),
                    monitor_width, window_height) ;
        }
        else {
            //If cannot move a lower-right point anymore, it move a upper-left point.
            auto right_max_width = minfo.work_rect.right() - fginfo.rect.left() ;

            if(desired_width <= right_max_width) {
                bind::resize_window(
                        fginfo.hwnd, fginfo.rect.left(), fginfo.rect.top(),
                        desired_width, window_height) ;
            }
            else {
                auto left_shift_delta = desired_width - right_max_width ;
                bind::resize_window(
                        fginfo.hwnd, fginfo.rect.left() - left_shift_delta, fginfo.rect.top(),
                        desired_width, window_height) ;
            }
        }
    }

    // Resize the height of a foreground window in a foreground monitor.
    // [1] Desired height
    // [2] Foreground Window Structure
    //
    inline void resize_in_monitor_height(
            LONG desired_height,
            const bind::ForegroundInfo& fginfo) {

        util::MonitorInfo minfo ;
        util::get_monitor_metrics(fginfo.hwnd, minfo) ;

        auto window_width = fginfo.rect.width() ;
        auto monitor_height = minfo.work_rect.height() ;

        if(desired_height >= monitor_height) {
            //a maximum height is a height of a foreground monitor.
            bind::resize_window(
                    fginfo.hwnd,
                    fginfo.rect.left(), minfo.work_rect.top(),
                    window_width, monitor_height) ;
        }
        else {
            //If cannot move a lower-right point anymore, it move a upper-left point.
            auto lower_max_height = minfo.work_rect.bottom() - fginfo.rect.top() ;

            if(desired_height <= lower_max_height) {
                bind::resize_window(
                        fginfo.hwnd, fginfo.rect.left(), fginfo.rect.top(),
                        window_width, desired_height) ;
            }
            else {
                auto left_shift_delta = desired_height - lower_max_height ;
                bind::resize_window(
                        fginfo.hwnd, fginfo.rect.left(), fginfo.rect.top() - left_shift_delta,
                        window_width, desired_height) ;
            }
        }
    }
}

namespace vind
{
    namespace bind
    {
        //ResizeWindowWidth
        ResizeWindowWidth::ResizeWindowWidth()
        : BindedFuncVoid("resize_window_width")
        {}
        void ResizeWindowWidth::sprocess(
                std::uint16_t count, const std::string& args) {
            if(args.empty()) {
                return ;
            }
            auto width = util::extract_num(args) ;
            ForegroundInfo fginfo ;
            resize_in_monitor_width(width, fginfo) ;
        }

        //IncreaseWindowWidth
        IncreaseWindowWidth::IncreaseWindowWidth()
        : BindedFuncVoid("increase_window_width")
        {}
        void IncreaseWindowWidth::sprocess(
                std::uint16_t count, const std::string& args) {
            long delta = 0 ;
            if(args.empty()) {
                auto& settable = core::SetTable::get_instance() ;
                delta = count * settable.get("window_hdelta").get<long>() ;
            }
            else {
                delta = util::extract_num(args) ;
            }

            ForegroundInfo fginfo ;
            auto width = fginfo.rect.width() + delta ;
            resize_in_monitor_width(width, fginfo) ;
        }

        //DecreaseWindowWidth
        DecreaseWindowWidth::DecreaseWindowWidth()
        : BindedFuncVoid("decrease_window_width")
        {}
        void DecreaseWindowWidth::sprocess(
                std::uint16_t count, const std::string& args) {
            long delta = 0 ;
            if(args.empty()) {
                auto& settable = core::SetTable::get_instance() ;
                delta = count * settable.get("window_hdelta").get<long>() ;
            }
            else {
                delta = util::extract_num(args) ;
            }

            ForegroundInfo fginfo ;

            auto width = fginfo.rect.width() - delta ;
            if(width <= 0) { 
                opt::VCmdLine::print(opt::ErrorMessage("E: Width below zero")) ;
                return ;
            }

            resize_window(
                    fginfo.hwnd,
                    fginfo.rect.left(), fginfo.rect.top(),
                    width, fginfo.rect.height()) ;
        }

        //ResizeWindowHeight
        ResizeWindowHeight::ResizeWindowHeight()
        : BindedFuncVoid("resize_window_height")
        {}
        void ResizeWindowHeight::sprocess(
                std::uint16_t count, const std::string& args) {
            if(args.empty()) {
                return ;
            }
            auto height = util::extract_num(args) ;
            ForegroundInfo fginfo ;
            resize_in_monitor_height(height, fginfo) ;
        }

        //IncreaseWindowHeight
        IncreaseWindowHeight::IncreaseWindowHeight()
        : BindedFuncVoid("increase_window_height")
        {}
        void IncreaseWindowHeight::sprocess(
                std::uint16_t count, const std::string& args) {
            long delta = 0 ;
            if(args.empty()) {
                auto& settable = core::SetTable::get_instance() ;
                delta = count * settable.get("window_vdelta").get<long>() ;
            }
            else {
                delta = util::extract_num(args) ;
            }

            ForegroundInfo fginfo ;
            auto height = fginfo.rect.height() + delta ;
            resize_in_monitor_height(height, fginfo) ;
        }

        //DecreaseWindowHeight
        DecreaseWindowHeight::DecreaseWindowHeight()
        : BindedFuncVoid("decrease_window_height")
        {}
        void DecreaseWindowHeight::sprocess(
                std::uint16_t count, const std::string& args) {
            long delta = 0 ;
            if(args.empty()) {
                auto& settable = core::SetTable::get_instance() ;
                delta = count * settable.get("window_vdelta").get<long>() ;
            }
            else {
                delta = util::extract_num(args) ;
            }

            ForegroundInfo fginfo ;
            auto height = fginfo.rect.height() - delta ;
            if(height <= 0) { 
                opt::VCmdLine::print(opt::ErrorMessage("E: Height below zero")) ;
                return ;
            }

            resize_window(
                    fginfo.hwnd,
                    fginfo.rect.left(), fginfo.rect.top(),
                    fginfo.rect.width(), height) ;
        }
    }
}
