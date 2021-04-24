#include "bind/uia/ec_scan.hpp"

#include <windows.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "bind/uia/point_2d.hpp"
#include "bind/uia/uia.hpp"
#include "io/screen_metrics.hpp"
#include "util/container.hpp"
#include "util/def.hpp"

namespace
{
    inline auto& get_cache_req() {
        static auto g_cache_req = vind::uiauto::make_SmartCacheReq(nullptr) ;
        return g_cache_req ;
    }
}

namespace vind
{
    namespace easyclick {
        void initialize_global_uia() {
            decltype(auto) cuia = uiauto::get_global_cuia() ;

            IUIAutomationCacheRequest* cr_raw ;
            if(FAILED(cuia->CreateCacheRequest(&cr_raw))) {
                throw LOGIC_EXCEPT("Could not create IUIAutomationCacheRequest.") ;
            }
            decltype(auto) g_cache_req = get_cache_req() ;
            g_cache_req.reset(cr_raw) ;

            //g_cache_req->AddProperty(UIA_ClickablePointPropertyId) ;
            g_cache_req->AddProperty(UIA_IsEnabledPropertyId) ;
            g_cache_req->AddProperty(UIA_IsOffscreenPropertyId) ;
            g_cache_req->AddProperty(UIA_IsKeyboardFocusablePropertyId) ;
            g_cache_req->AddProperty(UIA_BoundingRectanglePropertyId) ;

            if(FAILED(g_cache_req->put_AutomationElementMode(
                            AutomationElementMode::AutomationElementMode_None))) {
                throw LOGIC_EXCEPT("Could not initialize UI Automation Element Mode.") ;
            }
            if(FAILED(g_cache_req->put_TreeScope(TreeScope::TreeScope_Subtree))) {
                throw LOGIC_EXCEPT("Could not initialzie TreeScope.") ;
            }

            //g_cache_req->put_TreeScope(static_cast<TreeScope>(TreeScope::TreeScope_Children | TreeScope::TreeScope_Element)) ;
        }

        //Why, cannot get the value of ClickablePointPropertyId with GetCachedProperyValue.
        //We must use cache in order not to freeze.
        inline auto get_clickable_point(uiauto::SmartElement& elem) {
            if(VARIANT val ; SUCCEEDED(elem->GetCachedPropertyValue(UIA_ClickablePointPropertyId, &val))) {
                if(val.vt == (VT_R8 | VT_ARRAY)) {
                    if(LONG* ppvdata ; SUCCEEDED(SafeArrayAccessData(val.parray,
                                    reinterpret_cast<void**>(&ppvdata)))) {

                        return Point2D(ppvdata[0], ppvdata[1]) ;
                        SafeArrayUnaccessData(val.parray) ;
                    }
                }
                VariantClear(&val) ;
            }
            throw std::runtime_error("Could not get a clickable point.") ;
        }

        inline auto get_keyboard_focusable_point(
                uiauto::SmartElement& elem,
                const RECT& window_rect,
                const BOOL parent_is_focasuable=FALSE) {

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

        void scan_children_enumurately(
                uiauto::SmartElementArray& parents,
                std::vector<Point2D>& obj_points,
                const RECT& window_rect,
                const BOOL parent_is_focusable=FALSE) {
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
            decltype(auto) cuia = uiauto::get_global_cuia() ;
            IUIAutomationElement* elem_raw ;
            if(FAILED(cuia->ElementFromHandle(hwnd, &elem_raw))) {
                throw RUNTIME_EXCEPT("Could not get IUIAutomationElement from HWND by COM method.") ;
            }
            if(!elem_raw) {
                throw RUNTIME_EXCEPT("Could not get UIAutomationElement from HWND.") ;
                return ;
            }
            auto elem = uiauto::make_SmartElement(elem_raw) ;

            if(FAILED(elem->BuildUpdatedCache(get_cache_req().get(), &elem_raw))) {
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

        struct ProcessScanInfo {
            DWORD pid ;
            std::vector<Point2D>& points ;

            explicit ProcessScanInfo(const DWORD procid, std::vector<Point2D>& obj_points)
            : pid(procid),
              points(obj_points)
            {}
        } ;

        BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam) {
            auto psinfo = reinterpret_cast<ProcessScanInfo*>(lparam) ;

            DWORD procid ;
            auto thid = GetWindowThreadProcessId(hwnd, &procid) ;

            if(procid == psinfo->pid) {
                //enumerate all threads owned by the parent process.
                EnumThreadWindows(thid, ScanCenterPoint, reinterpret_cast<LPARAM>(&(psinfo->points))) ;
            }

            return TRUE ;
        }

        void scan_gui_objects(std::vector<Point2D>& obj_points) {
            auto hwnd = GetForegroundWindow() ;

            //scan all GUI objects in current window and children
            scan_object_from_hwnd(hwnd, obj_points) ;

            //enumerate all window owned by the foreground window process.
            DWORD procid ;
            if(!GetWindowThreadProcessId(hwnd, &procid)) {
                return ;
            }

            ProcessScanInfo psinfo(procid, obj_points) ;
            EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&psinfo)) ;

            util::remove_deplication(obj_points) ;
        }
    }
}
