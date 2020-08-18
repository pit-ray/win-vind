#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <windows.h>

namespace Utility
{
    const std::vector<std::string> split(std::string str, const std::string deliminator=",") noexcept ;
    const std::string remove_str(std::string str, const unsigned char target) noexcept ;

    template <typename T>
    inline void remove_deplication(std::vector<T>& vec) {
        if(vec.empty()) return ;
        std::sort(vec.begin(), vec.end()) ;
        const auto& itr = std::unique(vec.begin(), vec.end()) ;
        if(itr == vec.cend()) return ;
        vec.erase(itr) ;
    }
}

#define UNUSED(identifier) /* identifier */

#endif