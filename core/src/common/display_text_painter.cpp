#include "display_text_painter.hpp"

#include "utility.hpp"
#include "screen_metrics.hpp"

#include <cstring>

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
    std::unique_ptr<HDC__, decltype(&delete_hdc)> hdc ;
    std::unique_ptr<HFONT__, decltype(&delete_font)> hfont ;

    std::unique_ptr<HDC__, decltype(&delete_hdc)> hvdc;
    std::unique_ptr<HBITMAP__, decltype(&delete_bitmap)> bitmap ;

    COLORREF fg_color ;
    COLORREF bg_color ;

    LOGFONTA logfont ;

    int vdc_width ;
    int vdc_height ;


    explicit Impl()
    : hdc(nullptr, delete_hdc),
      hfont(nullptr, delete_font),
      hvdc(nullptr, delete_hdc),
      bitmap(nullptr, delete_bitmap),
      fg_color(RGB(0, 0, 0)),
      bg_color(RGB(0, 0, 0)),
      logfont(),
      vdc_width(0),
      vdc_height(0)
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

    virtual ~Impl() noexcept = default ;

    void create_display_hdc() {
        auto raw_hdc = CreateDCA("DISPLAY", NULL, NULL, NULL) ;
        if(!raw_hdc) {
            throw RUNTIME_EXCEPT("Could not create HDC of DISPLAY.") ;
        }
        hdc.reset(raw_hdc) ;

    }

    void update_font() {
        auto raw_font = CreateFontIndirectA(&logfont) ;

        if(!raw_font) {
            throw RUNTIME_EXCEPT("Could not create a font.") ;
        }
        hfont.reset(raw_font) ;

        if(!SelectObject(hdc.get(), hfont.get())) {
            throw RUNTIME_EXCEPT("Could not select a font.") ;
        }
    }

    void update_text_color() {
        if(SetTextColor(hdc.get(), fg_color) == CLR_INVALID) {
            throw RUNTIME_EXCEPT("Could not set a text color.") ;
        }
    }
    void update_back_color() {
        if(SetBkColor(hdc.get(), bg_color) == CLR_INVALID) {
            throw RUNTIME_EXCEPT("Could not set a background color.") ;
        }
    }

    void copy(const Impl& rhs) {
        create_display_hdc() ;

        logfont = rhs.logfont ;
        update_font() ;

        fg_color = rhs.fg_color ;
        update_text_color() ;

        bg_color = rhs.bg_color ;
        update_back_color() ;
    }
} ;

DisplayTextPainter::DisplayTextPainter(LONG font_size, LONG font_weight, const std::string face_name)
: pimpl(std::make_unique<Impl>())
{
    pimpl->create_display_hdc() ;
    set_font(font_size, font_weight, std::move(face_name)) ;
}

DisplayTextPainter::~DisplayTextPainter() noexcept = default ;

DisplayTextPainter::DisplayTextPainter(const DisplayTextPainter& rhs)
: pimpl(std::make_unique<Impl>())
{
    if(rhs.pimpl) {
        pimpl->copy(*(rhs.pimpl)) ;
    }
}

DisplayTextPainter& DisplayTextPainter::operator=(const DisplayTextPainter& rhs) {
    if(rhs.pimpl) {
        pimpl->copy(*(rhs.pimpl)) ;
    }

    return *this ;
}

DisplayTextPainter::DisplayTextPainter(DisplayTextPainter&&)            = default ;
DisplayTextPainter& DisplayTextPainter::operator=(DisplayTextPainter&&) = default ;

void DisplayTextPainter::set_font(LONG font_size, LONG font_weight, const std::string face_name) {
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

    pimpl->update_font() ;
}

void DisplayTextPainter::set_text_color(COLORREF color) {
    pimpl->fg_color = std::move(color) ;
    pimpl->update_text_color() ;
}
void DisplayTextPainter::set_text_color(unsigned char r, unsigned char g, unsigned char b) {
    set_text_color(RGB(r, g, b)) ;
}
void DisplayTextPainter::set_text_color(std::string hex) {
    set_text_color(Utility::hex2COLORREF(hex)) ;
}

void DisplayTextPainter::set_back_color(COLORREF color) {
    pimpl->bg_color = std::move(color) ;
    pimpl->update_back_color() ;
}
void DisplayTextPainter::set_back_color(unsigned char r, unsigned char g, unsigned char b) {
    set_back_color(RGB(r, g, b)) ;
}
void DisplayTextPainter::set_back_color(std::string hex) {
    set_back_color(Utility::hex2COLORREF(hex)) ;
}

void DisplayTextPainter::draw(const std::string& str, int x, int y, int extra) {
    auto hdc = pimpl->hvdc ? pimpl->hvdc.get() : pimpl->hdc.get() ;

    if(SetTextCharacterExtra(hdc, extra) == static_cast<int>(0x80000000)) {
        throw RUNTIME_EXCEPT("Could not set a character margin.") ;
    }
    if(!TextOutA(hdc, x, y, str.c_str(), lstrlenA(str.c_str()))) {
        throw RUNTIME_EXCEPT("Could not draw a text (" + str + ").") ;
    }
}


void DisplayTextPainter::enable_double_buffering() {
    RECT conbinded_rect ;
    ScreenMetrics::get_conbined_metrics(&conbinded_rect) ;

    pimpl->vdc_width  = ScreenMetrics::width(conbinded_rect) ;
    pimpl->vdc_height = ScreenMetrics::height(conbinded_rect) ;
    auto raw_bitmap = CreateCompatibleBitmap(
            pimpl->hdc.get(), pimpl->vdc_width, pimpl->vdc_height) ;

    if(!raw_bitmap) {
        throw RUNTIME_EXCEPT("Could not create a compatible bitmap.") ;
    }
    pimpl->bitmap.reset(raw_bitmap) ;

    auto raw_vdc = CreateCompatibleDC(NULL) ;
    if(!raw_vdc) {
        throw RUNTIME_EXCEPT("Could not create a compatible device context.") ;
    }
    pimpl->hvdc.reset(raw_vdc) ;

    if(!SelectObject(pimpl->hvdc.get(), pimpl->bitmap.get())) {
        throw RUNTIME_EXCEPT("Could not assign a bitmap to a device context.") ;
    }
}

void DisplayTextPainter::update_display_with_compatibleDC() {
    if(!BitBlt(pimpl->hdc.get(), 0, 0, pimpl->vdc_width, pimpl->vdc_height,
                pimpl->hvdc.get(), 0, 0, NOTSRCCOPY)) {

        throw RUNTIME_EXCEPT("Could not copy color data of a device context.") ;
    }
}
