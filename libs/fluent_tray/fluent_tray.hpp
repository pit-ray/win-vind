/**
 * @file fluent_tray.hpp
 * @brief Fluent Design-based GUI Library for System Tray Applications
 * @author pit-ray
 * @date 2024
 *
 **/

/*
 * MIT License
 *
 * Copyright (c) 2024 pit-ray
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _FLUENT_TRAY_HPP
#define _FLUENT_TRAY_HPP

#include <windows.h>

#if defined(_MSC_VER) && _MSC_VER >= 1500
#pragma warning(disable : 4005)
#endif

#include <ntstatus.h>

#if defined(_MSC_VER) && _MSC_VER >= 1500
#pragma warning(default : 4005)
#endif

#include <dwmapi.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef DWMWA_COLOR_DEFAULT
#define DWMWA_WINDOW_CORNER_PREFERENCE static_cast<DWMWINDOWATTRIBUTE>(33)
typedef enum {
    DWMWCP_DEFAULT = 0,
    DWMWCP_DONOTROUND = 1,
    DWMWCP_ROUND = 2,
    DWMWCP_ROUNDSMALL = 3
} DWM_WINDOW_CORNER_PREFERENCE;
#endif /* DWMWA_COLOR_DEFAULT */

#if defined(DEBUG)
#include <iostream>
#endif

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#pragma comment(lib, "Dwmapi")

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <memory>
#include <string>
#include <vector>


/**
 * @namespace fluent_tray
 * @brief Base namespace
 */
namespace fluent_tray
{
    /**
     * @namespace fluent_tray::util
     * @brief Utility namespace
     */
    namespace util
    {
        /**
         * @brief Converts a UTF-8 encoded string to a wide string.
         * @param [in]   str a UTF-8 encoded string object.
         * @param [out] wstr a output wide string object.
         * @return Returns true on success, false on failure.
         * @sa wstring2string
         */
        bool string2wstring(const std::string& str, std::wstring& wstr) {
            if(str.empty()) {
                wstr.clear() ;
                return true ;
            }

            auto needed_size = MultiByteToWideChar(
                    CP_UTF8, 0,
                    str.c_str(), static_cast<int>(str.length()),
                    NULL, 0) ;
#ifndef _FLUENT_TRAY_IGNORE_IN_TEST
            if(needed_size <= 0) {
                return false ;
            }
#endif

            wstr.resize(needed_size) ;
            if(MultiByteToWideChar(
                        CP_UTF8, 0,
                        str.c_str(), static_cast<int>(str.length()),
                        &wstr[0], static_cast<int>(wstr.length())) <= 0) {
#ifndef _FLUENT_TRAY_IGNORE_IN_TEST
                return false;
#endif
            }
            return true ;
        }

        /**
         * @brief Converts a wide string to a UTF-8 encoded string.
         * @param [in] wstr a output wide string object.
         * @param [out] str a UTF-8 encoded string object.
         * @return Returns true on success, false on failure.
         * @sa string2wstring
         */
        bool wstring2string(const std::wstring& wstr, std::string& str) {
            if(wstr.empty()) {
                str.clear() ;
                return true ;
            }

            auto needed_size = WideCharToMultiByte(
                    CP_UTF8, 0,
                    wstr.c_str(), static_cast<int>(wstr.length()),
                    NULL, 0,
                    NULL, NULL) ;
#ifndef _FLUENT_TRAY_IGNORE_IN_TEST
            if(needed_size <= 0) {
                return false ;
            }
#endif

            str.resize(needed_size) ;
            if(WideCharToMultiByte(
                        CP_UTF8, 0,
                        wstr.c_str(), static_cast<int>(wstr.length()),
                        &str[0], static_cast<int>(str.size()),
                        NULL, NULL) <= 0) {
#ifndef _FLUENT_TRAY_IGNORE_IN_TEST
                return false ;
#endif
            }
            return true ;
        }

        /**
         * @brief Generates a mask with the specified number of lower bits set to 1.
         * @param [in] bits Number of bits
         * @return A generated bit mask.
         */
        inline constexpr std::size_t bit2mask(std::size_t bits) noexcept {
            return (static_cast<std::size_t>(1) << bits) - 1 ;
        }

        /**
         * @brief Calculate the number of bits of type
         * @return The number of bits.
         */
        template <typename Type>
        inline constexpr int type2bit() noexcept {
            return static_cast<int>(sizeof(Type) * CHAR_BIT) ;
        }

        /**
         * @brief Divides the input value into upper and lower bits.
         * @param [in] input A input value.
         * @param [out] upper A output upper bits.
         * @param [out] lower A outputs lower bits.
         * @details The splits is performed by storing the lower bits of number of bits of OutType in lower and the remaining bits in upper.
         * @sa concatenate_bits
         */
        template <typename InType, typename OutType>
        inline void split_bits(InType input, OutType& upper, OutType& lower) noexcept {
            static constexpr auto bits = type2bit<OutType>() ;
            static const auto lower_mask = util::bit2mask(bits) ;

            upper = static_cast<OutType>(reinterpret_cast<std::size_t>(input) >> bits) ;
            lower = static_cast<OutType>(reinterpret_cast<std::size_t>(input) & lower_mask) ;
        }

        /**
         * @brief Generates a variable that combines the bits of two variables.
         * @param [in] upper A input upper bits.
         * @param [in] lower A input lower bits.
         * @param [out] out A output value.
         * @sa split_bits
         */
        template <typename InType, typename OutType>
        inline void concatenate_bits(InType upper, InType lower, OutType& out) noexcept {
            static constexpr auto bits = type2bit<InType>() ;
            static const auto lower_mask = util::bit2mask(bits) ;

            auto out_upper = static_cast<std::size_t>(upper) << bits ;
            auto out_lower = static_cast<std::size_t>(lower) & lower_mask ;
            out = reinterpret_cast<OutType>(out_upper | out_lower) ;
        }
        /**
         * @brief Calculate grayscale value from RGB
         * @param [in] rgb A input rgb value.
         * @return The grayscale value.
         */
        inline unsigned char rgb2gray(const COLORREF& rgb) {
            auto r = GetRValue(rgb) ;
            auto g = GetGValue(rgb) ;
            auto b = GetBValue(rgb) ;
            return static_cast<unsigned char>(0.2126 * r + 0.7152 * g + 0.0722 * b) ;
        }

