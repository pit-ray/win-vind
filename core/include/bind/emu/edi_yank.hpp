#ifndef _EDI_YANK_HPP
#define _EDI_YANK_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace textanalyze {
        struct SelRes ;
    }

    //Copy
    struct EdiCopyHighlightText : public BindedFuncCreator<EdiCopyHighlightText> {
        explicit EdiCopyHighlightText() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EdiNCopyLine : public BindedFuncCreator<EdiNCopyLine> {
        explicit EdiNCopyLine() ;
        static void sprocess(
                unsigned int repeat_num=1,
                const textanalyze::SelRes* const exres=nullptr) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
