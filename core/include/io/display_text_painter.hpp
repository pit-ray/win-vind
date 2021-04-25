#ifndef _DISPLAY_TEXT_PAINTER_HPP
#define _DISPLAY_TEXT_PAINTER_HPP

#include <memory>
#include <string>

#include <windows.h>

namespace vind
{
    class DisplayTextPainter {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        static void delete_hdc(HDC h) noexcept {
            if(h != nullptr) DeleteDC(h) ;
        }
        static void delete_obj(HGDIOBJ f) noexcept {
            if(f != nullptr) DeleteObject(f) ;
        }

        using HDCSPtr        = std::shared_ptr<HDC__> ;
        using HDCUPtr = std::unique_ptr<HDC__, decltype(&delete_hdc)> ;
        using HBitmapUPtr     = std::unique_ptr<HBITMAP__, decltype(&delete_obj)> ;
        using HFontUPtr       = std::unique_ptr<HFONT__, decltype(&delete_obj)> ;

        static HDCUPtr create_display_dc() ;

        static void set_dc_text_color(HDCSPtr& hdc, const COLORREF& color) ;
        static void set_dc_back_color(HDCSPtr& hdc, const COLORREF& color) ;

        void initialize_dc(bool enable_double_buffering) ;
        void copy(const DisplayTextPainter& rhs) ;

        static HFontUPtr create_font(const LOGFONTA& logfont) ;

        static void select_obj(HDCSPtr& hdc, const HBitmapUPtr& bitmap) ;
        static void select_obj(HDCSPtr& hdc, const HFontUPtr& font) ;

    public:
        explicit DisplayTextPainter(
                LONG font_size,
                LONG font_weight,
                const std::string& face_name,
                bool enable_double_buffering=false) ;
        virtual ~DisplayTextPainter() noexcept ;

        void set_font(
                LONG font_size,
                LONG font_weight,
                const std::string& face_name) ;

        void set_text_color(COLORREF color) ;
        void set_text_color(
                unsigned char r,
                unsigned char g,
                unsigned char b) ;
        void set_text_color(const std::string& hex) ;

        void set_back_color(COLORREF color) ;
        void set_back_color(
                unsigned char r,
                unsigned char g,
                unsigned char b) ;
        void set_back_color(const std::string& hex) ;

        void draw(
                const std::string& str,
                int x,
                int y,
                int extra=0) ;

        void refresh() ; //call it per a one drawing cycle.

        DisplayTextPainter(const DisplayTextPainter&) ;
        DisplayTextPainter& operator=(const DisplayTextPainter&) ;

        DisplayTextPainter(DisplayTextPainter&&) ;
        DisplayTextPainter& operator=(DisplayTextPainter&&) ;
    } ;
}

#endif
