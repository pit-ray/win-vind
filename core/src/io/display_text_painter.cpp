#include "display_text_painter.hpp"

#include "utility.hpp"
#include "screen_metrics.hpp"

#include <cstring>
#include <memory>

namespace vind
{
    struct DisplayTextPainter::Impl {
        hdc_type hdc ; //device context for actual use

        hdc_type display_dc ;
        hdc_type compatible_dc ;

        bitmap_type compatible_bitmap ;

        COLORREF fg_color ;
        COLORREF bg_color ;

        LOGFONTA logfont ; //infomation struct for creation of font
        font_type hfont ; //font handle

        explicit Impl()
        : hdc(nullptr, delete_hdc),
          display_dc(nullptr, delete_hdc),
          compatible_dc(nullptr, delete_hdc),
          compatible_bitmap(nullptr, delete_obj),
          fg_color(RGB(0, 0, 0)),
          bg_color(RGB(0, 0, 0)),
          logfont(),
          hfont(nullptr, delete_obj)
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
            logfont.lfPitchAndFamily = 0 ;
            logfont.lfFaceName[0]    = '\0' ;
        }
        virtual ~Impl() noexcept = default ;

        void copy_copyable_variables(const Impl& rhs) {
            fg_color = rhs.fg_color ;
            bg_color = rhs.bg_color ;
            logfont  = rhs.logfont ;
        }