        /**
         * @brief Checks if the file exists.
         * @param [in] path A input wide string of path.
         * @return Returns true if the file exists, false if it does not.
         */
        inline bool exists(const std::wstring& path) {
            struct _stat buffer ;
            return _wstat(path.c_str(), &buffer) == 0 ;
        }
    }

    /**
     * @brief Current tray status
     */
    enum class TrayStatus : unsigned char
    {
        //! The tray is working properly.
        RUNNING,

        //! The tray is trying to exit successfully.
        SHOULD_STOP,

        //! The tray has errors.
        FAILED,

        //! The tray is stopped successfully.
        STOPPED,
    } ;

    /**
     * @brief Class with information on each menu.
     */
    class FluentMenu {
    private:
        std::wstring label_ ;
        HICON hicon_ ;

        bool toggleable_ ;
        bool checked_ ;
        std::wstring checkmark_ ;

        HWND hwnd_ ;
        HMENU hmenu_ ;

        bool under_line_ ;

        COLORREF text_color_ ;
        COLORREF back_color_ ;
        COLORREF border_color_ ;
        HBRUSH back_brush_ ;

        std::function<bool(void)> callback_ ;
        std::function<bool(void)> unchecked_callback_ ;

    public:
        /**
         * @brief Create menu object.
         * @param [in] toggleable Create a switchable menu
         * @param [in] callback Function called when a click on the menu or a check is enabled.
         * @param [in] unchecked_callback Function called when a check is disabled.
         * @details The callback function must be a function with a bool return value and no arguments. The tray will exit successfully if the callback function returns false.
         */
        explicit FluentMenu(
            bool toggleable=false,
            const std::function<bool(void)>& callback=[] {return true ;},
            const std::function<bool(void)>& unchecked_callback=[] {return true ;})
        : label_(),
          hicon_(NULL),
          toggleable_(toggleable),
          checked_(false),
          checkmark_(),
          hwnd_(NULL),
          hmenu_(NULL),
          under_line_(false),
          text_color_(RGB(0, 0, 0)),
          back_color_(RGB(255, 255, 255)),
          border_color_(RGB(128, 128, 128)),
          back_brush_(NULL),
          callback_(callback),
          unchecked_callback_(unchecked_callback)
        {}

        FluentMenu(const FluentMenu&) = default ;
        FluentMenu& operator=(const FluentMenu&) = default ;

        FluentMenu(FluentMenu&&) = default ;
        FluentMenu& operator=(FluentMenu&&) = default ;

        ~FluentMenu() noexcept {
            if(back_brush_) {
                DeleteObject(back_brush_) ;
            }
        }

        /**
         * @brief Creates a menu window.
         * @param [in] hinstance An instance handle of application.
         * @param [in] parent_hwnd A window handle of tray.
         * @param [in] id A new unique identifier.
         * @param [in] label_text A label text.
         * @param [in] icon_path An icon path to show next to the label.
         * @param [in] checkmark A checkmark string.
         * @return Returns true on success, false on failure.
         */
        bool create_menu(
                HINSTANCE hinstance,
                HWND parent_hwnd,
                std::size_t id,
                const std::string& label_text="",
                const std::string& icon_path="",
                const std::string& checkmark="✓") {

            // Convert strings to the wide-strings
            if(!util::string2wstring(label_text, label_)) {
                return false ;
            }
            if(!util::string2wstring(checkmark, checkmark_)) {
                return false ;
            }

            auto style = WS_CHILD | WS_VISIBLE | BS_FLAT | BS_LEFT | BS_OWNERDRAW ;

            hmenu_ = reinterpret_cast<HMENU>(id) ;
            hwnd_ = CreateWindowW(
                TEXT("BUTTON"), label_.c_str(), style,
                0, 0, 100, 100,
                parent_hwnd, hmenu_,
                hinstance, NULL) ;
            if(!hwnd_) {
                return false ;
            }

            // Hide dash lines when selecting.
            SendMessageW(
                hwnd_, WM_CHANGEUISTATE,
                WPARAM(MAKELONG(UIS_SET, UISF_HIDEFOCUS)), 0) ;

            if(!icon_path.empty()) {
                std::wstring icon_path_wide ;
                if(!util::string2wstring(icon_path, icon_path_wide)) {
                    return false ;
                }

                if(!util::exists(icon_path_wide)) {
                    return false ;
                }

                hicon_ = static_cast<HICON>(LoadImageW(
                        NULL, icon_path_wide.c_str(),
                        IMAGE_ICON, 0, 0, LR_LOADFROMFILE)) ;
                if(!hicon_) {
                    return false ;
                }
            }

            return true ;
        }

        /**
         * @brief Execute the process when clicked on the menu.
         * @return Returns true on success, false on failure.
         *
         */
        bool process_click_event() {
            if(toggleable_) {
                checked_ = !checked_ ;
                if(!checked_) {
                    return unchecked_callback_() ;
                }
            }
            return callback_() ;
        }

        /**
         * @brief Checks the menu if it is toggleable.
         * @details Update only the current state without calling the callback function.
         */
        void check() noexcept {
            if(toggleable_) {
                checked_ = true ;
            }
        }

        /**
         * @brief Unchecks the menu if it is toggleable.
         * @details Update only the current state without calling the callback function.
         */
        void uncheck() noexcept {
            if(toggleable_) {
                checked_ = false ;
            }
        }

        /**
         * @brief Refer to the check status of the menu
         * @return If the menu is toggleable, returns whether it is checked or not, and always returns false if it is not toggleable.
         */
        bool is_checked() const noexcept {
            if(!toggleable_) {
                return false ;
            }
            return checked_ ;
        }

        /**
         * @brief Check if the menu is toggleable.
         * @return Returns true if the menu is toggleable, false otherwise.
         */
        bool is_toggleable() const noexcept {
            return toggleable_ ;
        }

        /**
         * @brief Refer to the menu window handle.
         * @return The window handle.
         */
        HWND window_handle() const noexcept {
            return hwnd_ ;
        }

        /**
         * @brief Refer to the menu handle.
         * @return The menu handle.
         */
        HMENU menu_handle() const noexcept {
            return hmenu_ ;
        }

