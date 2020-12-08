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

    void select_words() noexcept ;
    void select_line_EOL2BOL() ;
    void select_line_BOL2EOL() ;
    void unselect() ;

    Mode get_mode() noexcept ;
    void moving_update() ;

    bool is_first_line_selection() noexcept ;
}


#endif
