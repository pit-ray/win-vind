#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <windows.h>

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <vector>

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
    inline void remove_deplication(std::vector<T>& vec) {
        if(vec.empty()) return ;
        std::sort(vec.begin(), vec.end()) ;
        const auto& itr = std::unique(vec.begin(), vec.end()) ;
        if(itr == vec.cend()) return ;
        vec.erase(itr) ;
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
            //TranslateMessage(&msg) ;
            DispatchMessage(&msg) ;
        }
    }
}

#define UNUSED(identifier) /* identifier */

//exception class with scope identifier
#define LOGIC_EXCEPT(msg) \
    std::logic_error(std::string("An logic exception occurred from ") +\
            __PRETTY_FUNCTION__ + ". " + msg)

#define RUNTIME_EXCEPT(msg) \
    std::runtime_error(std::string("An runtime exception occurred from ") +\
            __PRETTY_FUNCTION__ + ". " + msg)

#endif
