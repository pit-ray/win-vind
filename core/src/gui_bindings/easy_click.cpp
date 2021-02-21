#include "easy_click.hpp"

#include "i_params.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "key_logger.hpp"
#include "keybrd_eventer.hpp"
#include "mouse_eventer.hpp"
#include "msg_logger.hpp"
#include "screen_metrics.hpp"
#include "uia.hpp"
#include "utility.hpp"
#include "virtual_key_fwd.hpp"
#include "vkc_converter.hpp"
#include "win_vind.hpp"

#include "disable_gcc_warning.hpp"
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>

#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include "enable_gcc_warning.hpp"


namespace EsyClk
{
    void print_hresult(HRESULT& res) ;
    inline static void draw_rect_by_center(LONG x, LONG y, LONG delta=10) ;

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

    static std::vector<Point2D> g_obj_points ;

    inline static auto& get_cache_req() {
        static auto g_cache_req = UIA::make_SmartCacheReq(nullptr) ;
        return g_cache_req ;
    }

    void initialize() {
        g_obj_points.reserve(2048) ;

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

                    g_obj_points.emplace_back(ppvdata[0], ppvdata[1]) ;
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

        if(!ScreenMetrics::is_fully_in_range(rect, window_rect)) {
            return ;
        }

        g_obj_points.emplace_back(
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

    static BOOL CALLBACK EnumThreadWndProc(HWND hwnd, LPARAM UNUSED(lparam)) {
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

        if(ScreenMetrics::width(rect) == 0 || ScreenMetrics::height(rect) == 0) {
            return TRUE ;
        }
        if(rect.left < 0 || rect.top < 0 || rect.right < 0 || rect.bottom < 0) {
            return TRUE ;
        }

        g_obj_points.emplace_back(
                rect.left + (rect.right - rect.left) / 2,
                rect.top + (rect.bottom - rect.top) / 2) ;
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

        //scan all GUI objects in current window and children
        scan_object_from_hwnd(hwnd) ;

        //enumerate all window owned by the foreground window process.
        DWORD procid ;
        if(!GetWindowThreadProcessId(hwnd, &procid)) {
            return ;
        }

        EnumWindows(EnumWindowsProc, static_cast<LPARAM>(procid)) ;

        Utility::remove_deplication(EsyClk::g_obj_points) ;
    }

    static HWND g_prehwnd = NULL ; 
    static RECT g_prerect = {0, 0, 0, 0} ;

    inline static bool need_update(HWND hwnd) {
        if(!iParams::get_b("enable_esyclk_hwnd_cache")) {
            return true ;
        }

        //If the location of the previous selected window is changed or other window is selected, must update.
        RECT rect ;
        if(hwnd == g_prehwnd) {
            if(GetWindowRect(hwnd, &rect)) {
                if(ScreenMetrics::is_equel(rect, g_prerect)) {
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
    static constexpr std::array<unsigned char, 26> gcx_labels = {
        VKC_A, VKC_S, VKC_D, VKC_G, VKC_H,
        VKC_K, VKC_L, VKC_Q, VKC_W, VKC_E,
        VKC_R, VKC_T, VKC_Y, VKC_U, VKC_I,
        VKC_O, VKC_P, VKC_Z, VKC_X, VKC_C,
        VKC_V, VKC_B, VKC_N, VKC_M, VKC_F, VKC_J
    } ;

    using hint_t = std::vector<unsigned char> ;
    static std::vector<hint_t> g_hints{} ;

    //Currrently, supported only 26 x 26 x 26 = 17576 patterns.
    inline static auto assign_identifiers_label(const std::size_t target_count) {
        // <= 26
        if(target_count <= gcx_labels.size()) {
            std::vector<hint_t> hints(target_count) ;
            for(std::size_t i = 0 ; i < target_count ; i ++) {
                hints[i].push_back(gcx_labels[i]) ;
            }
            return hints ;
        }

        // <= 26 * 26 (=676)
        static constexpr auto gcx_labels_size_pow2 = gcx_labels.size() * gcx_labels.size() ;
        if(target_count <= gcx_labels_size_pow2) {
            std::vector<hint_t> hints(target_count) ;

            std::size_t idx = 0 ;
            const auto l2_num = target_count / gcx_labels.size() + 1 ;
            for(auto i = l2_num ; i < gcx_labels.size() ; i ++) {
                hints[idx].push_back(gcx_labels[i]) ;
                idx ++ ;
            }

            for(std::size_t j = 0 ; j < l2_num ; j ++) {
                try {
                    for(std::size_t  i = 0 ; i < gcx_labels.size() ; i ++) {
                        hints.at(idx).push_back(gcx_labels[j]) ;
                        hints[idx].push_back(gcx_labels[i]) ;
                        idx ++ ;
                    }
                }
                catch(const std::out_of_range&) {break ;}
            }

            return hints ;
        }

        // <= 26 * 26 * 26 (=17576)
        static constexpr auto gcx_labels_size_pow3 = gcx_labels_size_pow2  * gcx_labels.size() ;
        std::vector<hint_t> hints ;
        if(target_count <= gcx_labels_size_pow3) {
            hints.resize(target_count) ;
        } else {
            hints.resize(gcx_labels_size_pow3) ;
        }
        const auto l3_num = target_count / gcx_labels_size_pow2 + 1 ;

        std::size_t idx = 0 ;
        for(std::size_t j = l3_num ; j < gcx_labels.size() ; j ++) {
            for(std::size_t i = 0 ; i < gcx_labels.size() ; i ++) {
                hints[idx].push_back(gcx_labels[j]) ;
                hints[idx].push_back(gcx_labels[i]) ;
                idx ++ ;
            }
        }

        for(std::size_t k = 0 ; k < l3_num ; k ++) {
            try {
                for(std::size_t j = 0 ; j < gcx_labels.size() ; j ++) {
                    for(std::size_t i = 0 ; i < gcx_labels.size() ; i ++) {
                        hints.at(idx).push_back(gcx_labels[k]) ;
                        hints[idx].push_back(gcx_labels[j]) ;
                        hints[idx].push_back(gcx_labels[i]) ;
                        idx ++ ;
                    }
                }
            }
            catch(const std::out_of_range&) {break ;}
        }

        return hints ;
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
    inline static void update_font() {
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

        //Handles
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

        //Colors
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

        if(SetBkColor(hdc.get(), bkcolor) == CLR_INVALID) {
            throw RUNTIME_EXCEPT("SetBkColor") ;
        }
        if(SetTextColor(hdc.get(), txcolor) == CLR_INVALID) {
            throw RUNTIME_EXCEPT("SetTextColor") ;
        }

        //Drawing
        auto draw = [&hdc, &delta] (auto&& str, auto&& point) {
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

        auto add_margin = [](const auto& str) {
            return " " + str + " " ;
        } ;

        if(!exist) {
            for(std::size_t i = 0 ; i < hints_str.size() ; i ++) {
                draw(add_margin(hints_str[i]), points[i]) ;
            }
        }
        else {
            for(std::size_t i = 0 ; i < hints_str.size() ; i ++) {
                if(matched_list[i] == 0) continue ;
                draw(add_margin(hints_str[i]), points[i]) ;
            }

            if(SetTextColor(hdc.get(), txcolor_ready) == CLR_INVALID) {
                throw RUNTIME_EXCEPT("SetTextColor") ;
            }

            //overdraw with the weak text color.
            for(std::size_t i = 0 ; i < hints_str.size() ; i ++) {
                if(matched_list[i] == 0) continue ;
                draw(" " + hints_str[i].substr(0, matched_list[i]), points[i]) ;
            }
        }
    }

    // [Return value]
    //   >= 0 : matched something
    //    < 0 : matched nothing
    inline static long match_with_hints(
            const KeyLogger& lgr,
            const std::vector<hint_t>& hints,
            std::vector<unsigned char>& matched_num_list,
            bool& at_least_exist) {

        if(lgr.empty()) {
            at_least_exist = false ;
            return -1 ;
        }

        for(std::size_t i = 0 ; i < hints.size() ; i ++) {
            std::size_t seq_idx ;
            for(seq_idx = 0 ; seq_idx < lgr.size() ; seq_idx ++) {
                try {
                    if(!lgr[seq_idx].is_containing(hints[i].at(seq_idx))) {
                        break ;
                    }
                }
                catch(const std::out_of_range&) {break ;}
            }

            if(seq_idx == lgr.size()) {
                at_least_exist = true ;
                matched_num_list[i] = seq_idx ;
            }
            else {
                matched_num_list[i] = 0 ;
            }

            if(seq_idx == hints[i].size()) {
                return i ;
            }
        }

        return -1 ;
    }

    inline static void loop_for_key_matching(
            const std::vector<Point2D>& points,
            const std::vector<hint_t>& hints,
            const std::vector<std::string>& hints_str,
            const unsigned char sendkey=VKC_UNDEFINED) {

        using Utility::remove_from_back ;
        KeyLogger lgr ;
        std::vector<unsigned char> matched_num_list(hints.size(), 0) ;

        KeyAbsorber::InstantKeyAbsorber ika ;

        bool at_least_exist = false ;
        const auto hwnd = GetForegroundWindow() ;

        using namespace std::chrono ;
        auto drawn_point = system_clock::now() ;
        static constexpr auto DRAW_INTERVAL_TIME = 600ms ;

        while(win_vind::update_background()) {

            // The drawing process is very heavy, so draw in the interval.
            if(system_clock::now() - drawn_point > DRAW_INTERVAL_TIME) {
                EsyClk::draw_identifiers(points, hints_str, matched_num_list, at_least_exist) ;
                drawn_point = system_clock::now() ;
            }

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
            std::fill(matched_num_list.begin(), matched_num_list.end(), 0) ;
            at_least_exist = false ;

            if(lgr.back().is_containing(VKC_BKSPACE)) {
                if(lgr.size() == 1) {
                    Utility::refresh_display(hwnd) ;
                    return ;
                }
                remove_from_back(lgr, 2) ;
                KeyAbsorber::release_virtually(VKC_BKSPACE) ;
                match_with_hints(lgr, hints, matched_num_list, at_least_exist) ; //update matching list
                continue ;
            }

            if(KeyBinder::is_invalid_log(lgr.back(), KeyBinder::AllSystemKey)) {
                remove_from_back(lgr, 1) ;
                continue ;
            }


            const auto full_match_idx = match_with_hints(lgr, hints, matched_num_list, at_least_exist) ;
            if(full_match_idx >= 0) {
                SetCursorPos(points[full_match_idx].x(), points[full_match_idx].y()) ;
                if(sendkey != VKC_UNDEFINED) {
                    MouseEventer::click(sendkey) ;
                }
                Utility::refresh_display(hwnd) ;
                return ;
            }

            if(!at_least_exist) {
                remove_from_back(lgr, 1) ;
            }
            else {
                Utility::refresh_display(hwnd) ;

                for(auto& key : lgr.back()) {
                    KeyAbsorber::release_virtually(key) ;
                }
            }
        }
    }

    inline static void common_process(const unsigned char sendkey) {
        if(need_update(GetForegroundWindow())) {
            g_obj_points.clear() ;
            scan_gui_objects() ;

            g_hints     = assign_identifiers_label(g_obj_points.size()) ;
            g_hints_str = convert_hints_to_str(g_hints) ;
        }

        if(!g_obj_points.empty()) {
            update_font() ;
            loop_for_key_matching(g_obj_points, g_hints, g_hints_str, sendkey) ;
        }

        //release all keys
        for(auto& key : KeyAbsorber::get_pressed_list()) {
            KeyAbsorber::release_virtually(key) ;
        }
    }
}

//EasyClickLeft
const std::string EasyClickLeft::sname() noexcept
{
    return "easy_click_left" ;
}
void EasyClickLeft::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) {
    if(first_call) {
        EsyClk::common_process(VKC_MOUSE_LEFT) ;
    }
}

//EasyClickRight
const std::string EasyClickRight::sname() noexcept
{
    return "easy_click_right" ;
}
void EasyClickRight::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) {
    if(first_call) {
        EsyClk::common_process(VKC_MOUSE_RIGHT) ;
    }
}

//EasyClickMid
const std::string EasyClickMid::sname() noexcept
{
    return "easy_click_mid" ;
}
void EasyClickMid::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) {
    if(first_call) {
        EsyClk::common_process(VKC_MOUSE_MID) ;
    }
}

//EasyClickHover
const std::string EasyClickHover::sname() noexcept
{
    return "easy_click_hover" ;
}
void EasyClickHover::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) {
    if(first_call) {
        EsyClk::common_process(VKC_UNDEFINED) ;
    }
}


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
    using namespace EsyClk ;

    g_prehwnd = GetForegroundWindow() ;
    if(!GetWindowRect(g_prehwnd, &g_prerect)) {
        return ;
    }

    g_obj_points.clear() ;
    scan_gui_objects() ;

    g_hints     = assign_identifiers_label(g_obj_points.size()) ;
    g_hints_str = convert_hints_to_str(g_hints) ;
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
