#ifndef _DISPLAY_TEXT_PAINTER_HPP
#define _DISPLAY_TEXT_PAINTER_HPP

#include <memory>
#include <string>

#include <windows.h>

class DisplayTextPainter {
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    explicit DisplayTextPainter(LONG font_size, LONG font_weight, const std::string face_name) ;
    virtual ~DisplayTextPainter() noexcept ;

    void set_font(LONG font_size, LONG font_weight, const std::string face_name) ;

    void set_text_color(COLORREF color) ;
    void set_text_color(unsigned char r, unsigned char g, unsigned char b) ;
    void set_text_color(std::string hex) ;

    void set_back_color(COLORREF color) ;
    void set_back_color(unsigned char r, unsigned char g, unsigned char b) ;
    void set_back_color(std::string hex) ;

    void draw(const std::string& str, int x, int y, int extra=0) ;

    void enable_double_buffering() ;
    void update_display_with_compatibleDC() ;

    DisplayTextPainter(const DisplayTextPainter&)            ;
    DisplayTextPainter& operator=(const DisplayTextPainter&) ;
    DisplayTextPainter(DisplayTextPainter&&) ;
    DisplayTextPainter& operator=(DisplayTextPainter&&) ;
} ;

#endif
