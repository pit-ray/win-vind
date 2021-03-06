#ifndef _TEXT_ANALYZER_HPP
#define _TEXT_ANALYZER_HPP

#include "disable_gcc_warning.hpp"

#include <functional>
#include <string>

#include "enable_gcc_warning.hpp"

namespace vind
{
    namespace textanalyze {
        struct SelRes {
            std::string str ;

            //Some editors, for example Microsoft Office Word, has a visible end of line (EOL) mark.
            //In the case, if pushup LShift + End, the line is selected including CR-LF.
            bool having_EOL ;

            explicit SelRes(const std::string& outstr, bool havEOL)
            : str(outstr),
              having_EOL(havEOL)
            {}

            explicit SelRes()
            : str(),
              having_EOL(false)
            {}
        } ;

        SelRes get_selected_text(
                std::function<void()> clip_func,
                bool backup=false) ;
    }
}

#endif
