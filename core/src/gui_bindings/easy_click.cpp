#include "easy_click.hpp"

#include "msg_logger.hpp"
#include "uia.hpp"
#include "utility.hpp"

#include "disable_gcc_warning.hpp"
#include <windows.h>
#include <oleauto.h>
#include <winerror.h>
#include <winuser.h>
#include "enable_gcc_warning.hpp"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

inline static bool operator==(const RECT& lhs, const RECT& rhs) noexcept {
    return lhs.left == rhs.left && lhs.top == rhs.top \
                     && lhs.right == rhs.right && lhs.bottom == rhs.bottom ;
}
inline static bool operator!=(const RECT& lhs, const RECT& rhs) noexcept {
    return !(lhs == rhs) ;
}

namespace EsyClk
{
    void print_hresult(HRESULT& res) ;
    inline static void draw_rect_by_center(LONG x, LONG y, LONG delta=10) ;

    //lhs > rhs
    inline static bool is_out_of_RECT(const RECT& lhs, const RECT& rhs) {
        return lhs.left < rhs.left || lhs.top < rhs.top \
                         || lhs.right > rhs.right || lhs.bottom > rhs.bottom ;
    }

    class Point2D {
    private:
        LONG mx ;
        LONG my ;

    public:
        explicit Point2D(LONG x, LONG y)
        : mx(x), my(y)
        {}

        template <typename T>
        explicit Point2D(T x, T y)
        : Point2D(static_cast<LONG>(x), static_cast<LONG>(y))
        {}

        LONG x() const noexcept {
            return mx ;
        }

        LONG y() const noexcept {
            return my ;
        }

        POINT get() const noexcept {
            return POINT{mx, my} ;
        }

        operator POINT() const noexcept {
            return POINT{mx, my} ;
        }

        bool operator==(const Point2D& rhs) noexcept {
            return mx == rhs.mx && my == rhs.my ;
        }
        bool operator!=(const Point2D& rhs) noexcept {
            return mx != rhs.mx || my != rhs.my ;
        }
        bool operator>(const Point2D& rhs) noexcept {
            return mx != rhs.mx ? mx > rhs.mx : my > rhs.my ;
        }
        bool operator<(const Point2D& rhs) noexcept {
            return mx != rhs.mx ? mx < rhs.mx : my < rhs.my ;
        }
        bool operator>=(const Point2D& rhs) noexcept {
            return mx != rhs.mx ? mx >= rhs.mx : my >= rhs.my ;
        }
        bool operator<=(const Point2D& rhs) noexcept {
            return mx != rhs.mx ? mx <= rhs.mx : my <= rhs.my ;
        }
    } ;

    static std::vector<Point2D> g_objpos ;
    static auto g_cache_req = UIA::make_SmartCacheReq(nullptr) ;

    static LOGFONT g_font{0} ;

    void init() {
        g_objpos.reserve(2048) ;

        decltype(auto) cuia = UIA::get_global_cuia() ;

        IUIAutomationCacheRequest* cr_raw ;
        if(FAILED(cuia->CreateCacheRequest(&cr_raw))) {
            throw LOGIC_EXCEPT("Could not create IUIAutomationCacheRequest.") ;
        }
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

        g_font.lfHeight         = 25 ;
        g_font.lfWidth          = 0 ;
        g_font.lfEscapement     = 0 ;
        g_font.lfOrientation    = 0 ;
        g_font.lfWeight         = FW_MEDIUM ;
        g_font.lfItalic         = FALSE ;
        g_font.lfUnderline      = FALSE ;
        g_font.lfStrikeOut      = FALSE ;
        g_font.lfCharSet        = ANSI_CHARSET ;
        g_font.lfOutPrecision   = OUT_TT_ONLY_PRECIS ;
        g_font.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
        g_font.lfQuality        = ANTIALIASED_QUALITY ;
        g_font.lfPitchAndFamily = 0 ;
        g_font.lfFaceName[0]    = '\0' ;
    }

    //Why, cannot get the value of ClickablePointPropertyId with GetCachedProperyValue.
    //We must use cache in order not to freeze.
    inline static void get_clickable_point(UIA::SmartElement& elem) {
        if(VARIANT val ; SUCCEEDED(elem->GetCachedPropertyValue(UIA_ClickablePointPropertyId, &val))) {
            if(val.vt == (VT_R8 | VT_ARRAY)) {
                if(LONG* ppvdata ; SUCCEEDED(SafeArrayAccessData(val.parray,
                                reinterpret_cast<void**>(&ppvdata)))) {

                    g_objpos.emplace_back(ppvdata[0], ppvdata[1]) ;
                    SafeArrayUnaccessData(val.parray) ;
                }
            }

            VariantClear(&val) ;
        }
    }

