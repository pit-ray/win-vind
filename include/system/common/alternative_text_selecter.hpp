#ifndef _ALTERNATIVE_TEXT_SELECTER_HPP
#define _ALTERNATIVE_TEXT_SELECTER_HPP

namespace AlternativeTextSelecter
{
    enum class Mode : unsigned char {
        WORD,
        EOL2BOL,
        BOL2EOL,

        UNSELECT,
    } ;

    bool is_select_words() noexcept ;
    bool is_select_line_EOL2BOL() noexcept ;
    bool is_select_line_BOL2EOL() noexcept ;
    bool is_unselect() noexcept ;

    Mode get_mode() noexcept ;
}


#endif