        /**
         * @brief Refer to the menu identifier.
         * @return The identifier.
         */
        std::size_t id() const noexcept {
            return reinterpret_cast<std::size_t>(hmenu_) ;
        }
        /**
         * @brief Get menu label as UTF-8 string.
         * @param [out] str The output string.
         * @return Returns true on success, false on failure.
         */
        bool get_label(std::string& str) const {
            return util::wstring2string(label_, str) ;
        }

        /**
         * @brief Show a separator line under the menu.
         */
        void show_separator_line() noexcept {
            under_line_ = true ;
        }

        /**
         * @brief Hide a separator line under the menu.
         */
        void hide_separator_line() noexcept {
            under_line_ = false ;
        }

        /**
         * @brief Set the menu color.
         * @param [in] text_color The color for label text.
         * @param [in] back_color The color for background.
         * @param [in] border_color The color for separator line.
         * @return Returns true on success, false on failure.
         * @details If the set value is CLR_INVALID, the value is not changed.
         */
        bool set_color(
                const COLORREF& text_color=CLR_INVALID,
                const COLORREF& back_color=CLR_INVALID,
                const COLORREF& border_color=CLR_INVALID) noexcept {
            if(text_color != CLR_INVALID) {
                text_color_ = text_color ;
            }
            if(back_color != CLR_INVALID) {
                back_color_ = back_color ;
            }
            if(border_color != CLR_INVALID) {
                border_color_ = border_color ;
            }

            // Create brush handle to draw a background of window.
            if(back_brush_) {
                // Release old handle.
                if(!DeleteObject(back_brush_)) {
                    return false ;
                }
            }
            back_brush_ = CreateSolidBrush(back_color_) ;
            if(back_brush_ == NULL) {
                return false ;
            }

            return true ;
        }

        /**
         * @brief Draws a menu using drawing information and the specified font.
         * @param [in] info Structure that holds drawing information.
         * @param [in] font The handle of font.
         * @return Returns true on success, false on failure.
         * @details info is the <a href="https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-drawitemstruct">DRAWITEMSTRUCT</a> obtained when the owner window receives a <a href="https://learn.microsoft.com/en-us/windows/win32/controls/wm-drawitem">WM_DRAWITEM</a> message.
         */
        bool draw_menu(LPDRAWITEMSTRUCT info, HFONT font) const {
            if(SetTextColor(info->hDC, text_color_) == CLR_INVALID) {
                return false ;
            }

            if(SetBkColor(info->hDC, back_color_) == CLR_INVALID) {
                return false ;
            }

            if(font) {
                if(!SelectObject(info->hDC, font)) {
                    return false ;
                }
            }

            LONG checkmark_size, label_height, icon_size, margin ;
            if(!calculate_layouts(
                    info->hDC,
                    checkmark_size, label_height, icon_size, margin)) {
                return false ;
            }

            auto& rect = info->rcItem ;

            auto y_center = rect.top + (rect.bottom - rect.top) / 2 ;
            auto x = rect.left + margin ;

            if(toggleable_ && checked_) {
                if(!TextOutW(
                        info->hDC, x, y_center - label_height / 2, checkmark_.c_str(),
                        static_cast<int>(checkmark_.length()))) {
                    return false ;
                }
            }
            x += checkmark_size + margin ;

            if(hicon_) {
                if(!DrawIconEx(
                        info->hDC, x, y_center - SM_CYICON / 2, hicon_,
                        icon_size, icon_size, 0, NULL, DI_NORMAL)) {
                    return false ;
                }
            }
            x += icon_size + margin ;

            if(!TextOutW(
                    info->hDC, x, y_center - label_height / 2, label_.c_str(),
                    static_cast<int>(label_.length()))) {
                return false ;
            }

            if(under_line_) {
                auto original_obj = SelectObject(info->hDC, GetStockObject(DC_PEN)) ;
                if(SetDCPenColor(info->hDC, border_color_) == CLR_INVALID) {
                    return false ;
                }

                auto lx = info->rcItem.left ;
                auto ly = info->rcItem.bottom - 1 ;
                auto rx = info->rcItem.right ;
                auto ry = ly + 1 ;
                if(!Rectangle(info->hDC, lx, ly, rx, ry)) {
                    return false ;
                }

                if(!SelectObject(info->hDC, original_obj)) {
                    return false ;
                }
            }

            return true ;
        }
        /**
         * @brief Refer to the brush for drawing the background.
         * @return The handle of brush.
         * @details Used for the return value of <a href="https://learn.microsoft.com/en-us/windows/win32/controls/wm-ctlcolorbtn">WM_CTLCOLORBTN</a> message.
         */
        HBRUSH background_brush() const noexcept {
            return back_brush_ ;
        }
        /**
         * @brief Calculates the size of the bounding box surrounding the menu based on the font information and the length of the label.
         * @param [in] font The handle of font.
         * @param [out] size The output width and height dimensions.
         * @return Returns true on success, false on failure.
         */
        bool calculate_required_dims(HFONT font, SIZE& size) const {
            auto hdc = GetDC(hwnd_) ;
            if(font) {
                if(!SelectObject(hdc, font)) {
                    return false ;
                }
            }

            if(!GetTextExtentPoint32W(
                    hdc, label_.c_str(),
                    static_cast<int>(label_.length()), &size)) {
                return false ;
            }

            LONG checkmark_size, label_height, icon_size, margin ;
            if(!calculate_layouts(
                    hdc, checkmark_size, label_height, icon_size, margin)) {
                return false ;
            }

            size.cx += margin + checkmark_size + margin + icon_size + margin ;

            return true ;
        }


    private:
        bool calculate_layouts(
                HDC hdc,
                LONG& checkmark_size,
                LONG& label_height,
                LONG& icon_size,
                LONG& margin) const {
            SIZE size ;
            if(!GetTextExtentPoint32W(hdc, L" ", 1, &size)) {
                return false ;
            }
            checkmark_size = size.cy ;
            margin = size.cy / 2 ;
            label_height = size.cy ;
            icon_size = 4 * label_height / 5 ;
            return true ;
        }

    } ;


    /**
     * @brief Class with information on the entire tray.
     */
    class FluentTray {
    private:
        std::wstring app_name_ ;