        Impl(const Impl&)            = delete ;
        Impl& operator=(const Impl&) = delete ;
        Impl(Impl&&)                 = default ;
        Impl& operator=(Impl&&)      = default ;
    } ;

    void DisplayTextPainter::initialize_dc(bool enable_double_buffering) {
        pimpl->display_dc = create_display_dc() ;

        if(enable_double_buffering) {
            RECT conbinded_rect ;
            ScreenMetrics::get_conbined_metrics(&conbinded_rect) ;
            const auto width  = ScreenMetrics::width(conbinded_rect) ;
            const auto height = ScreenMetrics::height(conbinded_rect) ;

            auto raw_bitmap = CreateCompatibleBitmap(pimpl->display_dc.get(), width, height) ;
            if(!raw_bitmap) {
                throw RUNTIME_EXCEPT("Could not create a compatible bitmap.") ;
            }
            pimpl->compatible_bitmap.reset(raw_bitmap) ;

            auto raw_compatible_dc = CreateCompatibleDC(NULL) ;
            if(!raw_compatible_dc) {
                throw RUNTIME_EXCEPT("Could not create a compatible device context.") ;
            }
            pimpl->compatible_dc.reset(raw_compatible_dc) ;

            select_obj(pimpl->compatible_dc, pimpl->compatible_bitmap) ;

            if(!BitBlt(pimpl->compatible_dc.get(), 0, 0,
                       width, height,
                       pimpl->display_dc.get(), 0, 0, SRCCOPY)) {
                throw RUNTIME_EXCEPT("Could not copy color data of a display device context to a compatible device context.") ;
            }

            pimpl->hdc = pimpl->compatible_dc ;
        }
        else {
            pimpl->hdc = pimpl->display_dc ;
        }
    }

    DisplayTextPainter::DisplayTextPainter(
            LONG font_size,
            LONG font_weight,
            const std::string face_name,
            bool enable_double_buffering)
    : pimpl(std::make_unique<Impl>())
    {
        initialize_dc(enable_double_buffering) ;
        set_font(font_size, font_weight, std::move(face_name)) ;
    }

    void DisplayTextPainter::copy(const DisplayTextPainter& rhs) {
        if(rhs.pimpl == nullptr) return ;

        pimpl->copy_copyable_variables(*(rhs.pimpl)) ;

        initialize_dc(rhs.pimpl->compatible_dc != nullptr) ;

        pimpl->hfont = create_font(rhs.pimpl->logfont) ;
        select_obj(pimpl->hdc, pimpl->hfont) ;

        set_dc_text_color(pimpl->hdc, rhs.pimpl->fg_color) ;
        set_dc_back_color(pimpl->hdc, rhs.pimpl->bg_color) ;
    }

    DisplayTextPainter::~DisplayTextPainter() noexcept                      = default ;
    DisplayTextPainter::DisplayTextPainter(DisplayTextPainter&&)            = default ;
    DisplayTextPainter& DisplayTextPainter::operator=(DisplayTextPainter&&) = default ;

    DisplayTextPainter::DisplayTextPainter(const DisplayTextPainter& rhs)
    : pimpl(std::make_unique<Impl>())
    {
        copy(rhs) ;
    }

    DisplayTextPainter& DisplayTextPainter::operator=(const DisplayTextPainter& rhs) {
        copy(rhs) ;
        return *this ;
    }

    void DisplayTextPainter::set_font(
            LONG font_size,
            LONG font_weight,
            const std::string face_name) {
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

        pimpl->hfont = create_font(pimpl->logfont) ;
        select_obj(pimpl->hdc, pimpl->hfont) ;
    }

    //foreground color
    void DisplayTextPainter::set_text_color(COLORREF color) {
        pimpl->fg_color = std::move(color) ;
        set_dc_text_color(pimpl->hdc, pimpl->fg_color) ;
    }
    void DisplayTextPainter::set_text_color(
            unsigned char r,
            unsigned char g,
            unsigned char b) {
        set_text_color(RGB(r, g, b)) ;
    }
    void DisplayTextPainter::set_text_color(std::string hex) {
        set_text_color(Utility::hex2COLORREF(hex)) ;
    }

    //background color
    void DisplayTextPainter::set_back_color(COLORREF color) {
        pimpl->bg_color = std::move(color) ;
        set_dc_back_color(pimpl->hdc, pimpl->bg_color) ;
    }
    void DisplayTextPainter::set_back_color(
            unsigned char r,
            unsigned char g,
            unsigned char b) {
        set_back_color(RGB(r, g, b)) ;
    }
    void DisplayTextPainter::set_back_color(std::string hex) {
        set_back_color(Utility::hex2COLORREF(hex)) ;
    }

    void DisplayTextPainter::draw(const std::string& str, int x, int y, int extra) {
        if(SetTextCharacterExtra(pimpl->hdc.get(), extra) == static_cast<int>(0x80000000)) {
            throw RUNTIME_EXCEPT("Could not set a character margin.") ;
        }
        if(!TextOutA(pimpl->hdc.get(), x, y, str.c_str(), lstrlenA(str.c_str()))) {
            throw RUNTIME_EXCEPT("Could not draw a text (" + str + ").") ;
        }
    }


    void DisplayTextPainter::refresh() {
        if(pimpl->compatible_dc) {
            RECT conbinded_rect ;
            ScreenMetrics::get_conbined_metrics(&conbinded_rect) ;
            if(!BitBlt(pimpl->display_dc.get(), 0, 0,
                       ScreenMetrics::width(conbinded_rect),
                       ScreenMetrics::height(conbinded_rect),
                       pimpl->compatible_dc.get(), 0, 0, SRCCOPY)) {
                throw RUNTIME_EXCEPT("Could not copy color data of a compatible device context to a display device context.") ;
            }
        }
        else {
            draw("", 0, 0, 0) ;
        }
    }

    //static utility functions
    DisplayTextPainter::hdc_unique_type DisplayTextPainter::create_display_dc() {
        auto raw_hdc = CreateDCA("DISPLAY", NULL, NULL, NULL) ;
        if(!raw_hdc) {
            throw RUNTIME_EXCEPT("Could not create HDC of DISPLAY.") ;
        }
        return hdc_unique_type(raw_hdc, delete_hdc) ;
    }

    void DisplayTextPainter::set_dc_text_color(
            std::shared_ptr<HDC__>& hdc,
            const COLORREF& color) {
        if(SetTextColor(hdc.get(), color) == CLR_INVALID) {
            throw RUNTIME_EXCEPT("Could not set a text color.") ;
        }
    }

    void DisplayTextPainter::set_dc_back_color(
            std::shared_ptr<HDC__>& hdc,
            const COLORREF& color) {
        if(SetBkColor(hdc.get(), color) == CLR_INVALID) {
            throw RUNTIME_EXCEPT("Could not set a background color.") ;
        }
    }

    DisplayTextPainter::font_type DisplayTextPainter::create_font(const LOGFONTA& logfont) {
        auto raw_font = CreateFontIndirectA(&logfont) ;
        if(!raw_font) {
            throw RUNTIME_EXCEPT("Could not create a font.") ;
        }
        return font_type(raw_font, delete_obj) ;
    }

    void DisplayTextPainter::select_obj(hdc_type& hdc, const bitmap_type& bitmap) {
        if(!SelectObject(hdc.get(), bitmap.get())) {
            throw RUNTIME_EXCEPT("The device context could not select a bitmap object.") ;
        }
    }

    void DisplayTextPainter::select_obj(hdc_type& hdc, const font_type& font) {
        if(!SelectObject(hdc.get(), font.get())) {
            throw RUNTIME_EXCEPT("The device context could not select a font object.") ;
        }
    }
}
