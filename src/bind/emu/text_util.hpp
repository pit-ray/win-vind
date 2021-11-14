#ifndef _TEXT_UTIL_HPP
#define _TEXT_UTIL_HPP

#include "text_analyzer.hpp"

namespace vind
{
    namespace bind
    {
        //Some editors have a visible EOL mark in a line.
        //This function select text from current position to EOL except for the visible EOL mark.
        //If the line has only null characters, it does not select.
        //  <EOL mark exists> [select] NONE    [clipboard] null characters with EOL.    (neighborhoods of LSB are 0x00)
        //  <plain text>      [select] NONE    [clipboard] null characters without EOL. (neighborhoods of LSB are 0x?0)
        bool select_line_until_EOL(const SelectedTextResult* const exres) ;

        void clear_clipboard_with_null() ;
    }
}

#endif