        HINSTANCE hinstance_ ;
        HWND hwnd_ ;
        bool visible_ ;
        NOTIFYICONDATAW icon_data_ ;

        TrayStatus status_ ;

        std::vector<FluentMenu> menus_ ;
        std::vector<bool> status_if_focus ;
        std::size_t next_menu_id_ ;
        int select_index_ ;

        POINT previous_mouse_pos_ ;

        LONG menu_x_margin_ ;
        LONG menu_y_margin_ ;

        LONG menu_x_pad_ ;
        LONG menu_y_pad_ ;

        COLORREF text_color_ ;
        COLORREF back_color_ ;
        COLORREF border_color_ ;
        unsigned char color_decay_ ;
        HBRUSH back_brush_ ;
        int autocolorpick_offset_ ;

        LONG menu_font_size_ ;
        HFONT font_ ;

        static unsigned int message_id_ ;

    public:
        /**
         * @brief Create tray object.
         * @param [in] menu_x_margin Horizontal margins outside menus.
         * @param [in] menu_y_margin Vertical margins outside menus.
         * @param [in] menu_x_pad Horizontal paddings inside menus.
         * @param [in] menu_y_pad Vertical paddings inside menus.
         * @param [in] autofadedborder_from_backcolor Decay value from the background color to determine the background color of the currently selected menu and the color of the separator line.
         * @param [in] autocolorpick_offset Pixel offset to determine the background color.
         * @param [in] message_id_offset Unique message identifier.
         */
        explicit FluentTray(
            LONG menu_x_margin=5,
            LONG menu_y_margin=5,
            LONG menu_x_pad=10,
            LONG menu_y_pad=5,
            unsigned char autofadedborder_from_backcolor=10,
            int autocolorpick_offset=5,
            int message_id_offset=25)
        : app_name_(),
          hinstance_(GetModuleHandle(NULL)),
          hwnd_(NULL),
          visible_(false),
          icon_data_(),
          status_(TrayStatus::STOPPED),
          menus_(),
          status_if_focus(),
          next_menu_id_(1),
          select_index_(-1),
          previous_mouse_pos_(),
          menu_x_margin_(menu_x_margin),
          menu_y_margin_(menu_y_margin),
          menu_x_pad_(menu_x_pad),
          menu_y_pad_(menu_y_pad),
          text_color_(CLR_INVALID),
          back_color_(CLR_INVALID),
          border_color_(CLR_INVALID),
          color_decay_(autofadedborder_from_backcolor),
          back_brush_(NULL),
          autocolorpick_offset_(autocolorpick_offset),
          menu_font_size_(0),
          font_(NULL)
        {
            message_id_ = WM_APP + message_id_offset ;
        }

        // Copy
        FluentTray(const FluentTray&) = delete ;
        FluentTray& operator=(const FluentTray&) = delete ;

        // Move
        FluentTray(FluentTray&&) = default ;
        FluentTray& operator=(FluentTray&&) = default ;

        virtual ~FluentTray() noexcept {
            if(font_ != NULL) {
                DeleteObject(font_) ;
            }
            if(back_brush_ != NULL) {
                DeleteObject(back_brush_) ;
            }
        }

        /**
         * @brief Initialize tray and create icon on tray.
         * @param [in] app_name The application name to be displayed as tooltip text.
         * @param [in] icon_path A UTF-8 encoded path to the icon to be displayed in the system tray.
         * @param [in] opacity Menu opacity from 0 to 255.
         * @param [in] round_corner Option to round the corners of the menu window (Windows 11 only)
         * @return Returns true on success, false on failure.
         */
        bool create_tray(
                const std::string& app_name,
                const std::string& icon_path="",
            unsigned char opacity=255,
                bool round_corner=true) {
            if(!util::string2wstring(app_name, app_name_)) {
                return false ;
            }

            WNDCLASSW winc ;
            winc.style = CS_HREDRAW | CS_VREDRAW ;
            winc.lpfnWndProc = &FluentTray::callback ;
            winc.cbClsExtra = 0 ;
            winc.cbWndExtra = sizeof(LONG) * 2 ;  // To store two-part address.
            winc.hInstance = hinstance_ ;
            winc.hIcon = LoadIcon(NULL, IDI_APPLICATION) ;
            winc.hCursor = LoadCursor(NULL, IDC_ARROW) ;
            winc.hbrBackground = GetSysColorBrush(COLOR_WINDOW) ;
            winc.lpszMenuName = NULL ;
            winc.lpszClassName = app_name_.c_str() ;

            if(!RegisterClassW(&winc)) {
                return false ;
            }

            hwnd_ = CreateWindowExW(
                WS_EX_TOOLWINDOW | WS_EX_LAYERED,
                app_name_.c_str(),
                app_name_.c_str(),
                WS_POPUPWINDOW,
                0, 0, 100, 100,
                NULL, NULL,
                hinstance_, NULL
            ) ;
            if(!hwnd_) {
                return false ;
            }

            // To access the this pointer inside the callback function,
            // the address divide into the two part address.
            LONG upper_addr, lower_addr ;
            util::split_bits(this, upper_addr, lower_addr) ;

            SetLastError(0) ;
            if(!SetWindowLongW(hwnd_, 0, upper_addr) && GetLastError() != 0) {
                return false ;
            }
            SetLastError(0) ;
            if(!SetWindowLongW(hwnd_, sizeof(LONG), lower_addr) && GetLastError() != 0) {
                return false ;
            }

            if(!SetLayeredWindowAttributes(hwnd_, 0, opacity, LWA_ALPHA)) {
                return false ;
            }

            // Set rounded window for Windows 11 only.
            if(round_corner) {
#if defined(_MSC_VER) && _MSC_VER >= 1500
               using RtlGetVersionType = NTSTATUS (WINAPI*)(PRTL_OSVERSIONINFOW) ;
                const auto hmodule = LoadLibraryW(L"ntdll.dll") ;
                if(!hmodule) {
                    return false ;
                }

                // cast to void* once to avoid warnings about type conversion.
                const auto RtlGetVersion = reinterpret_cast<RtlGetVersionType>(
                        reinterpret_cast<void*>(GetProcAddress(hmodule, "RtlGetVersion"))) ;
                if(!RtlGetVersion) {
                    FreeLibrary(hmodule) ;
                    return false ;
                }

                OSVERSIONINFOW vinfo ;
                vinfo.dwOSVersionInfoSize = sizeof(decltype(vinfo)) ;
                auto result = RtlGetVersion(&vinfo) ;
                if(!FreeLibrary(hmodule)) {
                    return false ;
                }

                if(result != STATUS_SUCCESS) {
                    return false ;
                }

                // Check if the OS is Windows11
                if(vinfo.dwBuildNumber >= 22000) {
                    auto pref = DWMWCP_ROUND ;
                    if(DwmSetWindowAttribute(
                            hwnd_,
                            DWMWA_WINDOW_CORNER_PREFERENCE,
                            &pref, sizeof(pref)) != S_OK) {
                        return false ;
                    }
                }
#endif // defined(_MSC_VER) && _MSC_VER >= 1500
            }

            if(!change_icon(icon_path)) {
                return false ;
            }

            if(!set_font()) {
                return false ;
            }

            status_ = TrayStatus::RUNNING ;

            return true ;
        }

