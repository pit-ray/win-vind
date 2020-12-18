#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <windows.h>

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

namespace Utility
{
    inline auto split(std::string str, const std::string deliminator=",") {
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

    inline auto remove_str(std::string str, const unsigned char target) {
        //target char is collected at end of sequence by remove.
        //Thus, erase form return iterator of remove to end of it.
        str.erase(remove(str.begin(), str.end(), target), str.end()) ;
        return str ;
    }

    template <typename T>
    inline void remove_deplication(T&& vec) {
        if(vec.empty()) return ;
        std::sort(vec.begin(), vec.end()) ;
        decltype(auto) itr = std::unique(vec.begin(), vec.end()) ;
        vec.erase(itr, vec.end()) ;
    }

    template <typename T, std::size_t N>
    inline constexpr std::size_t sizeof_array(T(&) [N]) noexcept {
        return N ;
    }

    inline auto a2A(std::string s) {
        std::for_each(s.begin(), s.end(), [](auto& c) {c = (c >= 'a' && c <= 'z') ?  c - ('a' - 'A') : c ;}) ;
        return s ;
    }
    inline auto A2a(std::string s) {
        std::for_each(s.begin(), s.end(), [](auto& c) {c = (c >= 'A' && c <= 'Z') ?  c + ('a' - 'A') : c ;}) ;
        return s ;
    }

    inline void replace_all(std::string& s, const std::string from, const std::string to) {
        size_t spos = 0 ;
        while((spos = s.find(from, spos)) != std::string::npos) {
            s.replace(spos, from.length(), to) ;
            spos += from.length() ;
        }
    }

    inline void get_win_message() noexcept {
        static MSG msg ;
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg) ;
            DispatchMessage(&msg) ;
        }
    }

    template <typename T>
    void remove_from_top(std::vector<T>& v, const std::size_t num_from_top) {
        if(v.empty()) {
            return ;
        }
        if(v.size() < num_from_top) {
            v.clear() ;
            return ;
        }
        v.erase(v.begin(), v.begin() + num_from_top) ;
    }

    template <typename T>
    void remove_from_back(std::vector<T>& v, std::size_t num_from_back) {
        if(v.empty()) {
            return ;
        }
        if(v.size() < num_from_back) {
            v.clear() ;
            return ;
        }
        v.erase(v.end() - num_from_back, v.end()) ;
    }

    constexpr unsigned int pow_i(int base, unsigned int exp) noexcept {
        return (exp == 0 ? 1 : base * pow_i(base, exp - 1)) ;
    }
    constexpr float pow_f(int base, int exp) noexcept {
        if(exp < 0) return pow_f(base, exp + 1) / base ;
        return (exp == 0 ? 1.0f : base * pow_f(base, exp - 1)) ;
    }
    constexpr double pow_d(int base, int exp) noexcept {
        if(exp < 0) return pow_d(base, exp + 1) / base ;
        return (exp == 0 ? 1.0 : base * pow_d(base, exp - 1)) ;
    }

    inline const auto hex2COLOREF(std::string hex) {
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

        return RGB(r, g, b) ;
    }
}

#define UNUSED(identifier) /* identifier */

//exception class with scope identifier
#if defined(__GNUC__)
#define LOGIC_EXCEPT(msg) \
    std::logic_error(std::string("An logic exception occurred from ") +\
            __PRETTY_FUNCTION__ + ". " + msg)

#define RUNTIME_EXCEPT(msg) \
    std::runtime_error(std::string("An runtime exception occurred from ") +\
            __PRETTY_FUNCTION__ + ". " + msg)

#elif defined(_MSC_VER) && _MSC_VER >= 1500
#define LOGIC_EXCEPT(msg) \
    std::logic_error(std::string("An logic exception occurred from ") +\
            __FUNCSIG__ + ". " + msg)

#define RUNTIME_EXCEPT(msg) \
    std::runtime_error(std::string("An runtime exception occurred from ") +\
            __FUNCSIG__ + ". " + msg)
#endif

#endif
