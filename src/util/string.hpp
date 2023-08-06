#ifndef _STRING_HPP
#define _STRING_HPP

#include <string>
#include <vector>

namespace vind
{
    namespace util {
        std::vector<std::string> split(
                std::string str,
                std::string deliminator=",") ;

        std::string remove_str(std::string str, char target) ;

        std::string replace_all(
                std::string s,
                const std::string& from,
                const std::string& to) ;

        // Extracts a number from a string.
        // Its range is from the appearance of the number to a non-numeric character.
        // Example:
        //      std::string: hBah0784hlk503
        //      -> std::stoi(0784)
        //      uint: 784
        //
        template <typename T>
        T extract_num(const std::string& str) {
            constexpr auto c_nums = "0123456789" ;

            auto bpos = str.find_first_of(c_nums) ;
            if(bpos == std::string::npos) return 0 ;

            auto epos = str.find_first_not_of(c_nums, bpos) ;

            return static_cast<T>(std::stoi(str.substr(bpos, epos))) ;
        }

        inline std::string tail(const std::string& str, std::size_t length) {
            if(length >= str.length()) {
                return str ;
            }
            return str.substr(str.length() - length) ;
        }

        inline char a2A(char c) {
            if(c >= 'a' && c <= 'z') {
                return c - ('a' - 'A') ;
            }
            return c ;
        }

        inline std::string a2A(std::string s) {
            for(char& c : s) {
                c = a2A(c) ;
            }
            return s ;
        }

        inline char A2a(char c) {
            if(c >= 'A' && c <= 'Z') {
                return c + ('a' - 'A') ;
            }
            return c ;
        }

        inline std::string A2a(std::string s) {
            for(char& c : s) {
                c = A2a(c) ;
            }
            return s ;
        }

        std::wstring s_to_ws(const std::string& str) ;
        std::string ws_to_s(const std::wstring& wstr) ;

        std::string trim(const std::string& str) ;
    }
}

#endif
