#ifndef _DISPLAY_TEXT_PAINTER_HPP
#define _DISPLAY_TEXT_PAINTER_HPP

#include <memory>
#include <string>

#include <windows.h>

class DisplayTextPainter {
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    static HDC create_display_dc() ;

    static void set_dc_text_color(
            std::shared_ptr<HDC__>& hdc,
            const COLORREF& color) ;

    static void set_dc_back_color(
            std::shared_ptr<HDC__>& hdc,
            const COLORREF& color) ;

public:
    explicit DisplayTextPainter(
            LONG font_size,
            LONG font_weight,
            const std::string face_name,
            bool enable_double_buffering=false) ;
    virtual ~DisplayTextPainter() noexcept ;

    void set_font(
            LONG font_size,
            LONG font_weight,
            const std::string face_name) ;

    void set_text_color(COLORREF color) ;
    void set_text_color(
            unsigned char r,
            unsigned char g,
            unsigned char b) ;
    void set_text_color(std::string hex) ;

    void set_back_color(COLORREF color) ;
    void set_back_color(
            unsigned char r,
            unsigned char g,
            unsigned char b) ;
    void set_back_color(std::string hex) ;

    void draw(
            const std::string& str,
            int x,
            int y,
            int extra=0) ;

    void update_with_double_buffering() ;

    DisplayTextPainter(DisplayTextPainter&&) ;
    DisplayTextPainter& operator=(DisplayTextPainter&&) ;

    DisplayTextPainter(const DisplayTextPainter&)            = delete ;
    DisplayTextPainter& operator=(const DisplayTextPainter&) = delete ;
} ;

#endif
