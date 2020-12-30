#include "utility.hpp"

namespace Utility
{
    std::vector<std::string> split(
            std::string str,
            const std::string deliminator) {
        if(str.length() < deliminator.length()) {
            return std::vector<std::string>{str} ;
        }

        std::vector<std::string> vec ;
        while(true) {
            auto pos = str.find(deliminator) ;
            if(pos == std::string::npos) {
                vec.push_back(str) ;
                return vec ;
            }
            const auto head = str.substr(0, pos) ;
            if(!head.empty()) {
                vec.push_back(head) ;
            }
            str = str.substr(pos + deliminator.size()) ;
        }
        return vec ;
    }

    std::string remove_str(std::string str, const unsigned char target) {
        //target char is collected at end of sequence by remove.
        //Thus, erase form return iterator of remove to end of it.
        str.erase(remove(str.begin(), str.end(), target), str.end()) ;
        return str ;
    }

    std::string a2A(std::string s) {
        std::for_each(s.begin(), s.end(), [](auto& c) {c = (c >= 'a' && c <= 'z') ?  c - ('a' - 'A') : c ;}) ;
        return s ;
    }
    std::string A2a(std::string s) {
        std::for_each(s.begin(), s.end(), [](auto& c) {c = (c >= 'A' && c <= 'Z') ?  c + ('a' - 'A') : c ;}) ;
        return s ;
    }

    void replace_all(std::string& s, const std::string from, const std::string to) {
        size_t spos = 0 ;
        while((spos = s.find(from, spos)) != std::string::npos) {
            s.replace(spos, from.length(), to) ;
            spos += from.length() ;
        }
    }

    MSG get_win_message() noexcept {
        MSG msg ;
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg) ;
            DispatchMessage(&msg) ;
        }
        return msg ;
    }

    rgb_t hex2rgb(std::string hex) {
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


    COLORREF hex2COLORREF(std::string hex) {
        auto [r, g, b] = hex2rgb(hex) ;
        return RGB(r, g, b) ;
    }

    rgb_t to_complementary_rgb(unsigned char r, unsigned char g, unsigned char b) noexcept {
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

     rgb_t to_complementary_rgb(rgb_t rgb) noexcept {
        auto [r, g, b] = rgb ;
        return to_complementary_rgb(r, g, b) ;
    }

    COLORREF to_complementary_COLORREF(rgb_t rgb) noexcept {
        auto [r, g, b] = to_complementary_rgb(rgb) ;
        return RGB(r, g, b) ;
    }

    COLORREF to_complementary_COLORREF(unsigned char r, unsigned char g, unsigned char b) noexcept {
        auto [ro, go, bo] = to_complementary_rgb(r, g, b) ;
        return RGB(ro, go, bo) ;
    }

    void refresh_display(HWND UNUSED(hwnd)) {
        //Some applications flicker by passing a NULL HWND as the first argument.  
        //This nedes improvement.
        if(!InvalidateRect(NULL, NULL, TRUE)) {
            throw RUNTIME_EXCEPT(" failed refresh display") ;
        }
    }
}
