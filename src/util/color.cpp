#include "util/color.hpp"

namespace vind
{
    namespace util
    {
        RGBTuple hex2rgb(std::string hex) {
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

        RGBTuple to_complementary_rgb(
                unsigned char r,
                unsigned char g,
                unsigned char b) noexcept {

            auto max = r ;
            auto min = r ;

            if(g >= max) max = g ;
            if(g <= min) min = g ;

            if(b >= max) max = b ;
            if(b <= min) min = b ;

            auto delta = max + min ;
            return {delta - r, delta - g, delta -b} ;
        }
    }
}
