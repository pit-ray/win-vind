#ifndef _EDI_CHANGE_HPP
#define _EDI_CHANGE_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace textanalyze {
        struct SelRes ;
    }

    struct EdiChangeHighlightText : public BindedFuncCreator<EdiChangeHighlightText> {
        explicit EdiChangeHighlightText() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EdiChangeLines : public BindedFuncCreator<EdiChangeLines> {
        explicit EdiChangeLines() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EdiChangeChars : public BindedFuncCreator<EdiChangeChars> {
        explicit EdiChangeChars() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EdiChangeUntilEOL : public BindedFuncCreator<EdiChangeUntilEOL> {
        explicit EdiChangeUntilEOL() ;
        static void sprocess(
                unsigned int repeat_num=1,
                const textanalyze::SelRes* const exres=nullptr) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
