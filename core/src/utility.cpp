#include "utility.hpp"

#include <chrono>

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

    bool is_existed_dir(const std::string& path)
    {
      auto flag = GetFileAttributesW(s_to_ws(path).c_str());
      return (flag != INVALID_FILE_ATTRIBUTES && (flag & FILE_ATTRIBUTE_DIRECTORY));
    }

    void create_directory(const std::string& path) {
        if(!CreateDirectoryW(Utility::s_to_ws(path).c_str(), NULL)) {
            throw RUNTIME_EXCEPT("Cannot create a directory " + path + ".") ;
        }
    }

    const std::wstring s_to_ws(const std::string& str) {
        std::wstring wstr ;

        if(str.empty()) {
            return wstr ;
        }

        auto str_len = static_cast<int>(strlen(str.c_str())) ;
        auto needed_size = MultiByteToWideChar(
                CP_UTF8, 0,
                str.c_str(), str_len,
                NULL, 0) ;
        if(needed_size <= 0) {
            throw LOGIC_EXCEPT("Could not get the needed size of " + str + ".") ;
        }

        wstr.resize(needed_size) ;
        if(MultiByteToWideChar(
                    CP_UTF8, 0,
                    str.c_str(), str_len,
                    &wstr[0], static_cast<int>(wstr.size())) <= 0) {
            throw LOGIC_EXCEPT("Could not convert the string format from UTF-8 to UTF-16.") ;
        }

        return wstr ;
    }

    const std::string ws_to_s(const std::wstring& wstr) {
        std::string str ;

        if(wstr.empty()) {
            return str ;
        }

        auto wstr_len = static_cast<int>(wcslen(wstr.c_str())) ;
        auto needed_size = WideCharToMultiByte(
                CP_UTF8, 0,
                wstr.c_str(), wstr_len,
                NULL, 0,
                NULL, NULL) ;
        if(needed_size <= 0) {
            throw LOGIC_EXCEPT("Could not get the needed size of std::wstring.") ;
        }

        str.resize(needed_size) ;
        if(WideCharToMultiByte(
                    CP_UTF8, 0,
                    wstr.c_str(), wstr_len,
                    &str[0], static_cast<int>(str.size()),
                    NULL, NULL) <= 0) {
            throw LOGIC_EXCEPT("Could not convert the string format from UTF-16 to UTF-8.") ;
        }

        return str ;
    }

    namespace Debug {
        static std::chrono::system_clock::time_point tp ;
        void bench_start() {
            tp = std::chrono::system_clock::now() ;
        } ;

        int bench_stop() {
            using namespace std::chrono ;
            auto delta = system_clock::now() - tp ;
            auto du = duration_cast<milliseconds>(delta) ;
            return static_cast<int>(du.count()) ;
        } ;
    }
}
