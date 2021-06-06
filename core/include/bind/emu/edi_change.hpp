#ifndef _EDI_CHANGE_HPP
#define _EDI_CHANGE_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace textanalyze {
        struct SelRes ;
    }

    struct ChangeHighlightText : public BindedFuncCreator<ChangeHighlightText> {
        explicit ChangeHighlightText() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ChangeLine : public BindedFuncCreator<ChangeLine> {
        explicit ChangeLine() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ChangeChar : public BindedFuncCreator<ChangeChar> {
        explicit ChangeChar() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ChangeUntilEOL : public BindedFuncCreator<ChangeUntilEOL> {
        explicit ChangeUntilEOL() ;
        static void sprocess(
                unsigned int repeat_num=1,
                const textanalyze::SelRes* const exres=nullptr) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