        /**
         * @brief Add a menu in order from the top.
         * @param [in] label_text The UTF-8 encoded string of the button label.
         * @param [in] icon_path An icon path to show next to the label.
         * @param [in] toggleable Create a switchable menu
         * @param [in] checkmark A checkmark string.
         * @param [in] callback Function called when a click on the menu or a check is enabled.
         * @param [in] unchecked_callback Function called when a check is disabled.
         * @return Returns true on success, false on failure.
         */
        bool add_menu(
                const std::string& label_text="",
                const std::string& icon_path="",
                bool toggleable=false,
                const std::string& checkmark="✓",
                const std::function<bool(void)>& callback=[] {return true ;},
                const std::function<bool(void)>& unchecked_callback=[] {return true ;}) {
            FluentMenu menu(toggleable, callback, unchecked_callback) ;
            if(!menu.create_menu(
                    hinstance_, hwnd_, next_menu_id_,
                    label_text, icon_path, checkmark)) {
                return false ;
            }

            menus_.push_back(std::move(menu)) ;
            status_if_focus.push_back(false) ;
            next_menu_id_ ++ ;
            return true ;
        }

        /**
         * @brief Add a separator line under the last menu item added.
         */
        void add_separator() {
            if(!menus_.empty()) {
                menus_.back().show_separator_line() ;
            }
        }

        /**
         * @brief Get window message and update tray.
         * @return Returns true on success, false on failure.
         */
        bool update() {
            if(status_ == TrayStatus::FAILED) {
                return false ;
            }

            MSG msg ;
            get_message(msg) ;

            if(GetForegroundWindow() != hwnd_ && visible_) {
                if(!hide_menu_window()) {
                    fail() ;
                    return false ;
                }
            }

            POINT pos ;
            if(!GetCursorPos(&pos)) {
                return false ;
            }

            if(pos.x != previous_mouse_pos_.x || pos.y != previous_mouse_pos_.y) {
                // The mouse cursor is moved, so switch to the mouse-mode.
                for(int i = 0 ; i < static_cast<int>(menus_.size()) ; i ++) {
                    auto& menu = menus_[i] ;
                    auto detected_hwnd = WindowFromPoint(pos) ;
                    if(!detected_hwnd) {
                        return false ;
                    }
                    // Checks whether the mouse cursor is over the menu or not.
                    if(detected_hwnd == menu.window_handle()) {
                        // Start selection by key from the currently selected menu.
                        select_index_ = i ;
                        break ;
                    }
                }
                previous_mouse_pos_ = pos ;
            }

            if(select_index_ < 0) {
                return true ;
            }

            // Update the color of only changed menu.
            for(int i = 0 ; i < static_cast<int>(menus_.size()) ; i ++) {
                if(i == select_index_) {
                    if(!status_if_focus[i]) {
                        // OFF -> ON
                        if(!change_menu_back_color(menus_[i], border_color_)) {
                            fail() ;
                            return false ;
                        }
                    }
                    status_if_focus[i] = true ;
                }
                else {
                    if(status_if_focus[i]) {
                        // ON -> OFF
                        if(!change_menu_back_color(menus_[i], back_color_)) {
                            fail() ;
                            return false ;
                        }
                    }
                    status_if_focus[i] = false ;
                }
            }

            return true ;
        }

        /**
         * @brief Create a message loop to update the tray.
         * @param [in] sleep_time
         * @return Returns true on success, false on failure.
         */
        bool update_with_loop(
            std::chrono::milliseconds sleep_time=std::chrono::milliseconds(1)) {

            while(true) {
                if(status_ == TrayStatus::SHOULD_STOP) {
                    status_ = TrayStatus::STOPPED ;
                    break ;
                }

                if(!update()) {
                    return false ;
                }

                Sleep(static_cast<int>(sleep_time.count())) ;
            }
            return true ;
        }

        /**
         * @brief Refer to the handle of menu window.
         * @return The handle of window.
         */
        HWND window_handle() const noexcept {
            return hwnd_ ;
        }

