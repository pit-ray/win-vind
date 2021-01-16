#include "easy_click.hpp"

#include "i_params.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "key_logger.hpp"
#include "keybrd_eventer.hpp"
#include "mouse_eventer.hpp"
#include "msg_logger.hpp"
#include "uia.hpp"
#include "utility.hpp"
#include "virtual_key_fwd.hpp"
#include "vkc_converter.hpp"
#include "win_vind.hpp"

#include <windows.h>
#include <wingdi.h>
#include <winuser.h>

#include <array>
#include <iostream>
#include <memory>
#include <tuple>
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

    //Is lhs out of rhs ?
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
            return my != rhs.my ? my > rhs.my : mx > rhs.mx ;
        }
        bool operator<(const Point2D& rhs) noexcept {
            return my != rhs.my ? my < rhs.my : mx < rhs.mx ;
        }
        bool operator>=(const Point2D& rhs) noexcept {
            return my != rhs.my ? my >= rhs.my : mx >= rhs.mx ;
        }
        bool operator<=(const Point2D& rhs) noexcept {
            return my != rhs.my ? my <= rhs.my : mx <= rhs.mx ;
        }
    } ;

    static std::vector<Point2D> g_objpos ;

    inline static auto& get_cache_req() {
        static auto g_cache_req = UIA::make_SmartCacheReq(nullptr) ;
        return g_cache_req ;
    }

    void initialize() {
        g_objpos.reserve(2048) ;

        decltype(auto) cuia = UIA::get_global_cuia() ;

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
    }


    // ----------------------------
    //
    //    @GUI  Scan  Functions
    //
    // ----------------------------

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

    static void enumurate_child_elem(
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
                auto children = UIA::make_SmartElementArray(children_raw) ;
                enumurate_child_elem(children, rect) ;
            }
            else {
                //If the parent element is a leaf in tree.
                get_keyboard_focusable_point(elem, rect) ;
            }
        }
    }

    static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM UNUSED(lparam)) {
        scan_object_from_hwnd(hwnd) ;
        return TRUE ;
    }

    static BOOL CALLBACK EnumThreadWndProc(HWND hwnd, LPARAM UNUSED(lparam)) {
        if(!IsWindowVisible(hwnd)) {
            return TRUE ;
        }

        //also register the position of the other thread window's title bar.
        RECT rect ;
        if(GetWindowRect(hwnd, &rect)) {
            g_objpos.emplace_back(
                    rect.left + (rect.right - rect.left) / 2,
                    rect.top + (rect.bottom - rect.top) / 2) ;
        }
        return TRUE ;
    }

    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam) {
        auto parent_procid = static_cast<DWORD>(lparam) ;

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
        if(!EnumChildWindows(hwnd, EnumChildProc, 0)) {
            return ;
        }

        //enumerate all window owned by the foreground window process.
        DWORD procid ;
        if(!GetWindowThreadProcessId(hwnd, &procid)) {
            return ;
        }

        EnumWindows(EnumWindowsProc, static_cast<LPARAM>(procid)) ;

        Utility::remove_deplication(EsyClk::g_objpos) ;
    }

    static HWND g_prehwnd = nullptr ;
    static RECT g_prerect = {0, 0, 0, 0} ;

    inline static bool need_update(HWND hwnd) {
        if(!iParams::get_b("enable_esyclk_hwnd_cache")) {
            return true ;
        }

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


    // ---------------------------------------
    //
    //    @Identifier  Assign  Functions
    //
    // ---------------------------------------
    static constexpr std::array<unsigned char, 26> labels = {
        VKC_A, VKC_S, VKC_D, VKC_G, VKC_H,
        VKC_K, VKC_L, VKC_Q, VKC_W, VKC_E,
        VKC_R, VKC_T, VKC_Y, VKC_U, VKC_I,
        VKC_O, VKC_P, VKC_Z, VKC_X, VKC_C,
        VKC_V, VKC_B, VKC_N, VKC_M, VKC_F, VKC_J
    } ;

    using hint_t = std::vector<unsigned char> ;
    static std::vector<hint_t> g_hints{} ;

    //Currrently, supported only 26 x 26 = 676 patterns.
    //If it would 3 digit combinations, the display is expected to be filled by label.
    //
    // -- Algorithm --
    // ex)
    // If five GUI-objects are detected and label is only 3.
    // <g_objpos>           <labels>
    //   [pos1]                a
    //   [pos2]                b
    //   [pos3]                c
    //   [pos4]
    //   [pos5]
    //
    // Step1: distribute single label
    //   [pos1] a
    //   [pos2] b
    //   [pos3] c
    //   [pos4]
    //   [pos5]
    //
    // Step2: push_back first label to last element of single identifiers.
    //   [pos1] a
    //   [pos2] b
    //   [pos3] c a
    //   [pos4]
    //   [pos5]
    //
    // Step3: distribute last-label and remaining labels.
    //   [pos1] a
    //   [pos2] b
    //   [pos3] c a
    //   [pos4] c b
    //   [pos5] c c
    //
    inline static auto assign_identifiers_label(const std::size_t target_count) {
        std::vector<hint_t> idx2hint(target_count) ;

        auto left_count = target_count ;
        std::size_t idx = 0 ;

        //optimizing for 1 digit  (Step1)
        for(auto& key : labels) {
            idx2hint[idx].push_back(key) ;
            idx ++ ;
            left_count -- ;

            if(left_count == 0) {
                return idx2hint ;
            }
        }

        //optimizing for 2 digit
        auto head_idx = labels.size() - 1 ;
        auto head_itr = labels.cend() - 1 ;
        while(true) {
            //(Step2)
            if(left_count != 0) {
                idx2hint[head_idx].push_back(labels.front()) ;
            }

            //(Step3)
            for(auto itr = labels.cbegin() + 1 ; itr != labels.cend() ; itr ++) {
                idx2hint[idx].push_back(*head_itr) ;
                idx2hint[idx].push_back(*itr) ;
                idx ++ ;
                left_count -- ;
                if(left_count == 0) {
                    return idx2hint ;
                }
            }

            if(head_itr == labels.cbegin()) {
                return idx2hint ;
            }
            head_itr -- ;
            head_idx -- ;
        }
    }

    static std::vector<std::string> g_hints_str{} ;
    inline static auto convert_hints_to_str(const std::vector<hint_t>& hints) {
        std::vector<std::string> hints_str(hints.size()) ;

        for(std::size_t i = 0 ; i < hints.size() ; i ++) {
            std::string str ;
            for(auto& key : hints[i]) {
                str.push_back(VKCConverter::get_shifted_ascii(key)) ;
            }
            hints_str[i] = std::move(str) ;
        }
        return hints_str ;
    }

    static LOGFONTA g_font ;
    void update_font() {
        g_font.lfHeight         = iParams::get_l("easy_click_font_size") ;
        g_font.lfWidth          = 0 ;
        g_font.lfEscapement     = 0 ;
        g_font.lfOrientation    = 0 ;
        g_font.lfWeight         = iParams::get_l("easy_click_font_weight") ;
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

    inline static void draw_identifiers(
            const std::vector<Point2D>& points,
            const std::vector<std::string>& hints_str,
            const std::vector<unsigned char>& matched_list,
            const bool exist) {

        auto delete_hdc = [] (HDC h) {
            if(h != nullptr) DeleteDC(h) ;
        } ;
        std::unique_ptr<HDC__, decltype(delete_hdc)> hdc(CreateDCA("DISPLAY", NULL, NULL, NULL), delete_hdc) ;
        if(!hdc) {
            throw RUNTIME_EXCEPT("CreateDC") ;
        }

        auto delete_font = [] (HFONT f) {
            if(f != nullptr) DeleteObject(f) ;
        } ;
        std::unique_ptr<HFONT__, decltype(delete_font)> font(CreateFontIndirectA(&g_font), delete_font) ;
        if(!font) {
            throw RUNTIME_EXCEPT("CreateFontIndirectA") ;
        }

        if(!SelectObject(hdc.get(), font.get())) {
            throw RUNTIME_EXCEPT("SelectObject") ;
        }

        auto [bk_r, bk_g, bk_b] = Utility::hex2rgb(iParams::get_s("easy_click_font_bkcolor")) ;
        auto bkcolor = RGB(bk_r, bk_g, bk_b) ;

        auto [tx_r, tx_g, tx_b] = Utility::hex2rgb(iParams::get_s("easy_click_font_color")) ;
        auto txcolor = RGB(tx_r, tx_g, tx_b) ;

        const unsigned char decay = iParams::get_uc("easy_click_matching_color_decay") ;
        using Utility::to_gray ;
        char sign = to_gray(tx_r, tx_g, tx_b) > to_gray(bk_r, bk_g, bk_b) ? -1 : 1 ;

        auto txcolor_ready = RGB(
                tx_r < decay ? 0 : tx_r + sign*decay,
                tx_g < decay ? 0 : tx_g + sign*decay,
                tx_b < decay ? 0 : tx_b + sign*decay) ;

        // A detected positon is the center one of object.
        // And, TextOutA draws from a left-upper coordinate, so must move.
        auto delta = g_font.lfHeight / 2 ;

        auto draw = [&hdc, &delta] (
                auto&& bkc,
                auto&& txc,
                auto&& str,
                auto&& point) {
            if(SetBkColor(hdc.get(), bkc) == CLR_INVALID) {
                throw RUNTIME_EXCEPT("SetBkColor") ;
            }
            if(SetTextColor(hdc.get(), txc) == CLR_INVALID) {
                throw RUNTIME_EXCEPT("SetTextColor") ;
            }

            if(SetTextCharacterExtra(hdc.get(), 1) == static_cast<int>(0x80000000)) {
                throw RUNTIME_EXCEPT("SetTextCharacterExtra") ;
            }

            if(!TextOutA(hdc.get(),
                    point.x() - delta,
                    point.y() - delta,
                    str.c_str(), lstrlenA(str.c_str()))) {

                throw RUNTIME_EXCEPT("Could not draw label. TextOutA") ;
            }
        } ;

        for(std::size_t i = 0 ; i < points.size() ; i ++) {
            auto str = " " + hints_str[i] + " " ; //add margin

            if(exist) {
                if(matched_list[i] == 0) {
                    continue ;
                }
                else {
                    draw(bkcolor, txcolor, str, points[i]) ;

                    auto overwrite_str = " " + hints_str[i].substr(0, matched_list[i]) ;
                    draw(bkcolor, txcolor_ready, overwrite_str, points[i]) ;
                    continue ;
                }
            }

            draw(bkcolor, txcolor, str, points[i]) ;
        }
    }

    inline static void loop_for_key_matching(
            const std::vector<Point2D>& points,
            const std::vector<hint_t>& hints) {

        using Utility::remove_from_back ;
        KeyLogger lgr ;
        std::vector<unsigned char> matched_num(points.size(), false) ;

        KeyAbsorber::InstantKeyAbsorber ika ;

        bool at_least_exist = false ;

        const auto hwnd = GetForegroundWindow() ;

        while(win_vind::update_background()) {
            EsyClk::draw_identifiers(
                    EsyClk::g_objpos,
                    EsyClk::g_hints_str,
                    matched_num,
                    at_least_exist) ;

            if(!KyLgr::log_as_char(lgr)) {
                remove_from_back(lgr, 1) ;
                continue ;
            }

            if(lgr.back().empty()) {
                remove_from_back(lgr, 1) ;
                continue ;
            }

            if(lgr.back().is_containing(VKC_ESC)) {
                Utility::refresh_display(hwnd) ;
                return ;
            }

            //initialize flags
            std::fill(matched_num.begin(), matched_num.end(), 0) ;
            at_least_exist = false ;

            if(lgr.back().is_containing(VKC_BKSPACE)) {
                if(lgr.size() == 1) {
                    Utility::refresh_display(hwnd) ;
                    return ;
                }
                remove_from_back(lgr, 2) ;
                continue ;
            }

            if(KeyBinder::is_invalid_log(lgr.back(), KeyBinder::AllSystemKey)) {
                remove_from_back(lgr, 1) ;
                continue ;
            }

            for(std::size_t i = 0 ; i < hints.size() ; i ++) {
                for(std::size_t j = 0 ; j < lgr.size() ; j ++) {
                    try {
                        if(!lgr[j].is_containing(hints[i].at(j))) {
                            break ;
                        }
                        matched_num[i] ++ ;

                        if(j == lgr.size() - 1) {
                            at_least_exist = true ;
                        }
                    }
                    catch(const std::out_of_range& e) {
                        break ;
                    }
                }

                if(matched_num[i] == hints[i].size()) {
                    SetCursorPos(points[i].x(), points[i].y()) ;
                    MouseEventer::click(VKC_MOUSE_LEFT) ;
                    Utility::refresh_display(hwnd) ;
                    return ;
                }
            }

            if(!at_least_exist) {
                remove_from_back(lgr, 1) ;
            }
            else if(lgr.size() == 1){
                Utility::refresh_display(hwnd) ;
            }
        }
    }
}

//EasyClick
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

        EsyClk::g_hints = EsyClk::assign_identifiers_label(EsyClk::g_objpos.size()) ;
        EsyClk::g_hints_str = EsyClk::convert_hints_to_str(EsyClk::g_hints) ;
    }

    EsyClk::update_font() ;
    if(!EsyClk::g_objpos.empty()) {
        EsyClk::loop_for_key_matching(EsyClk::g_objpos, EsyClk::g_hints) ;
    }

    for(auto& key : KeyAbsorber::get_pressed_list()) {
        KeyAbsorber::release_virtually(key) ;
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

    EsyClk::g_hints = EsyClk::assign_identifiers_label(EsyClk::g_objpos.size()) ;
    EsyClk::g_hints_str = EsyClk::convert_hints_to_str(EsyClk::g_hints) ;
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
