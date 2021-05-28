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

    struct EdiDeleteLinesAndStartInsert : public BindedFuncCreator<EdiDeleteLinesAndStartInsert> {
        explicit EdiDeleteLinesAndStartInsert() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EdiDeleteCharsAndStartInsert : public BindedFuncCreator<EdiDeleteCharsAndStartInsert> {
        explicit EdiDeleteCharsAndStartInsert() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EdiDeleteUntilEOLAndStartInsert : public BindedFuncCreator<EdiDeleteUntilEOLAndStartInsert> {
        explicit EdiDeleteUntilEOLAndStartInsert() ;
        static void sprocess(
                unsigned int repeat_num=1,
                const textanalyze::SelRes* const exres=nullptr) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
