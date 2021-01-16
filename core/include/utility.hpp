#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <windows.h>

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <tuple>

namespace Utility
{
    std::vector<std::string> split(std::string str, const std::string deliminator=",") ;

    std::string remove_str(std::string str, const unsigned char target) ;

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

    std::string a2A(std::string s) ;
    std::string A2a(std::string s) ;

    void replace_all(std::string& s, const std::string from, const std::string to) ;

    MSG get_win_message() noexcept ;

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

    using rgb_t = std::tuple<unsigned char, unsigned char, unsigned char> ;

    rgb_t hex2rgb(std::string hex) ;
    COLORREF hex2COLORREF(std::string hex) ;

    rgb_t to_complementary_rgb(unsigned char r, unsigned char g, unsigned char b) noexcept ;
    rgb_t to_complementary_rgb(rgb_t rgb) noexcept ;

    COLORREF to_complementary_COLORREF(rgb_t rgb) noexcept ;
    COLORREF to_complementary_COLORREF(unsigned char r, unsigned char g, unsigned char b) noexcept ;

    inline unsigned char to_gray(unsigned char r, unsigned char g, unsigned char b) noexcept {
        return static_cast<unsigned char>(0.299*r + 0.587*g + 0.114*b) ;
    }

    void refresh_display(HWND hwnd) ;
}

#ifndef UNUSED
#define UNUSED(identifier) /* identifier */
#endif

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
