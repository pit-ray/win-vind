#ifndef _SIMPL_TEXT_SELECTER_HPP
#define _SIMPL_TEXT_SELECTER_HPP

namespace SimplTextSelecter
{
    enum class Mode : unsigned char {
        Word,
        EOL2BOL,
        BOL2EOL,

        Unselect,
    } ;

    bool is_select_words() noexcept ;
    bool is_select_line_EOL2BOL() noexcept ;
    bool is_select_line_BOL2EOL() noexcept ;
    bool is_unselect() noexcept ;

    Mode get_mode() noexcept ;

    bool is_moving_update() noexcept ;

    bool is_first_line_selection() noexcept ;
}


#endif