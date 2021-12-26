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
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct YankLine : public BindedFuncVoid<YankLine> {
            explicit YankLine() ;
            static void sprocess(
                    unsigned int repeat_num=1,
                    const SelectedTextResult* const exres=nullptr) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
