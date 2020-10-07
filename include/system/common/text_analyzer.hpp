#ifndef _TEXT_ANALYZER_HPP
#define _TEXT_ANALYZER_HPP

#include <functional>
#include <string>

namespace TextAnalyzer
{
    struct SelRes {
        bool success ;
        std::string str ;

        //Some editors, for example Microsoft Office Word, has a visible end of line (EOL) mark.
        //In the case, if pushup LShift + End, the line is selected including CR-LF.
        bool having_EOL ;

        explicit SelRes(bool res, std::string outstr, bool havEOL)
        : success(res),
          str(outstr),
          having_EOL(havEOL)
        {}

        explicit SelRes(bool res)
        : SelRes(res, "", false)
        {}

        virtual ~SelRes() noexcept = default ;
        SelRes(SelRes&&) noexcept = default ;
        SelRes& operator=(SelRes&&) noexcept = default ;
        SelRes(const SelRes&) = default ;
        SelRes& operator=(const SelRes&) = default ;

        operator bool() const noexcept {
            return success ;
        }
    } ;

    const SelRes get_selected_text(std::function<bool()> clip_func, const bool backup=false) ;
}


#endif
