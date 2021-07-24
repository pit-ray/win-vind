#include "bind/window/arrange_window.hpp"

#include <windows.h>

#include <psapi.h>

#include <functional>
#include <map>
#include <sstream>
#include <unordered_map>

#include "bind/window/window_utility.hpp"
#include "err_logger.hpp"
#include "g_params.hpp"
#include "io/screen_metrics.hpp"
#include "key/ntype_logger.hpp"
#include "util/box_2d.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"


namespace
{
    using namespace vind ;

    std::unordered_map<HMONITOR, Box2D> g_mrects ;

    using OrderedHWNDs = std::map<SIZE_T, HWND> ;
    std::unordered_map<HMONITOR, OrderedHWNDs> g_m_ordered_hwnd ;

    std::unordered_set<std::string> g_ignores ;

    BOOL CALLBACK EnumWindowsProcForArrangement(HWND hwnd, LPARAM UNUSED(lparam)) {
        if(!windowutil::is_visible_hwnd(hwnd)) {
            return TRUE ; //continue
        }

        Box2D box ;
        if(!GetWindowRect(hwnd, &box.data())) {
            return TRUE ; //continue
        }

        if(!windowutil::is_window_mode(hwnd, box.data())) {
            return TRUE ; //continue
        }

        //Is existed in work area?
        screenmetrics::MonitorInfo minfo ;
        screenmetrics::get_monitor_metrics(hwnd, minfo) ;
        if(box.is_out_of(minfo.work_rect)) {
            return TRUE ;
        }

        // make a unique identifier and its priority
        DWORD proc_id = 0 ;
        GetWindowThreadProcessId(hwnd, &proc_id) ;

        HANDLE hproc = OpenProcess(
                PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                FALSE, proc_id) ;
        if(!hproc) {
            return TRUE ; //has not permission
        }

        PROCESS_MEMORY_COUNTERS pmc ;
        if(!GetProcessMemoryInfo(hproc, &pmc, sizeof(pmc))) {
            CloseHandle(hproc) ;
            std::stringstream ss ;
            ss << "Could not get the memory infomation of " << hwnd ;
            PRINT_ERROR(ss.str()) ;
            return FALSE ; //break
        }
        CloseHandle(hproc) ;

        //a key is unique and described the priority.
        auto& ordered_hwnd = g_m_ordered_hwnd[minfo.hmonitor] ;
        ordered_hwnd[pmc.WorkingSetSize + ordered_hwnd.size()] = hwnd ;

        g_mrects[minfo.hmonitor] = std::move(minfo.work_rect) ;

        return TRUE ;
    }

    inline void assign_local_area_in_monitors(std::unordered_map<HWND, Box2D>& rects) {
        for(auto& [hmonitor, mrect] : g_mrects) {
            const auto& ordered_hwnd = g_m_ordered_hwnd[hmonitor] ;

            //Its priority is the highest (based on the memory use).
            auto itr = ordered_hwnd.crbegin() ;

            auto pre_hwnd = itr->second ;
            rects[pre_hwnd] = mrect ;

            itr ++ ;

            while(itr != ordered_hwnd.crend()) {
                const auto hwnd = itr->second ;

                auto& pre_rect = rects[pre_hwnd] ;
                auto rect = pre_rect ;

                auto pre_w = pre_rect.width() ;
                auto pre_h = pre_rect.height() ;
                if(pre_w > pre_h) {
                    pre_rect.right() -= pre_rect.center_y() ;
                    rect.left()      += pre_rect.center_x() ;
                }
                else {
                    pre_rect.bottom() -= pre_h / 2 ;
                    rect.top()        += pre_h / 2 ;
                }
                rects[hwnd] = std::move(rect) ;

                pre_hwnd = hwnd ;
                itr ++ ;
            }
        }
    }
}

namespace vind
{
    //ArrangeWindows
    ArrangeWindows::ArrangeWindows()
    : BindedFuncCreator("arrange_windows")
    {}
    void ArrangeWindows::sprocess() {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("There is not a foreground window.") ;
        }

        //Search visible windows
        g_m_ordered_hwnd.clear() ;
        g_mrects.clear() ;

        if(!EnumWindows(EnumWindowsProcForArrangement, 0)) {
            throw RUNTIME_EXCEPT("Could not enumerate all top-level windows on the screen.") ;
        }

        if(g_m_ordered_hwnd.empty() || g_mrects.empty()) {
            return ;
        }

        if(!g_ignores.empty()) {
            for(const auto& monitor : g_mrects) {
                auto& hwnds = g_m_ordered_hwnd[monitor.first] ;

                auto itr = hwnds.begin() ;
                while(itr != hwnds.end()) {
                    auto filename = util::get_module_filename(itr->second) ;
                    auto modulename = util::A2a(filename.substr(
                                filename.find_last_of("\\") + 1)) ;

                    auto dot = modulename.find_last_of(".") ;
                    if(dot != std::string::npos) {
                        modulename = modulename.substr(0, dot) ;
                    }

                    if(g_ignores.find(modulename) != g_ignores.end()) {
                        hwnds.erase(itr) ;
                    }

                    itr ++ ;
                }
            }
        }

        std::unordered_map<HWND, Box2D> rects ;
        assign_local_area_in_monitors(rects) ;
        windowutil::batch_resize(rects) ;

        if(!SetForegroundWindow(hwnd)) {
            throw RUNTIME_EXCEPT("Could not set the foreground window.") ;
        }
    }
    void ArrangeWindows::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void ArrangeWindows::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
    void ArrangeWindows::reconstruct() {
        g_ignores.clear() ;
        auto str = gparams::get_s("arrangewin_ignore") ;

        auto modules = util::split(str, ",") ;
        for(auto& m : modules) {
            g_ignores.insert(util::A2a(util::trim(m))) ;
        }
    }
}
