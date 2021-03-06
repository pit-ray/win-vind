#include "display_text_painter.hpp"

#include "utility.hpp"
#include "screen_metrics.hpp"

#include <cstring>
#include <memory>

inline static void delete_hdc(HDC h) noexcept {
    if(h != nullptr) DeleteDC(h) ;
}

inline static void delete_font(HFONT f) noexcept {
    if(f != nullptr) DeleteObject(f) ;
}

inline static void delete_bitmap(HBITMAP b) noexcept {
    if(b != nullptr) DeleteObject(b) ;
}

struct DisplayTextPainter::Impl {
    std::shared_ptr<HDC__> hdc ; //device context for actual use

    std::shared_ptr<HDC__> display_dc ;
    std::shared_ptr<HDC__> compatible_dc ;

    std::unique_ptr<HBITMAP__, decltype(&delete_bitmap)> compatible_bitmap ;

    COLORREF fg_color ;
    COLORREF bg_color ;

    LOGFONTA logfont ; //infomation struct for creation of font
    std::unique_ptr<HFONT__, decltype(&delete_font)> hfont ; //font handle

    int display_width ;
    int display_height ;

    explicit Impl()
    : hdc(nullptr, delete_hdc),
      display_dc(nullptr, delete_hdc),
      compatible_dc(nullptr, delete_hdc),
      compatible_bitmap(nullptr, delete_bitmap),
      fg_color(RGB(0, 0, 0)),
      bg_color(RGB(0, 0, 0)),
      logfont(),
      hfont(nullptr, delete_font),
      display_width(0),
      display_height(0)
    {
        logfont.lfHeight         = 0 ;
        logfont.lfWidth          = 0 ;
        logfont.lfEscapement     = 0 ;
        logfont.lfOrientation    = 0 ;
        logfont.lfWeight         = 0 ;
        logfont.lfItalic         = FALSE ;
        logfont.lfUnderline      = FALSE ;
        logfont.lfStrikeOut      = FALSE ;
        logfont.lfCharSet        = ANSI_CHARSET ;
        logfont.lfOutPrecision   = OUT_TT_ONLY_PRECIS ;
        logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
        logfont.lfQuality        = ANTIALIASED_QUALITY ;
        logfont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN ;
        logfont.lfFaceName[0]    = '\0' ;
    }
} ;

DisplayTextPainter::DisplayTextPainter(
        LONG font_size,
        LONG font_weight,
        const std::string face_name,
        bool enable_double_buffering)
: pimpl(std::make_unique<Impl>())
{
    pimpl->display_dc.reset(create_display_dc()) ;

    if(enable_double_buffering) {
        RECT conbinded_rect ;
        ScreenMetrics::get_conbined_metrics(&conbinded_rect) ;

        pimpl->display_width  = ScreenMetrics::width(conbinded_rect) ;
        pimpl->display_height = ScreenMetrics::height(conbinded_rect) ;

        auto raw_bitmap = CreateCompatibleBitmap(
                pimpl->display_dc.get(),
                pimpl->display_width,
                pimpl->display_height) ;

        if(!raw_bitmap) {
            throw RUNTIME_EXCEPT("Could not create a compatible bitmap.") ;
        }
        pimpl->compatible_bitmap.reset(raw_bitmap) ;

        auto raw_compatible_dc = CreateCompatibleDC(NULL) ;
        if(!raw_compatible_dc) {
            throw RUNTIME_EXCEPT("Could not create a compatible device context.") ;
        }
        pimpl->compatible_dc.reset(raw_compatible_dc) ;

        if(SelectObject(
                    pimpl->compatible_dc.get(),
                    pimpl->compatible_bitmap.get())) {
            throw RUNTIME_EXCEPT("Could not assign a bitmap to a device context.") ;
        }

        if(!BitBlt(pimpl->compatible_dc.get(), 0, 0,
                   pimpl->display_width, pimpl->display_height,
                   pimpl->display_dc.get(), 0, 0, SRCCOPY)) {
            throw RUNTIME_EXCEPT("Could not copy color data of a display device context to a compatible device context.") ;
        }

        pimpl->hdc = pimpl->compatible_dc ;
    }
    else {
        pimpl->hdc = pimpl->display_dc ;
    }

    set_font(font_size, font_weight, std::move(face_name)) ;
}

