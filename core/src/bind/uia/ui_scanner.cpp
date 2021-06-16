#include "bind/uia/ui_scanner.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "bind/uia/point_2d.hpp"
#include "bind/uia/uia.hpp"
#include "io/screen_metrics.hpp"
#include "util/container.hpp"
#include "util/def.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace
{
    using namespace vind ;

    struct ProcessScanInfo {
        DWORD pid ;
        std::vector<Point2D>& points ;
    } ;

    BOOL CALLBACK ScanCenterPoint(HWND hwnd, LPARAM lparam) {
        auto obj_points = reinterpret_cast<std::vector<Point2D>*>(lparam) ;

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

        if(screenmetrics::width(rect) == 0 || screenmetrics::height(rect) == 0) {
            return TRUE ;
        }
        if(rect.left < 0 || rect.top < 0 || rect.right < 0 || rect.bottom < 0) {
            return TRUE ;
        }

        obj_points->emplace_back(
                rect.left + (rect.right - rect.left) / 2,
                rect.top + (rect.bottom - rect.top) / 2) ;
        return TRUE ;
    }

    BOOL CALLBACK EnumerateAllThread(HWND hwnd, LPARAM lparam) {
        auto psinfo = reinterpret_cast<ProcessScanInfo*>(lparam) ;

        DWORD procid ;
        auto thid = GetWindowThreadProcessId(hwnd, &procid) ;

        if(procid == psinfo->pid) {
            //enumerate all threads owned by the parent process.
            EnumThreadWindows(thid, ScanCenterPoint, reinterpret_cast<LPARAM>(&(psinfo->points))) ;
        }

        return TRUE ;
    }
}


namespace vind
{
    struct UIScanner::Impl {
        const uiauto::CUIA& cuia_ ;
        uiauto::SmartCacheReq cache_request_ ;

        explicit Impl()
        : cuia_(uiauto::get_global_cuia()),
          cache_request_(nullptr, uiauto::delete_com)
        {
            IUIAutomationCacheRequest* cr_raw ;
            if(FAILED(cuia_->CreateCacheRequest(&cr_raw))) {
                throw LOGIC_EXCEPT("Could not create IUIAutomationCacheRequest.") ;
            }
            cache_request_.reset(cr_raw) ;

            //g_cache_req->AddProperty(UIA_ClickablePointPropertyId) ;
            cache_request_->AddProperty(UIA_IsEnabledPropertyId) ;
            cache_request_->AddProperty(UIA_IsOffscreenPropertyId) ;
            cache_request_->AddProperty(UIA_IsKeyboardFocusablePropertyId) ;
            cache_request_->AddProperty(UIA_BoundingRectanglePropertyId) ;

            if(FAILED(cache_request_->put_AutomationElementMode(
                            AutomationElementMode::AutomationElementMode_None))) {
                throw LOGIC_EXCEPT("Could not initialize UI Automation Element Mode.") ;
            }
            if(FAILED(cache_request_->put_TreeScope(TreeScope::TreeScope_Subtree))) {
                throw LOGIC_EXCEPT("Could not initialzie TreeScope.") ;
            }
            //g_cache_req->put_TreeScope(static_cast<TreeScope>(TreeScope::TreeScope_Children | TreeScope::TreeScope_Element)) ;
        }

        //Why, cannot get the value of ClickablePointPropertyId with GetCachedProperyValue.
        //We must use cache in order not to freeze.
        static auto get_clickable_point(uiauto::SmartElement& elem) {
            if(VARIANT val ; SUCCEEDED(elem->GetCachedPropertyValue(UIA_ClickablePointPropertyId, &val))) {
                if(val.vt == (VT_R8 | VT_ARRAY)) {
                    if(LONG* ppvdata ; SUCCEEDED(SafeArrayAccessData(val.parray,
                                    reinterpret_cast<void**>(&ppvdata)))) {

                        auto result = Point2D(ppvdata[0], ppvdata[1]) ;
                        SafeArrayUnaccessData(val.parray) ;
                        return result ;
                    }
                }
                VariantClear(&val) ;
            }
            throw std::runtime_error("Could not get a clickable point.") ;
        }

        static auto get_keyboard_focusable_point(
                uiauto::SmartElement& elem,
                const RECT& window_rect,
                BOOL parent_is_focasuable=FALSE) {

            if(parent_is_focasuable == FALSE) {
                BOOL flag ;
                if(FAILED(elem->get_CachedIsKeyboardFocusable(&flag))) {
                    throw std::runtime_error("Could not get the keyboard focusable flag.") ;
                }

                if(!flag) {
                    throw std::runtime_error("Element is not keyboard focusable.") ;
                }
            }

            RECT rect ;
            if(FAILED(elem->get_CachedBoundingRectangle(&rect))) {
                throw std::runtime_error("Could not get the a rectangle of element.") ;
            }

            if(!screenmetrics::is_fully_in_range(rect, window_rect)) {
                throw std::runtime_error("The rectangle of element exists in a invalid range.") ;
            }

            return Point2D(
                    rect.left + (rect.right - rect.left) / 2,
                    rect.top  + (rect.bottom - rect.top) / 2) ;
        }