        /**
         * @brief Show the menu window above the tray icon
         * @return Returns true on success, false on failure.
         */
        bool show_menu_window() {
            // Initialize the color settings.
            if(back_color_ == CLR_INVALID) {
                // If the color is CLR_INVALID, it is determined from the theme.
                back_color_ = extract_taskbar_color() ;
                if(back_color_ == CLR_INVALID) {
                    return false ;
                }
                if(!update_background_brush()) {
                    return false ;
                }
            }
            if(text_color_ == CLR_INVALID) {
                // The text color is automatically determined from the background color.
                text_color_ = calculate_text_color_(back_color_) ;
                if(text_color_ == CLR_INVALID) {
                    return false ;
                }
            }
            if(border_color_ == CLR_INVALID) {
                // The border color is automatically determined from the background color.
                border_color_ = calculate_faded_color_(back_color_, color_decay_) ;
                if(border_color_ == CLR_INVALID) {
                    return false ;
                }
            }

            LONG max_label_size = 0 ;
            for(auto& menu : menus_) {
                SIZE size ;
                if(!menu.calculate_required_dims(font_, size)) {
                    return false ;
                }
                if(max_label_size < size.cx) {
                    max_label_size = size.cx ;
                }
            }

            // Update the sizes
            auto menu_width = max_label_size + 2 * menu_x_pad_ ;
            auto menu_height = menu_font_size_ + 2 * menu_y_pad_ ;
            auto popup_width = 2 * menu_x_margin_ + menu_width ;
            auto popup_height = static_cast<LONG>(
                menus_.size() * (menu_y_margin_ + menu_height) + menu_y_margin_) ;

            POINT cursor_pos ;
            if(!GetCursorPos(&cursor_pos)) {
                return false ;
            }

            RECT work_rect ;
            if(!SystemParametersInfo(
                    SPI_GETWORKAREA, 0, reinterpret_cast<PVOID>(&work_rect), 0)) {
                return false ;
            }

            auto screen_width = GetSystemMetrics(SM_CXSCREEN) ;
            auto screen_height = GetSystemMetrics(SM_CYSCREEN) ;

            auto work_width = work_rect.right - work_rect.left ;
            auto work_height = work_rect.bottom - work_rect.top ;

            auto taskbar_width = screen_width - work_width ;
            auto taskbar_height = screen_height - work_height ;

            auto pos = cursor_pos ;
            if(taskbar_width == 0) {  // horizontal taskbar
                if(cursor_pos.y <= taskbar_height) {
                    //top
                    pos.y = taskbar_height ;
                }
                else {
                    //bottom
                    // add 20% offset
                    pos.y = screen_height - (popup_height + 12 * taskbar_height / 10) ;
                }
                pos.x = cursor_pos.x - popup_width / 2 ;
            }
            else {  // vertical taskbar
                if(pos.x <= taskbar_width) {
                    //left
                    pos.x = taskbar_width ;
                }
                else {
                    //right
                    // add 20% offset
                    pos.x = popup_width + 12 * taskbar_width / 10 ;
                }

                pos.y = cursor_pos.y - popup_height / 2 ;
            }

            if(!SetWindowPos(
                    hwnd_, HWND_TOP,
                    pos.x, pos.y, popup_width, popup_height,
                    SWP_SHOWWINDOW)) {
                return false ;
            }

            for(std::size_t i = 0 ; i < menus_.size() ; i ++) {
                auto& menu = menus_[i] ;
                auto y = \
                     menu_y_margin_
                     + static_cast<LONG>(i) * (menu_height + menu_y_margin_) ;
                if(!SetWindowPos(
                        menu.window_handle(), HWND_TOP,
                        menu_x_margin_, y,
                        menu_width, menu_height,
                        SWP_SHOWWINDOW)) {
                    return false ;
                }

                if(!menu.set_color(text_color_, back_color_, border_color_)) {
                    return false ;
                }
            }
            std::fill(status_if_focus.begin(), status_if_focus.end(), false) ;

            if(!SetForegroundWindow(hwnd_)) {
                return false ;
            }

            visible_ = true ;

            return true ;
        }

        /**
         * @brief Hide the menu window above the tray icon.
         * @return Returns true on success, false on failure.
         */
        bool hide_menu_window() {
            ShowWindow(hwnd_, SW_HIDE) ;
            visible_ = false ;
            select_index_ = -1 ;
            std::fill(status_if_focus.begin(), status_if_focus.end(), false) ;
            return true ;
        }

        /**
         * @brief Shows a balloon tip that is placed in the notification area.
         * @param [in] title The UTF-8 string of title.
         * @param [in] message The UTF-8 string of message.
         * @return Returns true on success, false on failure.
         * @details  Titles longer than 48 characters and messages longer than 200 characters will be trimmed.
         */
        bool show_balloon_tip(const std::string& title, const std::string& message) {
            NOTIFYICONDATAW notify_data = {} ;
            notify_data.cbSize = sizeof(notify_data) ;
            notify_data.hWnd = hwnd_ ;
            notify_data.uFlags = NIF_INFO ;

            std::wstring title_wide ;
            if(!util::string2wstring(title, title_wide)) {
                return false ;
            }

            std::wstring message_wide ;
            if(!util::string2wstring(message, message_wide)) {
                return false ;
            }

            auto title_len = title_wide.length() > 47 ? 47 : title_wide.length() ;
            std::wmemcpy(
                notify_data.szInfoTitle, title_wide.c_str(), title_len) ;
            notify_data.szInfoTitle[title_len] = L'\0' ;

            auto message_len = message_wide.length() > 199 ? 199 : message_wide.length() ;
            std::wmemcpy(
                notify_data.szInfo, message_wide.c_str(), message_len) ;
            notify_data.szInfo[message_len] = L'\0' ;

            if(!Shell_NotifyIconW(NIM_MODIFY, &notify_data)) {
                return false ;
            }
            return true ;
        }

        /**
         * @brief Get the current status of tray.
         * @return The status.
         */
        TrayStatus status() const noexcept {
            return status_ ;
        }

        /**
         * @brief Exit the tray successfully.
         */
        void stop() noexcept {
            status_ = TrayStatus::SHOULD_STOP ;
        }

        /**
         * @brief Returns an iterator to the beginning of menus.
         * @return Iterator to the first element.
         */
        std::vector<FluentMenu>::iterator begin() noexcept {
            return menus_.begin() ;
        }

        /**
         * @brief Returns an iterator to the end of menus.
         * @return Iterator to the last element.
         */
        std::vector<FluentMenu>::iterator end() noexcept {
            return menus_.end() ;
        }

        /**
         * @brief Returns a constant iterator to the beginning of menus.
         * @return Constant iterator to the first element.
         */
        std::vector<FluentMenu>::const_iterator cbegin() const noexcept {
            return menus_.cbegin() ;
        }

        /**
         * @brief Returns a constant iterator to the end of menus.
         * @return Constant iterator to the last element.
         */
        std::vector<FluentMenu>::const_iterator cend() const noexcept {
            return menus_.cend() ;
        }

        /**
         * @brief Returns the reference to the beginning of menus.
         * @return Reference to the first element.
         */
        FluentMenu& front() {
            return menus_.front() ;
        }

        /**
         * @brief Returns the const reference to the beginning of menus.
         * @return Const reference to the first element.
         */
        const FluentMenu& front() const {
            return menus_.front() ;
        }

        /**
         * @brief Returns the reference to the last of menus.
         * @return Reference to the first element.
         */
        FluentMenu& back() {
            return menus_.back() ;
        }

