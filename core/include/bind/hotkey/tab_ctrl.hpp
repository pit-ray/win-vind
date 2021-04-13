#ifndef _TAB_CTRL_HPP
#define _TAB_CTRL_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct Switch2LeftTab : public BindedFuncCreator<Switch2LeftTab> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct Switch2RightTab : public BindedFuncCreator<Switch2RightTab> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct OpenNewTab : public BindedFuncCreator<OpenNewTab> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct CloseCurrentTab : public BindedFuncCreator<CloseCurrentTab> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
