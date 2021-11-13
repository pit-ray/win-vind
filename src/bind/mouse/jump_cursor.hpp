#ifndef _JUMP_CURSOR_HPP
#define _JUMP_CURSOR_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct JumpToLeft : public BindedFuncCreator<JumpToLeft> {
        explicit JumpToLeft() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct JumpToRight : public BindedFuncCreator<JumpToRight> {
        explicit JumpToRight() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct JumpToTop : public BindedFuncCreator<JumpToTop> {
        explicit JumpToTop() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct JumpToBottom : public BindedFuncCreator<JumpToBottom> {
        explicit JumpToBottom() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct JumpToHorizontalCenter : public BindedFuncCreator<JumpToHorizontalCenter> {
        explicit JumpToHorizontalCenter() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct JumpToVerticalCenter : public BindedFuncCreator<JumpToVerticalCenter> {
        explicit JumpToVerticalCenter() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;
}
#endif