    inline static void get_keyboard_focusable_point(
            UIA::SmartElement& elem,
            const RECT& window_rect,
            const BOOL parent_is_focasuable=FALSE) {
        if(!parent_is_focasuable) {
            BOOL flag ;
            if(FAILED(elem->get_CachedIsKeyboardFocusable(&flag))) {
                return ;
            }

            if(!flag) {
                return ;
            }
        }

        RECT rect ;
        if(FAILED(elem->get_CachedBoundingRectangle(&rect))) {
            return ;
        }

        if(is_out_of_RECT(rect, window_rect)) {
            return ;
        }

        g_objpos.emplace_back(
                rect.left + (rect.right - rect.left) / 2,
                rect.top  + (rect.bottom - rect.top) / 2) ;
    }

    inline static void enumurate_child_elem(
            UIA::SmartElementArray& parents,
            const RECT& window_rect,
            const BOOL& parent_is_focusable=FALSE) {
        int length ;
        parents->get_Length(&length) ;
        if(length == 0) return ;

        IUIAutomationElement* elem_raw ;
        auto elem = UIA::make_SmartElement(nullptr) ;

        IUIAutomationElementArray* children_raw ;
        auto children = UIA::make_SmartElementArray(nullptr) ;

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
                enumurate_child_elem(children, window_rect, flag) ; //recursive calling
                continue ;
            }

            //scan GUI objects only at leaves in tree.
            //get_clickable_point(elem) ;
            get_keyboard_focusable_point(elem, window_rect, parent_is_focusable) ;
        }
    }

    inline static void scan_object_from_hwnd(HWND hwnd) {

        decltype(auto) cuia = UIA::get_global_cuia() ;

        IUIAutomationElement* elem_raw ;
        if(FAILED(cuia->ElementFromHandle(hwnd, &elem_raw))) {
            throw RUNTIME_EXCEPT("Could not get IUIAutomationElement from HWND by COM method.") ;
        }
        if(!elem_raw) {
            throw RUNTIME_EXCEPT("Could not get UIAutomationElement from HWND.") ;
            return ;
        }
        auto elem = UIA::make_SmartElement(elem_raw) ;

        if(FAILED(elem->BuildUpdatedCache(g_cache_req.get(), &elem_raw))) {
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
                auto children = UIA::make_SmartElementArray(children_raw) ;
                enumurate_child_elem(children, rect) ;
            }
            else {
                //If the parent element is a leaf in tree.
                get_keyboard_focusable_point(elem, rect) ;
            }
        }
    }

    static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam) {
        scan_object_from_hwnd(hwnd) ;
        return TRUE ;
    }

    using TargetWindowInfo = std::pair<HWND, DWORD> ;

    static BOOL CALLBACK EnumThreadWndProc(HWND hwnd, LPARAM lparam) {
        if(hwnd == reinterpret_cast<TargetWindowInfo*>(lparam)->first) {
            return TRUE ;
        }

        if(!IsWindowVisible(hwnd)) {
            return TRUE ;
        }

        //also register the position of the other thread window's title bar.
        RECT rect ;
        if(GetWindowRect(hwnd, &rect)) {
            g_objpos.emplace_back(
                    (rect.left + rect.right) / 2,
                    rect.top + 16) ;
        }

        return TRUE ;
    }

    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam) {
        auto parent_procid = reinterpret_cast<TargetWindowInfo*>(lparam)->second ;

        DWORD procid ;
        auto thid = GetWindowThreadProcessId(hwnd, &procid) ;

        if(procid == parent_procid) {
            //enumerate all threads owned by the parent process.
            EnumThreadWindows(thid, EnumThreadWndProc, lparam) ;
        }

        return TRUE ;
    }

    inline static void scan_gui_objects() {
        auto hwnd = GetForegroundWindow() ;

        //scan GUI object in parent window
        scan_object_from_hwnd(hwnd) ;

        //Scan GUI object in child window
        EnumChildWindows(hwnd, EnumChildProc, 0) ;

        //enumerate all window owned by the foreground window process.
        DWORD procid ;
        GetWindowThreadProcessId(hwnd, &procid) ;

        TargetWindowInfo info(hwnd, procid) ;

        EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&info)) ;

        Utility::remove_deplication(EsyClk::g_objpos) ;
    }

    static std::unordered_map<std::string, const Point2D&> g_objpos_wid ;
    inline static auto assign_identifiers()
    {
        g_objpos_wid.clear() ;
        char c = 'A' ;
        for(int i = 0 ; i < g_objpos.size() ; i ++) {
            if(g_objpos[i].x() <= 0 || g_objpos[i].y() <= 0) {
                continue ;
            }
            std::string str {c} ;
            g_objpos_wid.emplace(str, g_objpos[i]) ;
            if(c < 'z') {
                c ++ ;
            }
        }
    }

    inline static void draw_identifiers() {
        auto hdc = CreateDCA("DISPLAY", NULL, NULL, NULL) ;
        if(!hdc) {
            throw RUNTIME_EXCEPT("CreateDC") ;
        }

        auto font = CreateFontIndirect(&g_font) ;
        if(!font) {
            throw RUNTIME_EXCEPT("CreateFontIndirectA") ;
        }

        if(!SelectObject(hdc, font)) {
            throw RUNTIME_EXCEPT("SelectObject") ;
        }

        if(SetBkColor(hdc, RGB(20, 20, 20)) == CLR_INVALID) {
            throw RUNTIME_EXCEPT("SetBkColor") ;
        }
        if(SetTextColor(hdc, RGB(244, 244, 244)) == CLR_INVALID) {
            throw RUNTIME_EXCEPT("SetTextColor") ;
            return ;
        }

        if(SetTextCharacterExtra(hdc, 2) == static_cast<int>(0x80000000)) {
            throw RUNTIME_EXCEPT("SetTextCharacterExtra") ;
        }

        for(auto& idpos : g_objpos_wid) {
            const auto& id = idpos.first ;
            const auto& pos = idpos.second ;
            TextOutA(hdc, pos.x(), pos.y(), id.c_str(), lstrlenA(id.c_str())) ;
            //draw_rect_by_center(pos.x(), pos.y()) ;
        }

        if(!DeleteDC(hdc)) {
            throw RUNTIME_EXCEPT("DeleteDC") ;
        }
    }

    static HWND g_prehwnd = nullptr ;
    static RECT g_prerect = {0, 0, 0, 0} ;

    inline static bool need_update(HWND hwnd) {
        //If the location of the previous selected window is changed or other window is selected, must update.
        RECT rect ;
        if(hwnd == g_prehwnd) {
            if(GetWindowRect(hwnd, &rect)) {
                if(rect == g_prerect) {
                    return false ;
                }
                g_prerect = rect ;
            }
        }
        else {
            g_prehwnd = hwnd ;
            if(GetWindowRect(hwnd, &rect)) {
                g_prerect = rect ;
            }
        }

        return true ;
    }
}

