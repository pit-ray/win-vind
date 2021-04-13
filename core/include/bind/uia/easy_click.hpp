#ifndef _EASY_CLICK_HPP
#define _EASY_CLICK_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    namespace easyclick {
        void initialize() ;
    }

    struct EasyClickLeft : public BindedFuncCreator<EasyClickLeft> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EasyClickRight : public BindedFuncCreator<EasyClickRight> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EasyClickMid : public BindedFuncCreator<EasyClickMid> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EasyClickHover : public BindedFuncCreator<EasyClickHover> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