        static void scan_children_enumurately(
                uiauto::SmartElementArray& parents,
                std::vector<Point2D>& obj_points,
                const RECT& window_rect,
                BOOL parent_is_focusable=FALSE) {
            int length ;
            parents->get_Length(&length) ;
            if(length == 0) {
                return ;
            }

            IUIAutomationElement* elem_raw ;
            auto elem = uiauto::make_SmartElement(nullptr) ;

            IUIAutomationElementArray* children_raw ;
            auto children = uiauto::make_SmartElementArray(nullptr) ;

            BOOL flag ;
            for(int i = 0 ; i < length ; i ++) {
                if(FAILED(parents->GetElement(i, &elem_raw))) {
                    continue ;
                }
                if(!elem_raw) continue ;
                elem.reset(elem_raw) ;

                if(FAILED(elem->get_CachedIsEnabled(&flag))) {
                    continue ;
                }
                if(!flag) continue ;

                if(FAILED(elem->GetCachedChildren(&children_raw))) {
                    continue ;
                }
                if(children_raw != nullptr) {
                    children.reset(children_raw) ;

                    //If a parent object is focusable, regards the children as focusable too.
                    if(FAILED(elem->get_CachedIsKeyboardFocusable(&flag))) {
                        return ;
                    }
                    scan_children_enumurately(children, obj_points, window_rect, flag) ; //recursive calling
                    continue ;
                }

                //scan GUI objects only at leaves in tree.
                try {
                    obj_points.push_back(get_keyboard_focusable_point(elem, window_rect, parent_is_focusable)) ;
                }
                catch(const std::runtime_error&) {
                    continue ;
                }
            }
        }

        void scan_object_from_hwnd(
                HWND hwnd,
                std::vector<Point2D>& obj_points) {
            IUIAutomationElement* elem_raw ;
            if(FAILED(cuia_->ElementFromHandle(hwnd, &elem_raw))) {
                throw RUNTIME_EXCEPT("Could not get IUIAutomationElement from HWND by COM method.") ;
            }
            if(!elem_raw) {
                throw RUNTIME_EXCEPT("Could not get UIAutomationElement from HWND.") ;
                return ;
            }
            auto elem = uiauto::make_SmartElement(elem_raw) ;

            if(FAILED(elem->BuildUpdatedCache(cache_request_.get(), &elem_raw))) {
                throw RUNTIME_EXCEPT("Could not update caches of UIAutomationElement.") ;
            }
            if(elem_raw != nullptr) {
                elem.reset(elem_raw) ; //successfully updated
            }

            BOOL flag ;
            if(FAILED(elem->get_CachedIsEnabled(&flag)))  return ;
            if(!flag) return ;

            if(FAILED(elem->get_CachedIsOffscreen(&flag))) return ;
            if(flag) return ;

            RECT rect ;
            if(!GetWindowRect(hwnd, &rect)) return ;

            IUIAutomationElementArray* children_raw ;
            if(SUCCEEDED(elem->GetCachedChildren(&children_raw))) {
                if(children_raw) {
                    auto children = uiauto::make_SmartElementArray(children_raw) ;
                    scan_children_enumurately(children, obj_points, rect) ;
                }
                else {
                    //If the parent element is a leaf in tree.
                    try {
                        obj_points.push_back(get_keyboard_focusable_point(elem, rect)) ;
                    }
                    catch(const std::runtime_error&) {
                        return ;
                    }
                }
            }
        }
    } ;

    UIScanner::UIScanner()
    : pimpl(std::make_unique<Impl>())
    {}

    UIScanner::~UIScanner() noexcept             = default ;
    UIScanner::UIScanner(UIScanner&&)            = default ;
    UIScanner& UIScanner::operator=(UIScanner&&) = default ;

    const uiauto::SmartCacheReq& UIScanner::get_cache_request() const noexcept {
        return pimpl->cache_request_ ;
    }

    void UIScanner::scan(
            HWND hwnd,
            std::vector<Point2D>& return_positions,
            bool scan_all_thread_window) const {

        if(!return_positions.empty()) {
            return_positions.clear() ;
        }

        //scan all GUI objects in current window and children
        pimpl->scan_object_from_hwnd(hwnd, return_positions) ;

        //enumerate all window owned by the foreground window process.
        if(scan_all_thread_window) {
            DWORD procid ;
            if(GetWindowThreadProcessId(hwnd, &procid)) {
                ProcessScanInfo psinfo{procid, return_positions} ;
                if(!EnumWindows(EnumerateAllThread, reinterpret_cast<LPARAM>(&psinfo))) {
                    throw RUNTIME_EXCEPT("Failed EnumerateAllThread.") ;
                }
            }
        }

        util::remove_deplication(return_positions) ;
    }
}