        /**
         * @brief Returns the const reference to the last of menus.
         * @return Const reference to the first element.
         */
        const FluentMenu& back() const {
            return menus_.back() ;
        }

        /**
         * @brief Returns the number of menus.
         * @return The number of menus.
         */
        std::size_t count_menus() const noexcept {
            return menus_.size() ;
        }

        /**
         * @brief Set font information to draw menus.
         * @param [in] font_size The height of fonts.
         * @param [in] font_weight The value from 0 to 1000 to determine font weight.
         * @param [in] font_name The UTF-8 encoded font name for label rendering.
         * @return Returns true on success, false on failure.
         * @details For font_weight, you can use the constant value specified for lfWeght in <a href="https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-logfontw">LOGFONT</a>. The default value used is FW_MEDIUM(500).
         */
        bool set_font(
                LONG font_size=0,
                LONG font_weight=0,
                const std::string& font_name="") {
            NONCLIENTMETRICSW metrics ;
            metrics.cbSize = sizeof(metrics) ;

            if(!SystemParametersInfoW(
                    SPI_GETNONCLIENTMETRICS,
                    metrics.cbSize, &metrics, 0)) {
                return false ;
            }

            auto& logfont = metrics.lfCaptionFont ;
            if(font_size != 0) {
                logfont.lfHeight = font_size ;
            }
            else {
                logfont.lfHeight = 20 ;
            }
            if(font_weight != 0) {
                logfont.lfWeight = font_weight ;
            }
            else {
                logfont.lfWeight = FW_MEDIUM ;
            }

            if(!font_name.empty()) {
                std::wstring font_name_wide ;
                if(!util::string2wstring(font_name, font_name_wide)) {
                    return false ;
                }
                auto& dst = logfont.lfFaceName ;

                if(font_name_wide.size() < LF_FACESIZE) {
                    std::wmemcpy(
                        dst, font_name_wide.c_str(), font_name_wide.length()) ;
                    dst[font_name_wide.size()] = L'\0' ;
                }
                else {
                    std::wmemcpy(
                        dst, font_name_wide.c_str(), LF_FACESIZE - 1) ;
                    dst[LF_FACESIZE - 1] = L'\0' ;
                }
            }

            auto font = CreateFontIndirectW(&logfont) ;
            if(!font) {
                return false ;
            }
            font_ = font ;
            menu_font_size_ = std::abs(logfont.lfHeight) ;

            return true;
        }

        /**
         * @brief Set colors to draw menus.
         * @param [in] text_color The color for label text.
         * @param [in] back_color The color for background.
         * @param [in] border_color The color for border.
         * @return Returns true on success, false on failure.
         */
        bool set_color(
                COLORREF text_color=CLR_INVALID,
                COLORREF back_color=CLR_INVALID,
                COLORREF border_color=CLR_INVALID) {
            if(back_color != CLR_INVALID) {
                back_color_ = back_color ;
                if(!update_background_brush()) {
                    return false ;
                }
            }
            if(text_color != CLR_INVALID) {
                text_color_ = text_color ;
            }
            if(border_color != CLR_INVALID) {
                border_color_ = border_color ;
            }
            return true ;
        }

        /**
         * @brief Load the image file and change the icon.
         * @return Returns true on success, false on failure.
         */
        bool change_icon(const std::string& icon_path) {
            if(icon_data_.cbSize > 0) {
                if(!Shell_NotifyIconW(NIM_DELETE, &icon_data_)) {
                    return false ;
                }
            }

            ZeroMemory(&icon_data_, sizeof(icon_data_)) ;

            if(icon_path.empty()) {
                icon_data_.cbSize = 0 ;
                return true ;
            }

            std::wstring icon_path_wide ;
            if(!util::string2wstring(icon_path, icon_path_wide)) {
                return false ;
            }

            if(!util::exists(icon_path_wide)) {
                return false ;
            }

            icon_data_.cbSize = sizeof(icon_data_) ;
            icon_data_.hWnd = hwnd_ ;
            icon_data_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP ;
            icon_data_.uCallbackMessage = message_id_ ;
            icon_data_.hIcon = static_cast<HICON>(
                LoadImageW(
                    NULL, icon_path_wide.c_str(),
                    IMAGE_ICON, 0, 0, LR_LOADFROMFILE)) ;
            wcscpy_s(icon_data_.szTip, app_name_.c_str()) ;
            icon_data_.dwState = NIS_SHAREDICON ;
            icon_data_.dwStateMask = NIS_SHAREDICON ;

            if(!Shell_NotifyIconW(NIM_ADD, &icon_data_)) {
                return false ;
            }
            hide_menu_window() ;

            return true ;
        }

