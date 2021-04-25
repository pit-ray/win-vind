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

        void replace_all(
                std::string& s,
                const std::string& from,
                const std::string& to) ;

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
    }
}

#endif
