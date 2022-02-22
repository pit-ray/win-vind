#ifndef _UNICODE_HPP
#define _UNICODE_HPP

#include <string>


namespace vind
{
    namespace util
    {
        /*
         * It performs grapheme boundary detection on multi-byte strings
         * encoded in UTF-8, and returns a UTF-32 string containing only
         * the first codepoint of each. The length matches the grapheme
         * cluster.
         *
         * @param[in] (utf8str) Multi-byte character strings encoded in UTF-8.
         * @return std::u32string UTF-32 string whose size() matches the grapheme cluster.
         */
        std::u32string break_unicode_grapheme(const std::string& utf8str) ;

        /*
         * It performs general UTF-8 to UTF-32 conversion. Thus, the length
         * of the returned string will match the number of codepoints.
         *
         * @param[in] (utf8str) Multi-byte character strings encoded in UTF-8.
         * @return std::u32string
         */
        std::u32string break_unicode_codepoint(const std::string& utf8str) ;


        enum class CharType : unsigned char {
            UNDEFINED,

            WHITE_SPACE,
            PUNCTUATION,

            EMOJI,

            SUPERSCRIPT,
            SUBSCRIPT,
            BRAILLE,

            HIRAGANA,
            KATAKANA,
            CJK_IDEOGRAPHS,
            HANGUL_SYLLABLES,

            CARRIAGE_RETURN,

            OTHERWISE,
        } ;

        /*
         * Classify classes of unicode code points.
         * The class criteria is based on vim/mbyte.c.
         *
         * @param[in] (cp) UTF-32 character corresponding the code point.
         * @return CharType
         */
        CharType classify_codepoint(char32_t cp) ;
    }
}

#endif
