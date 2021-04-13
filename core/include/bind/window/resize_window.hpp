#ifndef _RESIZE_WINDOW_HPP
#define _RESIZE_WINDOW_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct ResizeWindowWidth: public BindedFuncCreator<ResizeWindowWidth> {
        static void sprocess(long width=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct IncreaseWindowWidth : public BindedFuncCreator<IncreaseWindowWidth> {
        static void sprocess(long delta=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct DecreaseWindowWidth: public BindedFuncCreator<DecreaseWindowWidth> {
        static void sprocess(long delta=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct ResizeWindowHeight : public BindedFuncCreator<ResizeWindowHeight> {
        static void sprocess(long height=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct IncreaseWindowHeight : public BindedFuncCreator<IncreaseWindowHeight> {
        static void sprocess(long delta=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct DecreaseWindowHeight : public BindedFuncCreator<DecreaseWindowHeight> {
        static void sprocess(long delta=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
