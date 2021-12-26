#ifndef _TEXT_ANALYZER_HPP
#define _TEXT_ANALYZER_HPP

#include "util/disable_compiler_warning.hpp"

#include <functional>
#include <string>

#include "util/enable_compiler_warning.hpp"

namespace vind
{
    namespace bind {
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
    }
}

#endif
