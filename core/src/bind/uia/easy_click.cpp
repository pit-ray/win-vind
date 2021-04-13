#include "bind/uia/easy_click.hpp"

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

#include "bind/bind.hpp"
#include "bind/uia/uia.hpp"
#include "bind/base/ntype_logger.hpp"
#include "bind/base/keycode_logger.hpp"
#include "coreio/err_logger.hpp"
#include "coreio/i_params.hpp"
#include "io/display_text_painter.hpp"
#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "io/screen_metrics.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"
#include "util/color.hpp"
#include "util/container.hpp"
#include "util/winwrap.hpp"
#include "entry.hpp"


namespace
{
    using namespace vind ;

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

    inline auto& get_cache_req() {
        static auto g_cache_req = uiauto::make_SmartCacheReq(nullptr) ;
        return g_cache_req ;
    }

    void scan_gui_objects(std::vector<Point2D>& obj_points) ;

    using hint_t = std::vector<unsigned char> ;

    const std::vector<hint_t> assign_identifiers_label(const std::size_t target_count) ;
    const std::vector<std::string> convert_hints_to_str(const std::vector<hint_t>& hints) ;

    void loop_for_key_matching(
            HWND hwnd,
            const std::vector<Point2D>& points,
            const std::vector<hint_t>& hints,
            std::vector<unsigned char>& matching_nums,
            std::size_t& need_draw_count,
            std::mutex& mtx,
            const bool& continue_running,
            const unsigned char sendkey=KEYCODE_UNDEFINED) ;

    void do_easy_click(const unsigned char sendkey=KEYCODE_UNDEFINED) {
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

            bool continue_running = true ;

            //create new thread for matching inputed keys with hints.
            auto input_ft = std::async(
                    std::launch::async,
                    loop_for_key_matching,
                    hwnd, std::cref(obj_points), std::cref(hints),
                    std::ref(matching_nums), std::ref(need_draw_count),
                    std::ref(l_mtx), std::cref(continue_running), sendkey) ;

            try {
                //Colors
                auto [bk_r, bk_g, bk_b] = util::hex2rgb(iparams::get_s("easy_click_font_bkcolor")) ;
                auto bkcolor = RGB(bk_r, bk_g, bk_b) ;

                auto [tx_r, tx_g, tx_b] = util::hex2rgb(iparams::get_s("easy_click_font_color")) ;
                auto txcolor = RGB(tx_r, tx_g, tx_b) ;

                const unsigned char decay = iparams::get_uc("easy_click_matching_color_decay") ;
                using util::to_gray ;
                char sign = to_gray(tx_r, tx_g, tx_b) > to_gray(bk_r, bk_g, bk_b) ? -1 : 1 ;

                auto txcolor_ready = RGB(
                        tx_r < decay ? 0 : tx_r + sign*decay,
                        tx_g < decay ? 0 : tx_g + sign*decay,
                        tx_b < decay ? 0 : tx_b + sign*decay) ;

                const auto fontsize = iparams::get_l("easy_click_font_size") ;
                DisplayTextPainter dtp(
                        fontsize,
                        iparams::get_l("easy_click_font_weight"),
                        iparams::get_s("easy_click_font_name")) ;
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
                while(input_ft.wait_for(50ms) == std::future_status::timeout) {
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
            }
            catch(const std::exception& e) {
                continue_running = false ;
                throw e ;
            }

            util::refresh_display(NULL) ; //remove hints in display

            //release all keys
            for(unsigned char key : keyabsorber::get_pressed_list()) {
                keyabsorber::release_virtually(key) ;
            }
        }
    }

}

namespace vind
{
    //EasyClickLeft
    const std::string EasyClickLeft::sname() noexcept {
        return "easy_click_left" ;
    }
    void EasyClickLeft::sprocess() {
        do_easy_click(KEYCODE_MOUSE_LEFT) ;
    }
    void EasyClickLeft::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //EasyClickRight
    const std::string EasyClickRight::sname() noexcept {
        return "easy_click_right" ;
    }
    void EasyClickRight::sprocess() {
        do_easy_click(KEYCODE_MOUSE_RIGHT) ;
    }
    void EasyClickRight::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickRight::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //EasyClickMid
    const std::string EasyClickMid::sname() noexcept {
        return "easy_click_mid" ;
    }
    void EasyClickMid::sprocess() {
        do_easy_click(KEYCODE_MOUSE_MID) ;
    }
    void EasyClickMid::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickMid::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //EasyClickHover
    const std::string EasyClickHover::sname() noexcept {
        return "easy_click_hover" ;
    }
    void EasyClickHover::sprocess() {
        do_easy_click(KEYCODE_UNDEFINED) ;
    }
    void EasyClickHover::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickHover::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    namespace easyclick {

        void initialize() {
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
    }
}

namespace
{
    // ----------------------------
    //
    //    @GUI  Scan  Functions
    //
    // ----------------------------

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


