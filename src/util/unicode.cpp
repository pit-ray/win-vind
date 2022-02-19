#include "unicode.hpp"

#include "def.hpp"

#include <memory>

#include "disable_compiler_warning.hpp"

#if defined(_MSC_VER) && _MSC_VER >= 1500
#include <icu.h>
#else
#include <um/icu.h>
#endif

namespace
{
    // UBRK_DONE is a dirty macro that produces a lot
    // of warnings, so store it in a variable once.
    constexpr auto ubrk_done = UBRK_DONE ;
}

#include "enable_compiler_warning.hpp"


namespace
{
    inline void check_icu_err(UErrorCode status) {
        if(U_FAILURE(status)) {
            throw std::runtime_error(u_errorName(status)) ;
        }
    }

}


namespace vind
{
    namespace util
    {
        std::u32string break_unicode_grapheme(const std::string& utf8str) {
            std::u32string codepoints ;
            codepoints.resize(utf8str.size()) ;

            UErrorCode status = U_ZERO_ERROR ;

            std::unique_ptr<UText, decltype(&utext_close)> ut(
                    utext_openUTF8(NULL, utf8str.c_str(), -1, &status),
                    utext_close) ;
            check_icu_err(status) ;

            std::unique_ptr<UBreakIterator, decltype(&ubrk_close)> bi(
                    ubrk_open(UBRK_CHARACTER, uloc_getDefault(), NULL, 0, &status),
                    ubrk_close) ;
            check_icu_err(status) ;

            ubrk_setUText(bi.get(), ut.get(), &status) ;
            check_icu_err(status) ;

            UChar32 code ;
            std::size_t len = 0 ;

            while(ubrk_next(bi.get()) != ubrk_done) {
                code = utext_current32(ut.get()) ;
                if(code == U_SENTINEL) {
                    break ;
                }

                codepoints[len] = static_cast<char32_t>(code) ;
                len ++ ;
            }

            // Resize down to avoid multiple NUL terminated string.
            codepoints.resize(len) ;

            return codepoints ;
        }

        std::u32string break_unicode_codepoint(const std::string& utf8str) {
            std::u32string codepoints ;
            codepoints.resize(utf8str.size()) ;

            UErrorCode status = U_ZERO_ERROR ;

            std::unique_ptr<UText, decltype(&utext_close)> ut(
                    utext_openUTF8(NULL, utf8str.c_str(), -1, &status),
                    utext_close) ;
            check_icu_err(status) ;

            std::size_t len = 0 ;
            for(auto code = utext_next32From(ut.get(), 0) ;
                code != U_SENTINEL ;
                code = utext_next32(ut.get())) {

                codepoints[len] = static_cast<char32_t>(code) ;
                len ++ ;
            }

            // Resize down to avoid multiple NUL terminated string.
            codepoints.resize(len) ;

            return codepoints ;
        }
    }
}