    private:
        static LRESULT CALLBACK callback(
                HWND hwnd,
                UINT msg,
                WPARAM wparam,
                LPARAM lparam) {
            auto get_instance = [hwnd]() -> FluentTray* {
                auto upper_addr = GetWindowLongW(hwnd, 0) ;
                if(!upper_addr) {
                    return nullptr ;
                }

                auto lower_addr = GetWindowLongW(hwnd, sizeof(LONG)) ;
                if(!lower_addr) {
                    return nullptr ;
                }

                FluentTray* self ;
                util::concatenate_bits(upper_addr, lower_addr, self) ;
                return self ;
            } ;

            if(msg == WM_DESTROY || msg == WM_QUIT || msg == WM_CLOSE) {
                if(auto self = get_instance()) {
                    self->stop() ;
                    return 0 ;
                }
            }
            else if(msg == WM_ACTIVATE && wparam == WA_INACTIVE) {
                if(auto self = get_instance()) {
                    if(!self->hide_menu_window()) {
                        self->fail() ;
                    }
                    return 0 ;
                }
            }
            else if(msg == WM_DRAWITEM) {
                if(auto self = get_instance()) {
                    auto item = reinterpret_cast<LPDRAWITEMSTRUCT>(lparam) ;
                    auto menu_idx = self->get_menu_index_from_window(item->hwndItem) ;
                    if(menu_idx < 0) {
                        return FALSE ;
                    }
                    auto& menu = self->menus_[menu_idx] ;
                    if(!menu.draw_menu(item, self->font_)) {
                        self->fail() ;
                        return FALSE ;
                    }
                    return TRUE ;
                }
            }
            else if(msg == WM_CTLCOLORBTN) {
                if(auto self = get_instance()) {
                    auto menu_idx = self->get_menu_index_from_window(reinterpret_cast<HWND>(lparam)) ;
                    if(menu_idx < 0) {
                        return DefWindowProc(hwnd, msg, wparam, lparam) ;
                    }
                    auto& menu = self->menus_[menu_idx] ;
                    return reinterpret_cast<LRESULT>(menu.background_brush()) ;
                }
            }
            else if(msg == WM_COMMAND) {
                if(auto self = get_instance()) {
                    auto menu_idx = self->get_menu_index_from_id(LOWORD(wparam)) ;
                    if(menu_idx < 0) {
                        return FALSE ;
                    }
                    auto& menu = self->menus_[menu_idx] ;
                    if(!menu.process_click_event()) {
                        self->stop() ;
                        return FALSE ;
                    }
                    if(!self->hide_menu_window()) {
                        return FALSE ;
                    }
                    return TRUE ;
                }
            }
            else if(msg == WM_KEYDOWN) {
                if(auto self = get_instance()) {
                    if(wparam == VK_DOWN) {
                        if(self->select_index_ < 0) {
                            // Initialize the position of bounding box cursor
                            self->select_index_ = 0 ;
                        }
                        else {
                            self->select_index_ = (self->select_index_ + 1) % self->menus_.size() ;
                        }
                        return TRUE;
                    }
                    else if(wparam == VK_UP) {
                        if(self->select_index_ < 0) {
                            // Initialize the position of bounding box cursor
                            self->select_index_ = static_cast<int>(self->menus_.size() - 1) ;
                        }
                        else {
                            auto mod = static_cast<int>(self->menus_.size()) ;
                            self->select_index_ = ((self->select_index_ - 1) % mod + mod) % mod ;  // to be positive
                        }
                        return TRUE;
                    }
                    else if(wparam == VK_ESCAPE) {
                        if(!self->hide_menu_window()) {
                            return FALSE ;
                        }
                        return TRUE;
                    }
                    else if(wparam == VK_SPACE || wparam == VK_RETURN) {
                        if(self->select_index_ >= 0) {
                            auto& menu = self->menus_[self->select_index_] ;
                            if(!menu.process_click_event()) {
                                self->stop() ;
                                return FALSE ;
                            }
                            if(!self->hide_menu_window()) {
                                return FALSE ;
                            }
                        }
                        return TRUE;
                    }
                }
            }
            else if(msg == message_id_) {  //On NotifyIcon
                if(auto self = get_instance()) {
                    if(lparam == WM_LBUTTONUP || lparam == WM_RBUTTONUP) {
                        self->show_menu_window() ;
                        return 0 ;
                    }
                }
            }

            return DefWindowProc(hwnd, msg, wparam, lparam) ;
        }

        int get_menu_index_from_window(HWND hwnd) {
            int i = 0 ;
            for(auto& m : menus_) {
                if(m.window_handle() == hwnd) {
                    return i ;
                }
                i ++ ;
            }
            return -1 ;
        }

        int get_menu_index_from_id(WORD id) {
            int i = 0 ;
            for(auto& m : menus_) {
                if(m.id() == static_cast<std::size_t>(id)) {
                    return i ;
                }
                i ++ ;
            }
            return -1 ;
        }

        void get_message(MSG& message) {
            if(PeekMessage(&message, hwnd_, 0, 0, PM_REMOVE)) {
                DispatchMessage(&message) ;
            }
        }

        void fail() noexcept {
            status_ = TrayStatus::FAILED ;
        }

        bool change_menu_back_color(FluentMenu& menu, COLORREF new_color) {
            if(!menu.set_color(
                    text_color_, new_color, border_color_)) {
                return false ;
            }
            // Redraw
            if(!InvalidateRect(menu.window_handle(), NULL, TRUE)) {
                return false ;
            }
            return true ;
        }

        COLORREF extract_taskbar_color() const {
            // Get Taskbar color
            APPBARDATA abd ;
            abd.cbSize = sizeof(abd) ;
            if(!SHAppBarMessage(ABM_GETTASKBARPOS, &abd)) {
                return CLR_INVALID ;
            }

            COLORREF color = CLR_INVALID ;
            if(auto dc = GetDC(NULL)) {
                // Get Taskbar color
                color = GetPixel(
                    dc,
                    abd.rc.left + autocolorpick_offset_,
                    abd.rc.top + autocolorpick_offset_) ;
                if(color == CLR_INVALID) {
                    // if failed, use COLOR_WINDOW color.
                    color = GetSysColor(COLOR_WINDOW) ;
                }
                if(!ReleaseDC(NULL, dc)) {
                    return CLR_INVALID ;
                }
            }
            return color ;
        }

        static COLORREF calculate_faded_color_(
                COLORREF back_color,
                unsigned char color_decay=10) {
            auto back_gray_color = util::rgb2gray(back_color) ;

            unsigned char ash_value = back_gray_color ;
            if(back_gray_color < 128) {
                ash_value = static_cast<decltype(ash_value)>(
                    (std::min)(ash_value + color_decay, 255)) ;
            }
            else {
                ash_value = static_cast<decltype(ash_value)>(
                    (std::max)(ash_value - color_decay, 0)) ;
            }
            return RGB(ash_value, ash_value, ash_value) ;
        }

        static COLORREF calculate_text_color_(COLORREF back_color) {
            auto back_gray_color = util::rgb2gray(back_color) ;

            auto text_color = GetSysColor(COLOR_WINDOWTEXT) ;
            if(back_gray_color < 128) {
                // if dark background, use light text color.
                text_color = 0x00FFFFFF & ~text_color ;
            }
            return text_color ;
        }

        bool update_background_brush() {
            if(back_brush_) {
                // Release old handle.
                if(!DeleteObject(back_brush_)) {
                    return false ;
                }
            }
            back_brush_ = CreateSolidBrush(back_color_) ;
            if(back_brush_ == NULL) {
                return false ;
            }

            if(!SetClassLongPtr(
                    hwnd_, GCLP_HBRBACKGROUND,
                    reinterpret_cast<LONG_PTR>(back_brush_))) {
                return false ;
            }

            return true ;
        }
    } ;

    unsigned int FluentTray::message_id_ ;
}

#endif
