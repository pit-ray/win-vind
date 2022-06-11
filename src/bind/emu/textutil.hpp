#ifndef _TEXT_UTIL_HPP
#define _TEXT_UTIL_HPP

#include "util/disable_compiler_warning.hpp"

#include <functional>
#include <string>

#include "util/enable_compiler_warning.hpp"


namespace vind
{
    namespace bind
    {
        struct SelectedTextResult {
            std::string str ;

            //Some editors, for example Microsoft Office Word, has a visible end of line (EOL) mark.
            //In the case, if pushup LShift + End, the line is selected including CR-LF.
            bool having_EOL ;

            explicit SelectedTextResult(const std::string& outstr, bool havEOL)
            : str(outstr),
              having_EOL(havEOL)
            {}

            explicit SelectedTextResult()
            : str(),
              having_EOL(false)
            {}
        } ;

        SelectedTextResult get_selected_text(
                std::function<void()> clip_func,
                bool backup=false) ;
        /*
         * Some editors have a visible EOL mark in a line.
         * This function select text from current position to EOL except for the visible EOL mark.
         * If text could not be selected or only null characters are selected, returns false 
         */
        bool select_line_until_EOL() ;

        void clear_clipboard_with_null() ;
    }
}

#endif
