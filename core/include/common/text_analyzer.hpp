#ifndef _TEXT_ANALYZER_HPP
#define _TEXT_ANALYZER_HPP

#include "disable_gcc_warning.hpp"
#include <functional>
#include <string>
#include "enable_gcc_warning.hpp"

namespace TextAnalyzer
{
    struct SelRes {
        std::string str ;

        //Some editors, for example Microsoft Office Word, has a visible end of line (EOL) mark.
        //In the case, if pushup LShift + End, the line is selected including CR-LF.
        bool having_EOL ;

        explicit SelRes(std::string outstr, bool havEOL)
        : str(outstr),
          having_EOL(havEOL)
        {}

        explicit SelRes()
        : str(),
          having_EOL(false)
        {}
    } ;

    const SelRes get_selected_text(
            std::function<void()> clip_func,
            const bool backup=false) ;
}


#endif
