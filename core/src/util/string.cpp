#include "util/string.hpp"

#include <windows.h>

#include <algorithm>

#include "util/def.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{
    namespace util {
        std::vector<std::string> split(
                std::string str,
                std::string deliminator) {
            if(str.empty()) {
                return std::vector<std::string>{} ;
            }

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
                auto head = str.substr(0, pos) ;
                if(!head.empty()) {
                    vec.push_back(head) ;
                }
                str = str.substr(pos + deliminator.size()) ;
            }
            return vec ;
        }

        std::string remove_str(std::string str, char target) {
            //target char is collected at end of sequence by remove.
            //Thus, erase form return iterator of remove to end of it.
            str.erase(std::remove(str.begin(), str.end(), target), str.end()) ;
            return str ;
        }

        std::string replace_all(
                std::string s,
                const std::string& from,
                const std::string& to) {
            size_t spos = 0 ;
            while((spos = s.find(from, spos)) != std::string::npos) {
                s.replace(spos, from.length(), to) ;
                spos += to.length() ;
            }
            return s ;
        }

        std::wstring s_to_ws(const std::string& str) {
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

        std::string ws_to_s(const std::wstring& wstr) {
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

        std::string trim(const std::string& str) {
            auto first = str.find_first_not_of(" ") ;
            if(first == std::string::npos) {
                return "" ;
            }
            auto last = str.find_last_not_of(" ") ;
            return str.substr(first, last - first + 1) ;
        }
    }
}
