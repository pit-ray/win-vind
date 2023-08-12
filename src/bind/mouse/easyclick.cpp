#include "easyclick.hpp"

#include "hinter.hpp"

#include "util/container.hpp"
#include "util/point2d.hpp"
#include "util/rect.hpp"
#include "util/uiwalker.hpp"
#include "util/winwrap.hpp"

#include "core/cmdmatcher.hpp"
#include "core/cmdunit.hpp"
#include "core/hintassign.hpp"
#include "core/inputgate.hpp"
#include "core/settable.hpp"

#include "opt/optionlist.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"

#include "../window/winutil.hpp"

#include <algorithm>
#include <string>
#include <vector>


namespace
{
    using namespace vind ;

    struct ProcessScanInfo {
        DWORD pid ;
        std::vector<util::Point2D>& points ;
    } ;

    BOOL CALLBACK ScanCenterPoint(HWND hwnd, LPARAM lparam) {
        auto obj_list = reinterpret_cast<std::vector<util::Point2D>*>(lparam) ;

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

    struct UIScanner : public util::UIWalker {
        UIScanner()
        : UIWalker(UIA_IsKeyboardFocusablePropertyId)
        {}

        bool filter_element(const util::SmartElement& elem) override {
            BOOL flag ;
            if(util::is_failed(elem->get_CachedIsKeyboardFocusable(&flag))) {
                throw RUNTIME_EXCEPT("Could not get the keyboard focusable flag.") ;
            }
            return flag == TRUE ;
        }
    } ;

    bool scan_ui_objects(
            HWND hwnd,
            util::UIWalker& scanner,
            std::vector<util::SmartElement>& elements,
            std::vector<util::Point2D>& positions) {

        auto root_rect = util::get_window_rect(hwnd) ;

        auto& settable = core::SetTable::get_instance() ;
        if(settable.get("uiacachebuild").get<bool>()) {
            auto root_elem = opt::AsyncUIACacheBuilder::get_root_element(hwnd) ;
            scanner.scan(root_elem, elements) ;
        }
        else {
            scanner.scan(hwnd, elements) ;
        }

        for(auto& elem : elements) {
            RECT rect ;
            if(util::is_failed(elem->get_CachedBoundingRectangle(&rect))) {
                throw RUNTIME_EXCEPT("Could not get a rectangle of a element.") ;
            }

            if(util::is_fully_in_range(rect, root_rect)) {
                positions.emplace_back(
                    util::center_x(rect), util::center_y(rect)) ;
            }
        }

        // enumerate all window owned by the foreground window process.
        DWORD procid ;
        if(GetWindowThreadProcessId(hwnd, &procid)) {
            ProcessScanInfo psinfo{procid, positions} ;
            if(!EnumWindows(EnumerateAllThread, reinterpret_cast<LPARAM>(&psinfo))) {
                throw RUNTIME_EXCEPT("Failed to scan for threads in the same process.") ;
            }
        }

        if(positions.empty()) {
            return false ;
        }
        util::remove_deplication(positions) ;
        return true ;
    }
}


namespace vind
{

    namespace bind
    {
        struct EasyClick::Impl {
            Hinter hinter_ ;
            UIScanner scanner_ ;

            Impl()
            : hinter_(opt::ref_global_options_bynames(
                opt::AsyncUIACacheBuilder().name(),
                opt::VCmdLine().name())),
              scanner_()
            {}
        } ;

        EasyClick::EasyClick()
        : BindedFuncVoid("easyclick"),
          pimpl(std::make_unique<Impl>())
        {
            opt::AsyncUIACacheBuilder::register_properties(
                    pimpl->scanner_.get_properties()) ;
        }

        EasyClick::~EasyClick() noexcept = default ;
        EasyClick::EasyClick(EasyClick&&) = default ;
        EasyClick& EasyClick::operator=(EasyClick&&) = default ;

        void EasyClick::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;

            std::vector<util::SmartElement> elems ;
            std::vector<util::Point2D> points ;

            auto hwnd = util::get_foreground_window() ;
            if(!hwnd) {
                return ;
            }

            if(!scan_ui_objects(hwnd, pimpl->scanner_, elems, points)) {
                return ;
            }

            std::vector<core::Hint> hints ;
            std::vector<std::string> hint_texts ;
            core::assign_identifier_hints(
                points.size(), hints, hint_texts,
                settable.get("hintkeys").get<std::string>()) ;

            std::vector<core::CmdMatcher> matchers{} ;
            for(const auto& hint : hints) {
                std::vector<core::CmdUnit::SPtr> cmds ;
                for(const auto& unit : hint) {
                    cmds.push_back(std::make_shared<core::CmdUnit>(unit)) ;
                }
                matchers.emplace_back(std::move(cmds)) ;
            }

            pimpl->hinter_.start_matching(points, hint_texts, matchers) ;
        }