    // ---------------------------------------
    //
    //    @Identifier  Assign  Functions
    //
    // ---------------------------------------
    constexpr std::array<unsigned char, 26> gcx_labels = {
        KEYCODE_A, KEYCODE_S, KEYCODE_D, KEYCODE_G, KEYCODE_H,
        KEYCODE_K, KEYCODE_L, KEYCODE_Q, KEYCODE_W, KEYCODE_E,
        KEYCODE_R, KEYCODE_T, KEYCODE_Y, KEYCODE_U, KEYCODE_I,
        KEYCODE_O, KEYCODE_P, KEYCODE_Z, KEYCODE_X, KEYCODE_C,
        KEYCODE_V, KEYCODE_B, KEYCODE_N, KEYCODE_M, KEYCODE_F, KEYCODE_J
    } ;

    //Currrently, supported only 26 x 26 x 26 = 17576 patterns.
    const std::vector<hint_t> assign_identifiers_label(const std::size_t target_count) {
        // <= 26
        if(target_count <= gcx_labels.size()) {
            std::vector<hint_t> hints(target_count) ;
            for(std::size_t i = 0 ; i < target_count ; i ++) {
                hints[i].push_back(gcx_labels[i]) ;
            }
            return hints ;
        }

        // <= 26 * 26 (=676)
        constexpr auto gcx_labels_size_pow2 = gcx_labels.size() * gcx_labels.size() ;
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

    const std::vector<std::string> convert_hints_to_str(const std::vector<hint_t>& hints) {
        std::vector<std::string> hints_str(hints.size()) ;

        for(std::size_t i = 0 ; i < hints.size() ; i ++) {
            std::string str ;
            for(auto& key : hints[i]) {
                str.push_back(keycodecvt::get_shifted_ascii(key)) ;
            }
            hints_str[i] = std::move(str) ;
        }
        return hints_str ;
    }

    // [Return value] count that need to draw
    std::size_t match_with_hints(
            const KeyLoggerBase* const pc_lgr,
            const std::vector<hint_t>& hints,
            std::vector<unsigned char>& matching_nums,
            long* p_matched_index=nullptr) {

        if(p_matched_index) {
            *p_matched_index = -1 ;
        }

        if(pc_lgr->empty()) {
            return hints.size() ; //all is matched
        }

        std::size_t draw_count = 0 ;
        for(std::size_t i = 0 ; i < hints.size() ; i ++) {
            std::size_t seq_idx ;
            for(seq_idx = 0 ; seq_idx < pc_lgr->size() ; seq_idx ++) {
                try {
                    if(!pc_lgr->at(seq_idx).is_containing(hints[i].at(seq_idx))) {
                        break ;
                    }
                }
                catch(const std::out_of_range&) {break ;}
            }

            if(seq_idx == pc_lgr->size()) {
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

    void loop_for_key_matching(
            HWND hwnd,
            const std::vector<Point2D>& points,
            const std::vector<hint_t>& hints,
            std::vector<unsigned char>& matching_nums,
            std::size_t& need_draw_count,
            std::mutex& mtx,
            const bool& continue_running,
            const unsigned char sendkey) {

        keyabsorber::InstantKeyAbsorber ika ;
        KeycodeLogger lgr ;

        while(vind::update_background() && continue_running) {
            lgr.update() ;
            if(!lgr.is_changed()) {
                continue ;
            }

            if(lgr.latest().empty()) {
                lgr.remove_from_back(1) ;
                continue ;
            }

            if(lgr.latest().is_containing(KEYCODE_ESC)) {
                return ;
            }

            if(lgr.latest().is_containing(KEYCODE_BKSPACE)) {
                if(lgr.size() == 1) {
                    return ;
                }

                lgr.remove_from_back(2) ;
                keyabsorber::release_virtually(KEYCODE_BKSPACE) ;

                std::lock_guard<std::mutex> scoped_lock(mtx) ; // atomic ---------- (1)

                need_draw_count = match_with_hints(&lgr, hints, matching_nums) ; //update matching list

                continue ; //------------------------------------------------------ (1)
            }

            /*
            if(keybind::is_invalid_log(lgr.latest(), keybind::InvalidPolicy::AllSystemKey)) {
                lgr.remove_from_back(1) ;
                continue ;
            }
            */

            std::lock_guard<std::mutex> scoped_lock(mtx) ; // atomic -------------- (2)

            long full_match_idx ;
            need_draw_count = match_with_hints(&lgr, hints, matching_nums, &full_match_idx) ;

            if(full_match_idx >= 0) {
                SetCursorPos(points[full_match_idx].x(), points[full_match_idx].y()) ;
                if(sendkey != KEYCODE_UNDEFINED) {
                    mouse::click(sendkey) ;
                }
                return ;
            }

            if(need_draw_count == 0)
                lgr.remove_from_back(1) ;
            else
                util::refresh_display(hwnd) ;
            //--------------------------------------------------------------------- (2)
        }
    }
}
