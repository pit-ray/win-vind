#ifndef _JUMP_CURSOR_HPP
#define _JUMP_CURSOR_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct JumpToLeft : public BindedFuncVoid<JumpToLeft> {
            explicit JumpToLeft() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct JumpToRight : public BindedFuncVoid<JumpToRight> {
            explicit JumpToRight() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct JumpToTop : public BindedFuncVoid<JumpToTop> {
            explicit JumpToTop() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct JumpToBottom : public BindedFuncVoid<JumpToBottom> {
            explicit JumpToBottom() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct JumpToHorizontalCenter : public BindedFuncVoid<JumpToHorizontalCenter> {
            explicit JumpToHorizontalCenter() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct JumpToVerticalCenter : public BindedFuncVoid<JumpToVerticalCenter> {
            explicit JumpToVerticalCenter() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}
#endif
