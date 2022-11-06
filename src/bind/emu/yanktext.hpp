#ifndef _EDI_YANK_HPP
#define _EDI_YANK_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct SelectedTextResult ;

        //Copy
        struct YankHighlightText : public BindedFuncVoid<YankHighlightText> {
            explicit YankHighlightText() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct YankLine : public BindedFuncVoid<YankLine> {
            explicit YankLine() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
