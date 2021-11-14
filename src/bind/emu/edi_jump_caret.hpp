#ifndef _EDI_JUMP_CARET_HPP
#define _EDI_JUMP_CARET_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace bind
    {
        struct JumpCaretToBOL : public BindedFuncCreator<JumpCaretToBOL> {
            explicit JumpCaretToBOL() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;

            bool is_for_moving_caret() const noexcept override ;
        } ;

        struct JumpCaretToEOL : public BindedFuncCreator<JumpCaretToEOL> {
            explicit JumpCaretToEOL() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;

            bool is_for_moving_caret() const noexcept override ;
        } ;

        struct JumpCaretToBOF : public BindedFuncCreator<JumpCaretToBOF> {
            explicit JumpCaretToBOF() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;

            bool is_for_moving_caret() const noexcept override ;
        } ;

        struct JumpCaretToEOF : public BindedFuncCreator<JumpCaretToEOF> {
            explicit JumpCaretToEOF() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;

            bool is_for_moving_caret() const noexcept override ;
        } ;
    }
}
#endif