DisplayTextPainter::~DisplayTextPainter() noexcept                      = default ;
DisplayTextPainter::DisplayTextPainter(DisplayTextPainter&&)            = default ;
DisplayTextPainter& DisplayTextPainter::operator=(DisplayTextPainter&&) = default ;

void DisplayTextPainter::set_font(
        LONG font_size,
        LONG font_weight,
        const std::string face_name)
{
    pimpl->logfont.lfHeight = font_size ;
    pimpl->logfont.lfWeight = font_weight ;

    if(face_name.empty()) {
        pimpl->logfont.lfFaceName[0] = '\0' ;
    }
    else {
#if defined(_MSC_VER) && _MSC_VER >= 1500
#pragma warning(disable : 4996)
#endif
        std::strcpy(pimpl->logfont.lfFaceName, face_name.c_str()) ;

#if defined(_MSC_VER) && _MSC_VER >= 1500
#pragma warning(default : 4996)
#endif
    }

    auto raw_font = CreateFontIndirectA(&(pimpl->logfont)) ;

    if(!raw_font) {
        throw RUNTIME_EXCEPT("Could not create a font.") ;
    }
    pimpl->hfont.reset(raw_font) ;

    if(!SelectObject(pimpl->hdc.get(), pimpl->hfont.get())) {
        throw RUNTIME_EXCEPT("Could not select a font.") ;
    }
}

//foreground color
void DisplayTextPainter::set_text_color(COLORREF color)
{
    pimpl->fg_color = std::move(color) ;
    set_dc_text_color(pimpl->hdc, pimpl->fg_color) ;
}
void DisplayTextPainter::set_text_color(
        unsigned char r,
        unsigned char g,
        unsigned char b)
{
    set_text_color(RGB(r, g, b)) ;
}
void DisplayTextPainter::set_text_color(std::string hex)
{
    set_text_color(Utility::hex2COLORREF(hex)) ;
}

//background color
void DisplayTextPainter::set_back_color(COLORREF color)
{
    pimpl->bg_color = std::move(color) ;
    set_dc_back_color(pimpl->hdc, pimpl->bg_color) ;
}
void DisplayTextPainter::set_back_color(
        unsigned char r,
        unsigned char g,
        unsigned char b)
{
    set_back_color(RGB(r, g, b)) ;
}
void DisplayTextPainter::set_back_color(std::string hex)
{
    set_back_color(Utility::hex2COLORREF(hex)) ;
}

void DisplayTextPainter::draw(const std::string& str, int x, int y, int extra)
{
    if(SetTextCharacterExtra(pimpl->hdc.get(), extra) == static_cast<int>(0x80000000)) {
        throw RUNTIME_EXCEPT("Could not set a character margin.") ;
    }
    if(!TextOutA(pimpl->hdc.get(), x, y, str.c_str(), lstrlenA(str.c_str()))) {
        throw RUNTIME_EXCEPT("Could not draw a text (" + str + ").") ;
    }
}


void DisplayTextPainter::update_with_double_buffering()
{
    if(!pimpl->compatible_dc) return ;
    if(!BitBlt(pimpl->display_dc.get(), 0, 0,
               pimpl->display_width, pimpl->display_height,
               pimpl->compatible_dc.get(), 0, 0, SRCCOPY)) {
        throw RUNTIME_EXCEPT("Could not copy color data of a compatible device context to a display device context.") ;
    }
}

//static utility functions
HDC DisplayTextPainter::create_display_dc()
{
    auto raw_hdc = CreateDCA("DISPLAY", NULL, NULL, NULL) ;
    if(!raw_hdc) {
        throw RUNTIME_EXCEPT("Could not create HDC of DISPLAY.") ;
    }
    return raw_hdc ;
}

void DisplayTextPainter::set_dc_text_color(
        std::shared_ptr<HDC__>& hdc,
        const COLORREF& color)
{
    if(SetTextColor(hdc.get(), color) == CLR_INVALID) {
        throw RUNTIME_EXCEPT("Could not set a text color.") ;
    }
}

void DisplayTextPainter::set_dc_back_color(
        std::shared_ptr<HDC__>& hdc,
        const COLORREF& color)
{
    if(SetBkColor(hdc.get(), color) == CLR_INVALID) {
        throw RUNTIME_EXCEPT("Could not set a background color.") ;
    }
}
