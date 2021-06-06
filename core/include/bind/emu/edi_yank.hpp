#ifndef _EDI_YANK_HPP
#define _EDI_YANK_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace textanalyze {
        struct SelRes ;
    }

    //Copy
    struct YankHighlightText : public BindedFuncCreator<YankHighlightText> {
        explicit YankHighlightText() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct YankLine : public BindedFuncCreator<YankLine> {
        explicit YankLine() ;
        static void sprocess(
                unsigned int repeat_num=1,
                const textanalyze::SelRes* const exres=nullptr) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
