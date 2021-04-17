#ifndef _COLOR_HPP
#define _COLOR_HPP

#include <windows.h>

#include "disable_gcc_warning.hpp"

#include <tuple>

#include "enable_gcc_warning.hpp"

#include <string>

namespace vind
{
    namespace util {
        using rgb_t = std::tuple<unsigned char, unsigned char, unsigned char> ;

        const rgb_t hex2rgb(std::string hex) ;

        inline COLORREF hex2COLORREF(std::string hex) {
            auto [r, g, b] = hex2rgb(hex) ;
            return RGB(r, g, b) ;
        }

        const rgb_t to_complementary_rgb(
                unsigned char r,
                unsigned char g,
                unsigned char b) noexcept ;

         inline const rgb_t to_complementary_rgb(rgb_t rgb) noexcept {
            auto [r, g, b] = rgb ;
            return to_complementary_rgb(r, g, b) ;
        }

        inline COLORREF to_complementary_COLORREF(rgb_t rgb) noexcept {
            const auto [r, g, b] = to_complementary_rgb(rgb) ;
            return RGB(r, g, b) ;
        }

        inline COLORREF to_complementary_COLORREF(
                unsigned char r,
                unsigned char g,
                unsigned char b) noexcept {
            const auto [ro, go, bo] = to_complementary_rgb(r, g, b) ;
            return RGB(ro, go, bo) ;
        }

        inline unsigned char to_gray(
                unsigned char r,
                unsigned char g,
                unsigned char b) noexcept {
            return static_cast<unsigned char>(0.299*r + 0.587*g + 0.114*b) ;
        }
    }
}

#endif