        void EasyClick::reconstruct() {
            auto& settable = core::SetTable::get_instance() ;
            pimpl->hinter_.set_style(
                settable.get("easyclick_fontcolor").get<std::string>(),
                settable.get("easyclick_bgcolor").get<std::string>(),
                settable.get("easyclick_fontsize").get<long>(),
                settable.get("easyclick_fontweight").get<long>(),
                settable.get("easyclick_fontname").get<std::string>()) ;
        }
    }
}


namespace
{
    HWND get_top_hwnd_from_point(const util::Point2D& pos) {
        return GetAncestor(WindowFromPoint(pos), GA_ROOT) ;
    }

    BOOL CALLBACK EnumerateVisibleWindows(HWND hwnd, LPARAM lparam) {
        if(!bind::is_visible_hwnd(hwnd)) {
            return TRUE ;
        }

        auto rect = util::get_window_rect(hwnd) ;

        auto mid_coord = [](auto lx, auto rx) {
            return lx + (rx - lx) / 2 ;
        } ;
        // To check if the window is visible or not, samples reference
        // points. In this implementation, the points are the centers
        // of the grid that divides the window into four sections.
        //
        //  Window
        //  _________________________
        // |            |            |
        // |     A      |      B     |
        // |            |            |
        // |-------------------------|
        // |            |            |
        // |     C      |      D     |
        // |____________|____________|
        //
        std::vector<util::Point2D> reference_points ;
        reference_points.emplace_back(
            mid_coord(rect.left(), rect.center_x()),
            mid_coord(rect.top(), rect.center_y())) ;
        reference_points.emplace_back(
            mid_coord(rect.center_x(), rect.right()),
            mid_coord(rect.top(), rect.center_y())) ;
        reference_points.emplace_back(
            mid_coord(rect.left(), rect.center_x()),
            mid_coord(rect.center_y(), rect.bottom())) ;
        reference_points.emplace_back(
            mid_coord(rect.center_x(), rect.right()),
            mid_coord(rect.center_y(), rect.bottom())) ;

        // For a window to be considered visible,
        // the reference point must be at least 50% visible.
        int visibility_count = 0 ;
        for(const auto& point : reference_points) {
            auto top_z_hwnd = get_top_hwnd_from_point(point) ;
            if(top_z_hwnd == hwnd) {
                visibility_count ++ ;
            }
        }
        if(visibility_count < reference_points.size() / 2) {
            return TRUE ;
        }

        auto p_set = reinterpret_cast<std::vector<HWND>*>(lparam) ;
        p_set->push_back(hwnd) ;
        return TRUE ;
    }
}

namespace vind
{
    namespace bind
    {
        struct EasyClickAll::Impl {
            Hinter hinter_ ;
            UIScanner scanner_ ;

            Impl()
            : hinter_(opt::ref_global_options_bynames(
                opt::AsyncUIACacheBuilder().name(),
                opt::VCmdLine().name())),
              scanner_()
            {}
        } ;

        EasyClickAll::EasyClickAll()
        : BindedFuncVoid("easyclick_all"),
          pimpl(std::make_unique<Impl>())
        {
            opt::AsyncUIACacheBuilder::register_properties(
                    pimpl->scanner_.get_properties()) ;
        }

        EasyClickAll::~EasyClickAll() noexcept = default ;
        EasyClickAll::EasyClickAll(EasyClickAll&&) = default ;
        EasyClickAll& EasyClickAll::operator=(EasyClickAll&&) = default ;

        void EasyClickAll::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;

            std::vector<HWND> hwnds ;
            if(!EnumWindows(EnumerateVisibleWindows, reinterpret_cast<LPARAM>(&hwnds))) {
                throw RUNTIME_EXCEPT("Could not enumerate all visible windows on the screen.") ;
            }

            if(hwnds.empty()) {
                return ;
            }

            std::vector<util::SmartElement> elems ;
            std::vector<util::Point2D> points ;
            for(auto hwnd : hwnds) {
                if(!scan_ui_objects(hwnd, pimpl->scanner_, elems, points)) {
                    return ;
                }
            }

            std::vector<core::Hint> hints ;
            std::vector<std::string> hint_texts ;
            core::assign_identifier_hints(
                points.size(), hints, hint_texts,
                settable.get("hintkeys").get<std::string>()) ;

            std::vector<core::CmdMatcher> matchers{} ;
            for(const auto& hint : hints) {
                std::vector<core::CmdUnit::SPtr> cmds ;
                for(const auto& unit : hint) {
                    cmds.push_back(std::make_shared<core::CmdUnit>(unit)) ;
                }
                matchers.emplace_back(std::move(cmds)) ;
            }

            pimpl->hinter_.start_matching(points, hint_texts, matchers) ;
        }

        void EasyClickAll::reconstruct() {
            auto& settable = core::SetTable::get_instance() ;
            pimpl->hinter_.set_style(
                settable.get("easyclick_fontcolor").get<std::string>(),
                settable.get("easyclick_bgcolor").get<std::string>(),
                settable.get("easyclick_fontsize").get<long>(),
                settable.get("easyclick_fontweight").get<long>(),
                settable.get("easyclick_fontname").get<std::string>()) ;
        }
    }
}
