#ifndef _CLIPBOARD_HPP
#define _CLIPBOARD_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct CBCopy : public BindedFuncCreator<CBCopy> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CBPaste : public BindedFuncCreator<CBPaste> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CBCut : public BindedFuncCreator<CBCut> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CBDelete : public BindedFuncCreator<CBDelete> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CBBackSpace : public BindedFuncCreator<CBBackSpace> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}
#endif
