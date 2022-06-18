#ifndef _WINWRAP_HPP
#define _WINWRAP_HPP

#include <windows.h>

#include <filesystem>
#include <memory>
#include <string>

#include "box2d.hpp"
#include "def.hpp"
#include "point2d.hpp"
#include "string.hpp"


namespace vind
{
    namespace util {
        const MSG& get_win_message() noexcept ;

        void refresh_display(HWND hwnd) ;

        HWND get_foreground_window() ;
        void set_foreground_window(HWND hwnd) ;

        Box2D get_window_rect(HWND hwnd) ;

        inline BOOL b_to_B(bool b) noexcept {
            return b ? TRUE : FALSE ;
        }

        template <typename ...Args>
        std::string concat_args(Args&&... args) {
            std::initializer_list<std::string> arglist = {
                std::forward<Args>(args)...} ;
            std::string out ;
            for(auto& arg : arglist) {
                out += " " + arg ;
            }
            return out ;
        }

        void create_process(
                const std::filesystem::path& current_dir,
                std::string cmd,
                const std::string& args="",
                bool show_console_window=true,
                bool wait_until_finish=false) ;

        int shell_execute(const std::filesystem::path& url) ;

        std::string get_module_filename(HWND hwnd) ;

        // Wrapper SystemParametersInfo
        template <typename Ptr>
        inline void system_parameters_info(
                UINT param,
                UINT uiparam,
                Ptr pvparam,
                UINT fwinini) {
#if defined(_MSC_VER) && _MSC_VER >= 1500
#pragma warning(disable : 4312)
#endif
            if(!SystemParametersInfoW(
                    param,
                    uiparam,
                    reinterpret_cast<PVOID>(pvparam),
                    fwinini)) {
                throw RUNTIME_EXCEPT("Count not set a system parameter with SystemParametersInfo.") ;
            }

#if defined(_MSC_VER) && _MSC_VER >= 1500
#pragma warning(default : 4312)
#endif
        }

        bool is_failed(HRESULT result) noexcept ;


        inline void delete_hdc(HDC h) noexcept {
            if(h != nullptr) {
                DeleteDC(h) ;
            }
        }
        inline void delete_obj(HGDIOBJ f) noexcept {
            if(f != nullptr) {
                DeleteObject(f) ;
            }
        }

        using HDCSPtr     = std::shared_ptr<HDC__> ;
        using HDCUPtr     = std::unique_ptr<HDC__, decltype(&delete_hdc)> ;
        using HBitmapUPtr = std::unique_ptr<HBITMAP__, decltype(&delete_obj)> ;
        using HFontUPtr   = std::unique_ptr<HFONT__, decltype(&delete_obj)> ;

        HDCUPtr create_display_dc() ;

        void set_dc_text_color(HDCSPtr& hdc, const COLORREF& color) ;
        void set_dc_back_color(HDCSPtr& hdc, const COLORREF& color) ;

        HFontUPtr create_font(const LOGFONTW& logfont) ;

        template <typename SmartPtr>
        inline void select_obj(HDCSPtr& hdc, const SmartPtr& obj) {
            if(!SelectObject(hdc.get(), obj.get())) {
                throw RUNTIME_EXCEPT("The device context could not select an object.") ;
            }
        }

        void attach_thread_input(HWND hwnd) ;
        void detach_thread_input(HWND hwnd) ;

        Point2D get_cursor_pos() ;

        void set_cursor_pos(int x, int y) ;

        template <typename T1, typename T2>
        inline void set_cursor_pos(T1 x, T2 y) {
            return set_cursor_pos(static_cast<int>(x), static_cast<int>(y)) ;
        }

        inline auto set_cursor_pos(const Point2D& pos) {
            return set_cursor_pos(pos.x(), pos.y()) ;
        }

        Point2D get_caret_pos(HWND hwnd) ;

        // To get Windows system informations
        std::string get_Windows_edition(
                DWORD major_version,
                DWORD minor_version) ;

        std::string get_Windows_architecture() noexcept ;

        // It returns {Major, Minor, Build}.
        std::tuple<DWORD, DWORD, DWORD> get_Windows_versions() ;

        std::string get_Windows_display_version() ;
    }
}

#endif
