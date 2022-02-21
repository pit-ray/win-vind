#include "unicode.hpp"

#include "debug.hpp"
#include "def.hpp"

#include <memory>
#include <tuple>
#include <vector>

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
            UErrorCode status = U_ZERO_ERROR ;

            std::unique_ptr<UText, decltype(&utext_close)> utext(
                    utext_openUTF8(NULL, utf8str.c_str(), -1, &status),
                    utext_close) ;
            check_icu_err(status) ;

            std::unique_ptr<UBreakIterator, decltype(&ubrk_close)> itr(
                    ubrk_open(UBRK_CHARACTER, uloc_getDefault(), NULL, 0, &status),
                    ubrk_close) ;
            check_icu_err(status) ;

            ubrk_setUText(itr.get(), utext.get(), &status) ;
            check_icu_err(status) ;

            std::u32string codepoints ;
            codepoints.resize(utf8str.size()) ;

            UChar32 code ;
            std::size_t len = 0 ;

            auto idx = ubrk_first(itr.get()) ;
            while(idx != ubrk_done) {
                code = utext_char32At(utext.get(), idx) ;
                if(code == U_SENTINEL) {
                    break ;
                }

                codepoints[len] = static_cast<char32_t>(code) ;
                len ++ ;

                idx = ubrk_next(itr.get()) ;
            }

            // Resize down to avoid multiple NUL terminated string.
            codepoints.resize(len) ;

            return codepoints ;
        }

        std::u32string break_unicode_codepoint(const std::string& utf8str) {
            std::u32string codepoints ;
            codepoints.resize(utf8str.size()) ;

            UErrorCode status = U_ZERO_ERROR ;

            std::unique_ptr<UText, decltype(&utext_close)> utext(
                    utext_openUTF8(NULL, utf8str.c_str(), -1, &status),
                    utext_close) ;
            check_icu_err(status) ;

            std::size_t len = 0 ;
            for(auto code = utext_next32From(utext.get(), 0) ;
                code != U_SENTINEL ;
                code = utext_next32(utext.get())) {

                codepoints[len] = static_cast<char32_t>(code) ;
                len ++ ;
            }

            // Resize down to avoid multiple NUL terminated string.
            codepoints.resize(len) ;

            return codepoints ;
        }


        inline bool is_white_space(char32_t cp) noexcept {
            return cp == U' ' || \
                   cp == U'\t' || \
                   cp == U'\u0000' || \
                   cp == U'\u00a0' ;
        }

        /*
         * TODO: We need to configure the characters considered as
         *       word according to the `iskeyword` option as in Vim.
         *       Currently, It is a approximation of Vim in Windows
         *       default value of "@,48-57,_,128-167,224-235".
         */
        bool is_word(char32_t cp) {
            // isalpha (only a-z,A-Z)
            if(cp >= U'A' && cp <= U'Z') {
                return true ;
            }
            if(cp >= U'a' && cp <= U'z') {
                return true ;
            }
            if(cp >= U'\u0048' && cp <= U'\u0057') {
                return true ;
            }
            if(cp == U'_') {
                return true ;
            }
            if(cp >= U'\u0128' && cp <= U'\u0167') {
                return true ;
            }
            if(cp >= U'\u0224' && cp <= U'\u0235') {
                return true ;
            }
            return false ;
        }


        CharType classify_codepoint(char32_t cp) {
            /*
             * NOTE: The table refers to the following table in Vim
             *       to have the same behavior as Vim.
             * ref. https://github.com/vim/vim/blob/master/src/mbyte.c#L2782
             */
            static std::vector<std::tuple<char32_t, char32_t, CharType>> cp_table {
                {U'\u037e', U'\u037e', CharType::PUNCTUATION}, // Greek question mark
                {U'\u0387', U'\u0387', CharType::PUNCTUATION}, // Greek ano teleia
                {U'\u055a', U'\u055f', CharType::PUNCTUATION}, // Armenian punctuation
                {U'\u0589', U'\u0589', CharType::PUNCTUATION}, // Armenian full stop
                {U'\u05be', U'\u05be', CharType::PUNCTUATION},
                {U'\u05c0', U'\u05c0', CharType::PUNCTUATION},
                {U'\u05c3', U'\u05c3', CharType::PUNCTUATION},
                {U'\u05f3', U'\u05f4', CharType::PUNCTUATION},
                {U'\u060c', U'\u060c', CharType::PUNCTUATION},
                {U'\u061b', U'\u061b', CharType::PUNCTUATION},
                {U'\u061f', U'\u061f', CharType::PUNCTUATION},
                {U'\u066a', U'\u066d', CharType::PUNCTUATION},
                {U'\u06d4', U'\u06d4', CharType::PUNCTUATION},
                {U'\u0700', U'\u070d', CharType::PUNCTUATION}, // Syriac punctuation
                {U'\u0964', U'\u0965', CharType::PUNCTUATION},
                {U'\u0970', U'\u0970', CharType::PUNCTUATION},
                {U'\u0df4', U'\u0df4', CharType::PUNCTUATION},
                {U'\u0e4f', U'\u0e4f', CharType::PUNCTUATION},
                {U'\u0e5a', U'\u0e5b', CharType::PUNCTUATION},
                {U'\u0f04', U'\u0f12', CharType::PUNCTUATION},
                {U'\u0f3a', U'\u0f3d', CharType::PUNCTUATION},
                {U'\u0f85', U'\u0f85', CharType::PUNCTUATION},
                {U'\u104a', U'\u104f', CharType::PUNCTUATION}, // Myanmar punctuation
                {U'\u10fb', U'\u10fb', CharType::PUNCTUATION}, // Georgian punctuation
                {U'\u1361', U'\u1368', CharType::PUNCTUATION}, // Ethiopic punctuation
                {U'\u166d', U'\u166e', CharType::PUNCTUATION}, // Canadian Syl. punctuation
                {U'\u1680', U'\u1680', CharType::WHITE_SPACE},
                {U'\u169b', U'\u169c', CharType::PUNCTUATION},
                {U'\u16eb', U'\u16ed', CharType::PUNCTUATION},
                {U'\u1735', U'\u1736', CharType::PUNCTUATION},
                {U'\u17d4', U'\u17dc', CharType::PUNCTUATION}, // Khmer punctuation
                {U'\u1800', U'\u180a', CharType::PUNCTUATION}, // Mongolian punctuation
                {U'\u2000', U'\u200b', CharType::WHITE_SPACE}, // spaces
                {U'\u200c', U'\u2027', CharType::PUNCTUATION}, // punctuation and symbols
                {U'\u2028', U'\u2029', CharType::WHITE_SPACE},
                {U'\u202a', U'\u202e', CharType::PUNCTUATION}, // punctuation and symbols
                {U'\u202f', U'\u202f', CharType::WHITE_SPACE},
                {U'\u2030', U'\u205e', CharType::PUNCTUATION}, // punctuation and symbols
                {U'\u203c', U'\u203c', CharType::EMOJI},
                {U'\u2049', U'\u2049', CharType::EMOJI},
                {U'\u205f', U'\u205f', CharType::WHITE_SPACE},
                {U'\u2060', U'\u27ff', CharType::PUNCTUATION}, // punctuation and symbols
                {U'\u2070', U'\u207f', CharType::SUPERSCRIPT}, // superscript
                {U'\u2080', U'\u2094', CharType::SUBSCRIPT},   // subscript
                {U'\u20a0', U'\u27ff', CharType::PUNCTUATION}, // all kinds of symbols
                {U'\u2122', U'\u2122', CharType::EMOJI},
                {U'\u2139', U'\u2139', CharType::EMOJI},
                {U'\u2194', U'\u2199', CharType::EMOJI},
                {U'\u21a9', U'\u21aa', CharType::EMOJI},
                {U'\u231a', U'\u231b', CharType::EMOJI},
                {U'\u2328', U'\u2328', CharType::EMOJI},
                {U'\u23cf', U'\u23cf', CharType::EMOJI},
                {U'\u23e9', U'\u23f3', CharType::EMOJI},
                {U'\u23f8', U'\u23fa', CharType::EMOJI},
                {U'\u24c2', U'\u24c2', CharType::EMOJI},
                {U'\u25aa', U'\u25ab', CharType::EMOJI},
                {U'\u25b6', U'\u25b6', CharType::EMOJI},
                {U'\u25c0', U'\u25c0', CharType::EMOJI},
                {U'\u25fb', U'\u25fe', CharType::EMOJI},
                {U'\u2600', U'\u2604', CharType::EMOJI},
                {U'\u260e', U'\u260e', CharType::EMOJI},
                {U'\u2611', U'\u2611', CharType::EMOJI},
                {U'\u2614', U'\u2615', CharType::EMOJI},
                {U'\u2618', U'\u2618', CharType::EMOJI},
                {U'\u261d', U'\u261d', CharType::EMOJI},
                {U'\u2620', U'\u2620', CharType::EMOJI},
                {U'\u2622', U'\u2623', CharType::EMOJI},
                {U'\u2626', U'\u2626', CharType::EMOJI},
                {U'\u262a', U'\u262a', CharType::EMOJI},
                {U'\u262e', U'\u262f', CharType::EMOJI},
                {U'\u2638', U'\u263a', CharType::EMOJI},
                {U'\u2640', U'\u2640', CharType::EMOJI},
                {U'\u2642', U'\u2642', CharType::EMOJI},
                {U'\u2648', U'\u2653', CharType::EMOJI},
                {U'\u265f', U'\u2660', CharType::EMOJI},
                {U'\u2663', U'\u2663', CharType::EMOJI},
                {U'\u2665', U'\u2666', CharType::EMOJI},
                {U'\u2668', U'\u2668', CharType::EMOJI},
                {U'\u267b', U'\u267b', CharType::EMOJI},
                {U'\u267e', U'\u267f', CharType::EMOJI},
                {U'\u2692', U'\u2697', CharType::EMOJI},
                {U'\u2699', U'\u2699', CharType::EMOJI},
                {U'\u269b', U'\u269c', CharType::EMOJI},
                {U'\u26a0', U'\u26a1', CharType::EMOJI},
                {U'\u26a7', U'\u26a7', CharType::EMOJI},
                {U'\u26aa', U'\u26ab', CharType::EMOJI},
                {U'\u26b0', U'\u26b1', CharType::EMOJI},
                {U'\u26bd', U'\u26be', CharType::EMOJI},
                {U'\u26c4', U'\u26c5', CharType::EMOJI},
                {U'\u26c8', U'\u26c8', CharType::EMOJI},
                {U'\u26ce', U'\u26cf', CharType::EMOJI},
                {U'\u26d1', U'\u26d1', CharType::EMOJI},
                {U'\u26d3', U'\u26d4', CharType::EMOJI},
                {U'\u26e9', U'\u26ea', CharType::EMOJI},
                {U'\u26f0', U'\u26f5', CharType::EMOJI},
                {U'\u26f7', U'\u26fa', CharType::EMOJI},
                {U'\u26fd', U'\u26fd', CharType::EMOJI},
                {U'\u2702', U'\u2702', CharType::EMOJI},
                {U'\u2705', U'\u2705', CharType::EMOJI},
                {U'\u2708', U'\u270d', CharType::EMOJI},
                {U'\u270f', U'\u270f', CharType::EMOJI},
                {U'\u2712', U'\u2712', CharType::EMOJI},
                {U'\u2714', U'\u2714', CharType::EMOJI},
                {U'\u2716', U'\u2716', CharType::EMOJI},
                {U'\u271d', U'\u271d', CharType::EMOJI},
                {U'\u2721', U'\u2721', CharType::EMOJI},
                {U'\u2728', U'\u2728', CharType::EMOJI},
                {U'\u2733', U'\u2734', CharType::EMOJI},
                {U'\u2744', U'\u2744', CharType::EMOJI},
                {U'\u2747', U'\u2747', CharType::EMOJI},
                {U'\u274c', U'\u274c', CharType::EMOJI},
                {U'\u274e', U'\u274e', CharType::EMOJI},
                {U'\u2753', U'\u2755', CharType::EMOJI},
                {U'\u2757', U'\u2757', CharType::EMOJI},
                {U'\u2763', U'\u2764', CharType::EMOJI},
                {U'\u2795', U'\u2797', CharType::EMOJI},
                {U'\u27a1', U'\u27a1', CharType::EMOJI},
                {U'\u27b0', U'\u27b0', CharType::EMOJI},
                {U'\u27bf', U'\u27bf', CharType::EMOJI},
                {U'\u2800', U'\u28ff', CharType::BRAILLE},     // braille
                {U'\u2900', U'\u2998', CharType::PUNCTUATION}, // arrows, brackets, etc.
                {U'\u2934', U'\u2935', CharType::EMOJI},
                {U'\u29d8', U'\u29db', CharType::PUNCTUATION},
                {U'\u29fc', U'\u29fd', CharType::PUNCTUATION},
                {U'\u2b05', U'\u2b07', CharType::EMOJI},
                {U'\u2b1b', U'\u2b1c', CharType::EMOJI},
                {U'\u2b50', U'\u2b50', CharType::EMOJI},
                {U'\u2b55', U'\u2b55', CharType::EMOJI},
                {U'\u2e00', U'\u2e7f', CharType::PUNCTUATION}, // supplemental punctuation
                {U'\u3000', U'\u3000', CharType::WHITE_SPACE}, // ideographic space
                {U'\u3001', U'\u3020', CharType::PUNCTUATION}, // ideographic punctuation
                {U'\u3030', U'\u3030', CharType::EMOJI},
                {U'\u3030', U'\u3030', CharType::PUNCTUATION},
                {U'\u303d', U'\u303d', CharType::EMOJI},
                {U'\u303d', U'\u303d', CharType::PUNCTUATION},
                {U'\u3040', U'\u309f', CharType::HIRAGANA}, // Hiragana
                {U'\u30a0', U'\u30ff', CharType::KATAKANA}, // Katakana
                {U'\u3297', U'\u3297', CharType::EMOJI},
                {U'\u3299', U'\u3299', CharType::EMOJI},
                {U'\u3300', U'\u9fff', CharType::CJK_IDEOGRAPHS},   // CJK Ideographs
                {U'\uac00', U'\ud7a3', CharType::HANGUL_SYLLABLES}, // Hangul Syllables
                {U'\uf900', U'\ufaff', CharType::CJK_IDEOGRAPHS},   // CJK Ideographs
                {U'\ufd3e', U'\ufd3f', CharType::PUNCTUATION},
                {U'\ufe30', U'\ufe6b', CharType::PUNCTUATION}, // punctuation forms
                {U'\uff00', U'\uff0f', CharType::PUNCTUATION}, // half/fullwidth ASCII
                {U'\uff1a', U'\uff20', CharType::PUNCTUATION}, // half/fullwidth ASCII
                {U'\uff3b', U'\uff40', CharType::PUNCTUATION}, // half/fullwidth ASCII
                {U'\uff5b', U'\uff65', CharType::PUNCTUATION}, // half/fullwidth ASCII
                {U'\U0001d000', U'\U0001d24f', CharType::PUNCTUATION}, // Musical notation
                {U'\U0001d400', U'\U0001d7ff', CharType::PUNCTUATION}, // Mathematical Alphanumeric Symbols
                {U'\U0001f000', U'\U0001f2ff', CharType::PUNCTUATION}, // Game pieces; enclosed characters
                {U'\U0001f004', U'\U0001f004', CharType::EMOJI},
                {U'\U0001f0cf', U'\U0001f0cf', CharType::EMOJI},
                {U'\U0001f170', U'\U0001f171', CharType::EMOJI},
                {U'\U0001f17e', U'\U0001f17f', CharType::EMOJI},
                {U'\U0001f18e', U'\U0001f18e', CharType::EMOJI},
                {U'\U0001f191', U'\U0001f19a', CharType::EMOJI},
                {U'\U0001f1e6', U'\U0001f1ff', CharType::EMOJI},
                {U'\U0001f201', U'\U0001f202', CharType::EMOJI},
                {U'\U0001f21a', U'\U0001f21a', CharType::EMOJI},
                {U'\U0001f22f', U'\U0001f22f', CharType::EMOJI},
                {U'\U0001f232', U'\U0001f23a', CharType::EMOJI},
                {U'\U0001f250', U'\U0001f251', CharType::EMOJI},
                {U'\U0001f300', U'\U0001f321', CharType::EMOJI},
                {U'\U0001f300', U'\U0001f9ff', CharType::PUNCTUATION}, // Many symbol blocks
                {U'\U0001f324', U'\U0001f393', CharType::EMOJI},
                {U'\U0001f396', U'\U0001f397', CharType::EMOJI},
                {U'\U0001f399', U'\U0001f39b', CharType::EMOJI},
                {U'\U0001f39e', U'\U0001f3f0', CharType::EMOJI},
                {U'\U0001f3f3', U'\U0001f3f5', CharType::EMOJI},
                {U'\U0001f3f7', U'\U0001f4fd', CharType::EMOJI},
                {U'\U0001f4ff', U'\U0001f53d', CharType::EMOJI},
                {U'\U0001f549', U'\U0001f54e', CharType::EMOJI},
                {U'\U0001f550', U'\U0001f567', CharType::EMOJI},
                {U'\U0001f56f', U'\U0001f570', CharType::EMOJI},
                {U'\U0001f573', U'\U0001f57a', CharType::EMOJI},
                {U'\U0001f587', U'\U0001f587', CharType::EMOJI},
                {U'\U0001f58a', U'\U0001f58d', CharType::EMOJI},
                {U'\U0001f590', U'\U0001f590', CharType::EMOJI},
                {U'\U0001f595', U'\U0001f596', CharType::EMOJI},
                {U'\U0001f5a4', U'\U0001f5a5', CharType::EMOJI},
                {U'\U0001f5a8', U'\U0001f5a8', CharType::EMOJI},
                {U'\U0001f5b1', U'\U0001f5b2', CharType::EMOJI},
                {U'\U0001f5bc', U'\U0001f5bc', CharType::EMOJI},
                {U'\U0001f5c2', U'\U0001f5c4', CharType::EMOJI},
                {U'\U0001f5d1', U'\U0001f5d3', CharType::EMOJI},
                {U'\U0001f5dc', U'\U0001f5de', CharType::EMOJI},
                {U'\U0001f5e1', U'\U0001f5e1', CharType::EMOJI},
                {U'\U0001f5e3', U'\U0001f5e3', CharType::EMOJI},
                {U'\U0001f5e8', U'\U0001f5e8', CharType::EMOJI},
                {U'\U0001f5ef', U'\U0001f5ef', CharType::EMOJI},
                {U'\U0001f5f3', U'\U0001f5f3', CharType::EMOJI},
                {U'\U0001f5fa', U'\U0001f64f', CharType::EMOJI},
                {U'\U0001f680', U'\U0001f6c5', CharType::EMOJI},
                {U'\U0001f6cb', U'\U0001f6d2', CharType::EMOJI},
                {U'\U0001f6d5', U'\U0001f6d7', CharType::EMOJI},
                {U'\U0001f6e0', U'\U0001f6e5', CharType::EMOJI},
                {U'\U0001f6e9', U'\U0001f6e9', CharType::EMOJI},
                {U'\U0001f6eb', U'\U0001f6ec', CharType::EMOJI},
                {U'\U0001f6f0', U'\U0001f6f0', CharType::EMOJI},
                {U'\U0001f6f3', U'\U0001f6fc', CharType::EMOJI},
                {U'\U0001f7e0', U'\U0001f7eb', CharType::EMOJI},
                {U'\U0001f90c', U'\U0001f93a', CharType::EMOJI},
                {U'\U0001f93c', U'\U0001f945', CharType::EMOJI},
                {U'\U0001f947', U'\U0001f978', CharType::EMOJI},
                {U'\U0001f97a', U'\U0001f9cb', CharType::EMOJI},
                {U'\U0001f9cd', U'\U0001f9ff', CharType::EMOJI},
                {U'\U0001fa70', U'\U0001fa74', CharType::EMOJI},
                {U'\U0001fa78', U'\U0001fa7a', CharType::EMOJI},
                {U'\U0001fa80', U'\U0001fa86', CharType::EMOJI},
                {U'\U0001fa90', U'\U0001faa8', CharType::EMOJI},
                {U'\U0001fab0', U'\U0001fab6', CharType::EMOJI},
                {U'\U0001fac0', U'\U0001fac2', CharType::EMOJI},
                {U'\U0001fad0', U'\U0001fad6', CharType::EMOJI},
                {U'\U00020000', U'\U0002a6df', CharType::CJK_IDEOGRAPHS}, // CJK Ideographs
                {U'\U0002a700', U'\U0002b73f', CharType::CJK_IDEOGRAPHS}, // CJK Ideographs
                {U'\U0002b740', U'\U0002b81f', CharType::CJK_IDEOGRAPHS}, // CJK Ideographs
                {U'\U0002f800', U'\U0002fa1f', CharType::CJK_IDEOGRAPHS}, // CJK Ideographs
            } ;

            if(cp < U'\u0100') {
                if(is_white_space(cp)) {
                    return CharType::WHITE_SPACE ;
                }
                if(is_word(cp)) {
                    return CharType::OTHERWISE ;
                }
                return CharType::PUNCTUATION ;
            }

            // binary search in table
            int first = 0 ;
            int last = static_cast<int>(cp_table.size()) - 1 ;
            int mid ;
            while(first <= last) {
                // NOTE: (first + last) / 2 may overflow.
                mid = first + (last - first) / 2 ;

                if(cp < std::get<0>(cp_table[mid])) {
                    last = mid - 1 ;
                }
                else if(std::get<1>(cp_table[mid]) < cp) {
                    first = mid + 1 ;
                }
                else {
                    return std::get<2>(cp_table[mid]) ;
                }
            }

            return CharType::OTHERWISE ;
        }
    }


}
