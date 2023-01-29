#include "display_text_painter.hpp"

#include "color.hpp"
#include "debug.hpp"
#include "def.hpp"
#include "point2d.hpp"
#include "rect.hpp"
#include "rect.hpp"
#include "screen_metrics.hpp"
#include "string.hpp"
#include "winwrap.hpp"

#include <cstring>
#include <memory>
#include <string>

namespace vind
{
    namespace util
    {
        struct DisplayTextPainter::Impl {
            util::HDCSPtr hdc ; //device context for actual use

            util::HDCSPtr display_dc ;
            util::HDCSPtr compatible_dc ;

            util::HBitmapUPtr compatible_bitmap ;

            COLORREF fg_color ;
            COLORREF bg_color ;

            LOGFONTW logfont ; //infomation struct for creation of font
            util::HFontUPtr hfont ; //font handle

            LONG font_size ;

            explicit Impl()
            : hdc(nullptr, util::delete_hdc),
              display_dc(nullptr, util::delete_hdc),
              compatible_dc(nullptr, util::delete_hdc),
              compatible_bitmap(nullptr, util::delete_obj),
              fg_color(RGB(0, 0, 0)),
              bg_color(RGB(0, 0, 0)),
              logfont(),
              hfont(nullptr, util::delete_obj),
              font_size(1)
            {
                logfont.lfItalic         = FALSE ;
                logfont.lfUnderline      = FALSE ;
                logfont.lfStrikeOut      = FALSE ;
                logfont.lfCharSet        = ANSI_CHARSET ;
                logfont.lfOutPrecision   = OUT_TT_ONLY_PRECIS ;
                logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
                logfont.lfQuality        = ANTIALIASED_QUALITY ;
                logfont.lfFaceName[0]    = L'\0' ;
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
            pimpl->display_dc = util::create_display_dc() ;

            if(enable_double_buffering) {
                auto box = util::get_combined_metrics() ;

                auto width  = box.width() ;
                auto height = box.height() ;

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

                util::select_obj(pimpl->compatible_dc, pimpl->compatible_bitmap) ;

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
                const std::string& face_name,
                bool enable_double_buffering)
        : pimpl(std::make_unique<Impl>())
        {
            initialize_dc(enable_double_buffering) ;
            set_font(font_size, font_weight, face_name) ;
        }

        void DisplayTextPainter::copy(const DisplayTextPainter& rhs) {
            if(rhs.pimpl == nullptr) return ;

            pimpl->copy_copyable_variables(*(rhs.pimpl)) ;

            initialize_dc(rhs.pimpl->compatible_dc != nullptr) ;

            pimpl->hfont = util::create_font(rhs.pimpl->logfont) ;
            util::select_obj(pimpl->hdc, pimpl->hfont) ;

            util::set_dc_text_color(pimpl->hdc, rhs.pimpl->fg_color) ;
            util::set_dc_back_color(pimpl->hdc, rhs.pimpl->bg_color) ;
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
                const std::string& face_name) {
            // To automatically scale regarding the DPI of
            // the position for drawing, save the base size.
            pimpl->font_size = font_size ;

            pimpl->logfont.lfWeight = font_weight ;

            if(face_name.empty()) {
                pimpl->logfont.lfFaceName[0] = L'\0' ;
            }
            else {
                auto wstr = s_to_ws(face_name) ;
                auto dst = pimpl->logfont.lfFaceName ;

                if(face_name.size() < LF_FACESIZE) {
                    std::memcpy(dst, wstr.c_str(), sizeof(WCHAR) * wstr.length()) ;
                    dst[face_name.size()] = L'\0' ;
                }
                else {
                    std::memcpy(dst, wstr.c_str(), sizeof(WCHAR) * (LF_FACESIZE - 1)) ;
                    dst[LF_FACESIZE - 1] = L'\0' ;
                }
            }
        }

        void DisplayTextPainter::apply_font(float scale) {
            pimpl->logfont.lfHeight = static_cast<LONG>(pimpl->font_size * scale) ;
            pimpl->hfont = util::create_font(pimpl->logfont) ;
            util::select_obj(pimpl->hdc, pimpl->hfont) ;
        }

        //foreground color
        void DisplayTextPainter::set_text_color(COLORREF color) {
            pimpl->fg_color = std::move(color) ;
            util::set_dc_text_color(pimpl->hdc, pimpl->fg_color) ;
        }
        void DisplayTextPainter::set_text_color(
                unsigned char r,
                unsigned char g,
                unsigned char b) {
            set_text_color(RGB(r, g, b)) ;
        }
        void DisplayTextPainter::set_text_color(const std::string& hex) {
            set_text_color(util::hex2COLORREF(hex)) ;
        }

        //background color
        void DisplayTextPainter::set_back_color(COLORREF color) {
            pimpl->bg_color = std::move(color) ;
            util::set_dc_back_color(pimpl->hdc, pimpl->bg_color) ;
        }
        void DisplayTextPainter::set_back_color(
                unsigned char r,
                unsigned char g,
                unsigned char b) {
            set_back_color(RGB(r, g, b)) ;
        }
        void DisplayTextPainter::set_back_color(const std::string& hex) {
            set_back_color(util::hex2COLORREF(hex)) ;
        }

        void DisplayTextPainter::draw(const std::string& str, const Point2D& pos, int extra) {
            if(SetTextCharacterExtra(pimpl->hdc.get(), extra) == static_cast<int>(0x80000000)) {
                throw RUNTIME_EXCEPT("Could not set a character margin.") ;
            }
            auto wstr = s_to_ws(str) ;

            // Change the scale of font regarding the DPI.
            apply_font(get_monitor_scale(pos)) ;

            if(!TextOutW(
                    pimpl->hdc.get(), pos.x(), pos.y(),
                    wstr.c_str(), static_cast<int>(wstr.length()))) {
                throw RUNTIME_EXCEPT("Could not draw a text (" + str + ").") ;
            }
        }


        void DisplayTextPainter::refresh() {
            if(pimpl->compatible_dc) {
                auto box = util::get_combined_metrics() ;
                if(!BitBlt(pimpl->display_dc.get(), 0, 0,
                           box.width(),
                           box.height(),
                           pimpl->compatible_dc.get(), 0, 0, SRCCOPY)) {
                    throw RUNTIME_EXCEPT("Could not copy color data of a compatible device context to a display device context.") ;
                }
            }
            else {
                draw("", Point2D{0, 0}, 0) ;
            }
        }
    }
}
