#include "easy_click.hpp"

#include "display_text_painter.hpp"
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
#include <stdexcept>
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>

#include <array>
#include <chrono>
#include <cstring>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include "enable_gcc_warning.hpp"


namespace EasyClick
{
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

    inline static auto& get_cache_req() {
        static auto g_cache_req = UIA::make_SmartCacheReq(nullptr) ;
        return g_cache_req ;
    }

    static void scan_gui_objects(std::vector<Point2D>& obj_points) ;

    using hint_t = std::vector<unsigned char> ;

    static const std::vector<hint_t> assign_identifiers_label(const std::size_t target_count) ;
    inline static const std::vector<std::string> convert_hints_to_str(const std::vector<hint_t>& hints) ;

    static void loop_for_key_matching(
            HWND hwnd,
            const std::vector<Point2D>& points,
            const std::vector<hint_t>& hints,
            std::vector<unsigned char>& matching_nums,
            std::size_t& need_draw_count,
            std::mutex& mtx,
            const unsigned char sendkey=VKC_UNDEFINED) ;

    inline static void do_easy_click(const unsigned char sendkey=VKC_UNDEFINED) {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("There is not a foreground window.") ;
        }

        std::vector<Point2D> obj_points ;
        obj_points.reserve(2048) ;

        scan_gui_objects(obj_points) ;

        if(!obj_points.empty()) {
            const auto hints = assign_identifiers_label(obj_points.size()) ;
            const auto hints_str = convert_hints_to_str(hints) ;

            std::mutex l_mtx ;
            std::vector<unsigned char> matching_nums(hints.size(), 0) ;
            auto need_draw_count = hints.size() ;

            //create new thread for matching inputed keys with hints.
            auto input_ft = std::async(
                    std::launch::async,
                    loop_for_key_matching,
                    hwnd, std::cref(obj_points), std::cref(hints),
                    std::ref(matching_nums), std::ref(need_draw_count),
                    std::ref(l_mtx), sendkey) ;

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

            const auto fontsize = iParams::get_l("easy_click_font_size") ;
            DisplayTextPainter dtp(
                    fontsize,
                    iParams::get_l("easy_click_font_weight"),
                    "Consolas") ;
            dtp.set_back_color(bkcolor) ;
            dtp.set_text_color(txcolor) ;

            auto dtp_weak(dtp) ; //copy construct
            dtp_weak.set_text_color(txcolor_ready) ;

            // A detected positon is the center one of object.
            // And, TextOutA draws from a left-upper coordinate, so must move.
            auto align = [fontsize] (auto&& v) {
                return v - fontsize / 2 ;
            } ;

            auto add_margin = [](const auto& str) {
                return " " + str + " " ;
            } ;

            using namespace std::chrono ;
            while(input_ft.wait_for(50ms) == std::future_status::timeout) { //about 24 fps
                std::lock_guard<std::mutex> scoped_lock(l_mtx) ; //atomic ---------- (0)

                if(need_draw_count == hints.size()) {
                    for(std::size_t i = 0 ; i < hints_str.size() ; i ++) {
                        dtp.draw(add_margin(hints_str[i]),
                                align(obj_points[i].x()), align(obj_points[i].y()), 1) ;
                    }
                }
                else {
                    for(std::size_t i = 0 ; i < hints_str.size() ; i ++) {
                        if(matching_nums[i] == 0) {
                            continue ;
                        }
                        dtp.draw(add_margin(hints_str[i]),
                                align(obj_points[i].x()), align(obj_points[i].y()), 1) ;

                        //overdraw with the weak text color.
                        dtp_weak.draw(" " + hints_str[i].substr(0, matching_nums[i]),
                                align(obj_points[i].x()), align(obj_points[i].y()), 1) ;
                    }
                }

                dtp.refresh() ;
                dtp_weak.refresh() ;
                //------------------------------------------------------------------ (0)
            }

            Utility::refresh_display(NULL) ; //remove hints in display

            //release all keys
            for(unsigned char key : KeyAbsorber::get_pressed_list()) {
                KeyAbsorber::release_virtually(key) ;
            }
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
        EasyClick::do_easy_click(VKC_MOUSE_LEFT) ;
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
        EasyClick::do_easy_click(VKC_MOUSE_RIGHT) ;
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
        EasyClick::do_easy_click(VKC_MOUSE_MID) ;
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
        EasyClick::do_easy_click(VKC_UNDEFINED) ;
    }
}

namespace EasyClick {

    void initialize() {
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
    inline static auto get_clickable_point(UIA::SmartElement& elem) {
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

    inline static auto get_keyboard_focusable_point(
            UIA::SmartElement& elem,
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

        if(!ScreenMetrics::is_fully_in_range(rect, window_rect)) {
            throw std::runtime_error("The rectangle of element exists in a invalid range.") ;
        }

        return Point2D(
                rect.left + (rect.right - rect.left) / 2,
                rect.top  + (rect.bottom - rect.top) / 2) ;
    }

    static void scan_children_enumurately(
            UIA::SmartElementArray& parents,
            std::vector<Point2D>& obj_points,
            const RECT& window_rect,
            const BOOL parent_is_focusable=FALSE) {
        int length ;
        parents->get_Length(&length) ;
        if(length == 0) {
            return ;
        }

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
                scan_children_enumurately(children, obj_points, window_rect, flag) ; //recursive calling
                continue ;
            }

            //scan GUI objects only at leaves in tree.
            //get_clickable_point(elem) ;
            try {
                obj_points.push_back(get_keyboard_focusable_point(elem, window_rect, parent_is_focusable)) ;
            }
            catch(const std::runtime_error&) {
                continue ;
            }
        }
    }

