#ifndef _SELECT_HPP
#define _SELECT_HPP
#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct SelectAll : public BindedFuncCreator<SelectAll> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct ForwardUINavigation : public BindedFuncCreator<ForwardUINavigation> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct BackwardUINavigation : public BindedFuncCreator<BackwardUINavigation> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct DecideFocusedUIObject : public BindedFuncCreator<DecideFocusedUIObject> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
