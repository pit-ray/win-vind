#ifndef _WINWRAP_HPP
#define _WINWRAP_HPP

#include <windows.h>

#include <memory>
#include <string>

#include "def.hpp"
#include "point_2d.hpp"
#include "string.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{
    namespace util {
        const MSG& get_win_message() noexcept ;

        void refresh_display(HWND hwnd) ;

        inline BOOL b_to_B(bool b) noexcept {
            return b ? TRUE : FALSE ;
        }

        bool is_existed_dir(const std::string& path) ;

        void create_directory(const std::string& path) ;
        void copy_file(
                const std::string& src,
                const std::string& dst,
                bool allow_overwrite=false) ;

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
                const std::string& current_dir,
                std::string cmd,
                const std::string& args="",
                bool show_console_window=true) ;

        int shell_execute(const std::string& url) ;

        std::string get_module_filename(HWND hwnd) ;

        // Wrapper SystemParametersInfo
        template <typename Ptr>
        inline void system_parameters_info(
                UINT param,
                UINT uiparam,
                Ptr pvparam,
                UINT fwinini) {
            if(!SystemParametersInfoW(
                    param,
                    uiparam,
                    reinterpret_cast<PVOID>(pvparam),
                    fwinini)) {
                throw RUNTIME_EXCEPT("Count not set a system parameter with SystemParametersInfo.") ;
            }
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

        HFontUPtr create_font(const LOGFONTA& logfont) ;

        template <typename SmartPtr>
        inline void select_obj(HDCSPtr& hdc, const SmartPtr& obj) {
            if(!SelectObject(hdc.get(), obj.get())) {
                throw RUNTIME_EXCEPT("The device context could not select an object.") ;
            }
        }

        void attach_thread_input(HWND hwnd) ;
        void detach_thread_input(HWND hwnd) ;

        Point2D get_caret_pos(HWND hwnd) ;
    }
}

#endif
