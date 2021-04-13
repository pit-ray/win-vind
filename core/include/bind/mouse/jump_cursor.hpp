#ifndef _JUMP_CURSOR_HPP
#define _JUMP_CURSOR_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct Jump2Left : public BindedFuncCreator<Jump2Left> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct Jump2Right : public BindedFuncCreator<Jump2Right> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct Jump2Top : public BindedFuncCreator<Jump2Top> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct Jump2Bottom : public BindedFuncCreator<Jump2Bottom> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct Jump2XCenter : public BindedFuncCreator<Jump2XCenter> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct Jump2YCenter : public BindedFuncCreator<Jump2YCenter> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}
#endif
