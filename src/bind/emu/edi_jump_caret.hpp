#ifndef _EDI_JUMP_CARET_HPP
#define _EDI_JUMP_CARET_HPP

#include "movebase.hpp"

namespace vind
{
    namespace bind
    {
        struct JumpCaretToBOL : public MoveBaseCreator<JumpCaretToBOL> {
            explicit JumpCaretToBOL() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct JumpCaretToEOL : public MoveBaseCreator<JumpCaretToEOL> {
            explicit JumpCaretToEOL() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct JumpCaretToBOF : public MoveBaseCreator<JumpCaretToBOF> {
            explicit JumpCaretToBOF() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct JumpCaretToEOF : public MoveBaseCreator<JumpCaretToEOF> {
            explicit JumpCaretToEOF() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}
#endif
