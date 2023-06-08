#ifndef _SCREEN_TEXTRENDER_HPP
#define _SCREEN_TEXTRENDER_HPP

#include "point2d.hpp"

#include <memory>
#include <string>

#include <windows.h>

namespace vind
{
    namespace util
    {
        class ScreenTextRenderer {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            void initialize_dc(bool enable_double_buffering) ;
            void copy(const ScreenTextRenderer& rhs) ;

        public:
            explicit ScreenTextRenderer(
                    LONG font_size,
                    LONG font_weight,
                    const std::string& face_name,
                    bool enable_double_buffering=false) ;
            virtual ~ScreenTextRenderer() noexcept ;

            void set_font(
                    LONG font_size,
                    LONG font_weight,
                    const std::string& face_name) ;

            void apply_font(float scale=1.0f) ;

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
                    const Point2D& pos,
                    int extra=0) ;

            void refresh() ; //call it per a one drawing cycle.

            ScreenTextRenderer(const ScreenTextRenderer&) ;
            ScreenTextRenderer& operator=(const ScreenTextRenderer&) ;

            ScreenTextRenderer(ScreenTextRenderer&&) ;
            ScreenTextRenderer& operator=(ScreenTextRenderer&&) ;
        } ;
    }
}

#endif
