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
        unsigned int extract_num(const std::string& str) ;

        inline std::string tail(const std::string& str, std::size_t length) {
            if(length >= str.length()) {
                return str ;
            }
            return str.substr(str.length() - length) ;
        }

        inline std::string a2A(std::string s) {
            for(char& c : s) {
                if(c >= 'a' && c <= 'z') c = c - ('a' - 'A') ;
            }
            return s ;
        }

        inline std::string A2a(std::string s) {
            for(char& c : s) {
                if(c >= 'A' && c <= 'Z') c = c + ('a' - 'A') ;
            }
            return s ;
        }

        std::wstring s_to_ws(const std::string& str) ;
        std::string ws_to_s(const std::wstring& wstr) ;

        std::string trim(const std::string& str) ;
    }
}

#endif
