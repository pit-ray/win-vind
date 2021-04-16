#ifndef _CLIPBOARD_HPP
#define _CLIPBOARD_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct CBCopy : public BindedFuncCreator<CBCopy> {
        explicit CBCopy() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct CBPaste : public BindedFuncCreator<CBPaste> {
        explicit CBPaste() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct CBCut : public BindedFuncCreator<CBCut> {
        explicit CBCut() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct CBDelete : public BindedFuncCreator<CBDelete> {
        explicit CBDelete() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct CBBackSpace : public BindedFuncCreator<CBBackSpace> {
        explicit CBBackSpace() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}
#endif
