#ifndef _UTIL_FAKE_STRING_HPP
#define _UTIL_FAKE_STRING_HPP

#include "util/string.hpp"
#include <cstring>

#include <cwchar>
#include <string>

namespace vind {
    namespace util {
        //
        // The output string of std::wstring is the fake converted string.
        // It is the same bytes as before-string.
        //
        std::wstring s_to_ws(const std::string& str) {
            std::wstring fakestr ;
            fakestr.resize(str.size()) ;
            std::memcpy(fakestr.data(), str.data(), sizeof(char) * str.size()) ;
            return fakestr ;
        }
    }
}

// stub function
namespace
{
    std::string from_fake_wstr(const std::wstring& str) {
        std::string truestr{} ;
        auto expected_true_str_length = sizeof(wchar_t) * str.capacity() ;
        truestr.resize(expected_true_str_length) ;
        std::memcpy(truestr.data(), str.data(), expected_true_str_length) ;
        return truestr ;
    }
}
#endif
