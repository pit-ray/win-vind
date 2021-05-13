#ifndef _EDI_JUMP_CARET_HPP
#define _EDI_JUMP_CARET_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct EdiJumpCaret2BOL : public BindedFuncCreator<EdiJumpCaret2BOL> {
        explicit EdiJumpCaret2BOL() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;

        bool is_for_moving_caret() const noexcept override ;
    } ;

    struct EdiJumpCaret2EOL : public BindedFuncCreator<EdiJumpCaret2EOL> {
        explicit EdiJumpCaret2EOL() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;

        bool is_for_moving_caret() const noexcept override ;
    } ;

    struct EdiNJumpCaret2Line_DfBOF : public BindedFuncCreator<EdiNJumpCaret2Line_DfBOF> {
        explicit EdiNJumpCaret2Line_DfBOF() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;

        bool is_for_moving_caret() const noexcept override ;
    } ;

    struct EdiNJumpCaret2Line_DfEOF : public BindedFuncCreator<EdiNJumpCaret2Line_DfEOF> {
        explicit EdiNJumpCaret2Line_DfEOF() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;

        bool is_for_moving_caret() const noexcept override ;
    } ;
}
#endif