//EasyClick
EasyClick::EasyClick()
{
    EsyClk::init() ;
}
const std::string EasyClick::sname() noexcept
{
    return "easy_click" ;
}
void EasyClick::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) {
    if(!first_call) return ;

    if(EsyClk::need_update(GetForegroundWindow())) {
        EsyClk::g_objpos.clear() ;
        EsyClk::scan_gui_objects() ;
    }

    std::cout << "capa:" << EsyClk::g_objpos.capacity() << "\tsize: " << EsyClk::g_objpos.size() << std::endl ;

    if(!EsyClk::g_objpos.empty()) {
        EsyClk::assign_identifiers() ;
        EsyClk::draw_identifiers() ;
    }
}


//Ideally, need_update must understand switching of tabs in Microsoft Edge.
//However, we currently don't implement such a feature.
//UpdateEasyClick
const std::string UpdateEasyClick::sname() noexcept
{
    return "update_easy_click" ;
}
void UpdateEasyClick::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) {
    if(!first_call) return ;

    EsyClk::g_prehwnd = GetForegroundWindow() ;
    GetWindowRect(EsyClk::g_prehwnd, &EsyClk::g_prerect) ;

    EsyClk::g_objpos.clear() ;
    EsyClk::scan_gui_objects() ;
}


namespace EsyClk
{
    //for debug
    void print_hresult(HRESULT& res) {
        switch(res) {
            case S_OK:
                std::cout << "S_OK\n" ;
                break ;

            case S_FALSE:
                std::cout << "S_FALSE\n" ;
                break ;

            case E_FAIL:
                std::cout << "E_FAIL\n" ;
                break ;

            case E_INVALIDARG:
                std::cout << "E_INVALIDARG\n" ;
                break ;

            case E_NOINTERFACE:
                std::cout << "E_NOINTERFACE\n" ;
                break ;

            case E_OUTOFMEMORY:
                std::cout << "E_OUTOFMEMORY\n" ;
                break ;

            case E_UNEXPECTED:
                std::cout << "E_UNEXPECTED\n" ;
                break ;

            case E_NOTIMPL:
                std::cout << "E_NOTIMPL\n" ;
                break ;
        }
    }

    //for debug
    inline static void draw_rect_by_center(LONG x, LONG y, LONG delta)
    {
        auto hdc = CreateDCA("DISPLAY", NULL, NULL, NULL) ;
        if(!hdc) {
            throw RUNTIME_EXCEPT("CreateDC") ;
        }

        Rectangle(hdc, x - delta, y - delta, x + delta, y + delta) ;

        if(!DeleteDC(hdc)) {
            throw RUNTIME_EXCEPT("DeleteDC") ;
        }
    }
}
