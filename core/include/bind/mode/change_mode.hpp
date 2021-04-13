#ifndef _CHANGE_MODE_HPP
#define _CHANGE_MODE_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct Change2Normal : public BindedFuncCreator<Change2Normal> {
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct Change2Insert : public BindedFuncCreator<Change2Insert> {
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct Change2Visual : public BindedFuncCreator<Change2Visual> {
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct MyConfigWindowNormal : public BindedFuncCreator<MyConfigWindowNormal> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct MyConfigWindowInsert : public BindedFuncCreator<MyConfigWindowInsert> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
