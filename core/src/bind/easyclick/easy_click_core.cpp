#include "bind/easyclick/easy_click_core.hpp"

#include <future>
#include <memory>

#include "bind/easyclick/display_hinter.hpp"
#include "bind/easyclick/ec_hints.hpp"
#include "bind/easyclick/input_hinter.hpp"
#include "bind/easyclick/ui_scanner.hpp"
#include "g_params.hpp"
#include "io/mouse.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "key/ntype_logger.hpp"
#include "opt/async_uia_cache_builder.hpp"
#include "util/container.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"
#include "util/winwrap.hpp"


namespace
{
    using namespace vind ;

    struct ProcessScanInfo {
        DWORD pid ;
        std::vector<Point2D>& points ;
    } ;

    BOOL CALLBACK ScanCenterPoint(HWND hwnd, LPARAM lparam) {
        auto obj_list = reinterpret_cast<std::vector<Point2D>*>(lparam) ;

        if(!IsWindowVisible(hwnd)) {
            return TRUE ;
        }
        if(!IsWindowEnabled(hwnd)) {
            return TRUE ;
        }

        //also register the position of the other thread window's title bar.
        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(util::width(rect) == 0 || util::height(rect) == 0) {
            return TRUE ;
        }
        if(rect.left < 0 || rect.top < 0 || rect.right < 0 || rect.bottom < 0) {
            return TRUE ;
        }

        obj_list->emplace_back(
                util::center_x(rect),
                util::center_y(rect)) ;
        return TRUE ;
    }

    BOOL CALLBACK EnumerateAllThread(HWND hwnd, LPARAM lparam) {
        auto psinfo = reinterpret_cast<ProcessScanInfo*>(lparam) ;

        DWORD procid ;
        auto thid = GetWindowThreadProcessId(hwnd, &procid) ;

        if(procid == psinfo->pid) {
            //enumerate all threads owned by the parent process.
            EnumThreadWindows(
                    thid, ScanCenterPoint,
                    reinterpret_cast<LPARAM>(&(psinfo->points))) ;
        }

        return TRUE ;
    }
}


namespace vind
{
    struct EasyClickCore::Impl {
        UIScanner scanner_{} ;
        std::vector<SmartElement> elements_{} ;
        std::vector<Point2D> positions_{} ;
        std::vector<Hint> hints_{} ;
        std::vector<std::string> strhints_{} ;
        InputHinter input_hinter_{} ;
        DisplayHinter display_hinter_{} ;
    } ;

    EasyClickCore::EasyClickCore()
    : pimpl(std::make_unique<Impl>())
    {
        pimpl->positions_.reserve(2048) ;
        pimpl->elements_.reserve(2048) ;
        pimpl->hints_.reserve(2048) ;
        pimpl->strhints_.reserve(2048) ;

        AsyncUIACacheBuilder::register_properties(
                pimpl->scanner_.get_properties()) ;
    }

    EasyClickCore::~EasyClickCore() noexcept = default ;

    EasyClickCore::EasyClickCore(EasyClickCore&&)            = default ;
    EasyClickCore& EasyClickCore::operator=(EasyClickCore&&) = default ;

    void EasyClickCore::scan_ui_objects(HWND hwnd) const {
        pimpl->hints_.clear() ;
        pimpl->elements_.clear() ;
        pimpl->positions_.clear() ;
        pimpl->strhints_.clear() ;

        RECT root_rect ;
        if(!GetWindowRect(hwnd, &root_rect)) {
            throw RUNTIME_EXCEPT("Could not get a rectangle of the root window.") ;
        }

        if(gparams::get_b("uiacachebuild")) {
            auto root_elem = AsyncUIACacheBuilder::get_root_element(hwnd) ;
            pimpl->scanner_.scan(root_elem, pimpl->elements_) ;
        }
        else {
            pimpl->scanner_.scan(hwnd, pimpl->elements_) ;
        }

        for(auto& elem : pimpl->elements_) {
            RECT rect ;
            if(util::is_failed(elem->get_CachedBoundingRectangle(&rect))) {
                throw RUNTIME_EXCEPT("Could not get a rectangle of a element.") ;
            }

            if(util::is_fully_in_range(rect, root_rect)) {
                pimpl->positions_.emplace_back(
                        util::center_x(rect),
                        util::center_y(rect)) ;
            }
        }

        // enumerate all window owned by the foreground window process.
        DWORD procid ;
        if(GetWindowThreadProcessId(hwnd, &procid)) {
            ProcessScanInfo psinfo{procid, pimpl->positions_} ;
            if(!EnumWindows(EnumerateAllThread, reinterpret_cast<LPARAM>(&psinfo))) {
                throw RUNTIME_EXCEPT("Failed to scan for threads in the same process.") ;
            }
        }

        if(pimpl->positions_.empty()) {
            return ;
        }

        util::remove_deplication(pimpl->positions_) ;

        assign_identifier_hints(pimpl->positions_.size(), pimpl->hints_) ;
        convert_hints_to_strings(pimpl->hints_, pimpl->strhints_) ;
    }

    void EasyClickCore::create_matching_loop(KeyCode sendkey) const {
        if(pimpl->positions_.empty() || pimpl->hints_.empty()) {
            return ;
        }

        auto ft = pimpl->input_hinter_.launch_async_loop(
                pimpl->positions_,
                pimpl->hints_) ;

        using namespace std::chrono ;
        while(ft.wait_for(50ms) == std::future_status::timeout) {
            try {
                if(pimpl->input_hinter_.drawable_hints_num() == pimpl->hints_.size()) {
                    // Hints were not matched yet, so must draw all hints.
                    pimpl->display_hinter_.paint_all_hints(
                            pimpl->positions_,
                            pimpl->strhints_) ;
                }
                else {
                    pimpl->display_hinter_.paint_matching_hints(
                            pimpl->positions_,
                            pimpl->strhints_,
                            pimpl->input_hinter_.matched_counts()) ;
                }
            }
            catch(const std::exception& e) {
                pimpl->input_hinter_.cancel() ;
                throw e ;
            }
        }

        util::refresh_display(NULL) ;

        if(auto pos = ft.get()) {
            SetCursorPos(pos->x(), pos->y()) ;
            mouse::click(sendkey) ;
        }

        //Release all keys in order to avoid the next matching right after.
        for(KeyCode key : keyabsorber::get_pressed_list()) {
            keyabsorber::release_virtually(key) ;
        }
    }

    void EasyClickCore::reconstruct() {
        pimpl->display_hinter_.load_config() ;
    }
}
