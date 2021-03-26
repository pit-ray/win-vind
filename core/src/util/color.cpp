#include "color.hpp"

namespace vind
{
    namespace util {
        const rgb_t hex2rgb(std::string hex) {
            if(hex.front() == '#') {
                hex.erase(0, 1) ;
            }

            unsigned char r = 0 ;
            unsigned char g = 0 ;
            unsigned char b = 0 ;

            if(hex.length() == 6) {
                auto r_hex = hex.substr(0, 2) ;
                auto g_hex = hex.substr(2, 2) ;
                auto b_hex = hex.substr(4, 2) ;
                r = static_cast<unsigned char>(strtol(r_hex.c_str(), nullptr, 16)) ;
                g = static_cast<unsigned char>(strtol(g_hex.c_str(), nullptr, 16)) ;
                b = static_cast<unsigned char>(strtol(b_hex.c_str(), nullptr, 16)) ;
            }

            return {r, g, b} ;
        }

        const rgb_t to_complementary_rgb(
                unsigned char r,
                unsigned char g,
                unsigned char b) noexcept {
            unsigned char max = 0 ;
            unsigned char min = 255 ;

            if(r >= max) max = r ;
            if(r <= min) min = r ;

            if(g >= max) max = g ;
            if(g <= min) min = g ;

            if(g >= max) max = g ;
            if(g <= min) min = g ;

            auto delta = max + min ;
            return {delta - r, delta - g, delta -b} ;
        }
    }
}