    static void scan_object_from_hwnd(HWND hwnd, std::vector<Point2D>& obj_points) {
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

        /* multi thread scan ?
        auto ft = std::async(std::launch::async, [&elem, &elem_raw] {
                return elem->BuildUpdatedCache(get_cache_req().get(), &elem_raw) ;
        }) ;
        if(FAILED(ft.get())) {
            throw RUNTIME_EXCEPT("Could not update caches of UIAutomationElement.") ;
        }
        */
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

    static BOOL CALLBACK ScanCenterPoint(HWND hwnd, LPARAM lparam) {
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

        if(ScreenMetrics::width(rect) == 0 || ScreenMetrics::height(rect) == 0) {
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

    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam) {
        auto psinfo = reinterpret_cast<ProcessScanInfo*>(lparam) ;

        DWORD procid ;
        auto thid = GetWindowThreadProcessId(hwnd, &procid) ;

        if(procid == psinfo->pid) {
            //enumerate all threads owned by the parent process.
            EnumThreadWindows(thid, ScanCenterPoint, reinterpret_cast<LPARAM>(&(psinfo->points))) ;
        }

        return TRUE ;
    }

    static void scan_gui_objects(std::vector<Point2D>& obj_points) {
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

        Utility::remove_deplication(obj_points) ;
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

    //Currrently, supported only 26 x 26 x 26 = 17576 patterns.
    static const std::vector<hint_t> assign_identifiers_label(const std::size_t target_count) {
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

    inline static const std::vector<std::string> convert_hints_to_str(const std::vector<hint_t>& hints) {
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

    // [Return value] count that need to draw
    inline static std::size_t match_with_hints(
            const KeyLogger& lgr,
            const std::vector<hint_t>& hints,
            std::vector<unsigned char>& matching_nums,
            long* p_matched_index=nullptr) {

        if(p_matched_index) {
            *p_matched_index = -1 ;
        }

        if(lgr.empty()) {
            return hints.size() ; //all is matched
        }

        std::size_t draw_count = 0 ;
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
                draw_count ++ ;
                matching_nums[i] = static_cast<unsigned char>(seq_idx) ;
            }
            else {
                matching_nums[i] = 0 ;
            }

            if(seq_idx == hints[i].size()) {
                if(p_matched_index)
                    *p_matched_index = static_cast<long>(i) ;
                return 1 ;
            }
        }

        return draw_count ;
    }

    static void loop_for_key_matching(
            HWND hwnd,
            const std::vector<Point2D>& points,
            const std::vector<hint_t>& hints,
            std::vector<unsigned char>& matching_nums,
            std::size_t& need_draw_count,
            std::mutex& mtx,
            const unsigned char sendkey) {

        using Utility::remove_from_back ;
        KeyAbsorber::InstantKeyAbsorber ika ;
        KeyLogger lgr ;

        while(win_vind::update_background()) {
            if(!KyLgr::log_as_char(lgr)) {
                remove_from_back(lgr, 1) ;
                continue ;
            }

            if(lgr.back().empty()) {
                remove_from_back(lgr, 1) ;
                continue ;
            }

            if(lgr.back().is_containing(VKC_ESC)) {
                return ;
            }

            if(lgr.back().is_containing(VKC_BKSPACE)) {
                if(lgr.size() == 1) {
                    return ;
                }

                remove_from_back(lgr, 2) ;
                KeyAbsorber::release_virtually(VKC_BKSPACE) ;

                std::lock_guard<std::mutex> scoped_lock(mtx) ; // atomic ---------- (1)

                need_draw_count = match_with_hints(lgr, hints, matching_nums) ; //update matching list

                continue ; //------------------------------------------------------ (1)
            }

            if(KeyBinder::is_invalid_log(lgr.back(), KeyBinder::AllSystemKey)) {
                remove_from_back(lgr, 1) ;
                continue ;
            }

            std::lock_guard<std::mutex> scoped_lock(mtx) ; // atomic -------------- (2)

            long full_match_idx ;
            need_draw_count = match_with_hints(lgr, hints, matching_nums, &full_match_idx) ;

            if(full_match_idx >= 0) {
                SetCursorPos(points[full_match_idx].x(), points[full_match_idx].y()) ;
                if(sendkey != VKC_UNDEFINED) {
                    MouseEventer::click(sendkey) ;
                }
                return ;
            }

            if(need_draw_count == 0) {
                remove_from_back(lgr, 1) ;
            }
            else {
                for(const auto& key : lgr.back()) {
                    KeyAbsorber::release_virtually(key) ;
                }

                Utility::refresh_display(hwnd) ;
            }

            //--------------------------------------------------------------------- (2)
        }
    }
}
