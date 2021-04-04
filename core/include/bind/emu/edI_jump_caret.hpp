#ifndef _EDI_JUMP_CARET_HPP
#define _EDI_JUMP_CARET_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct EdiJumpCaret2BOL : public BindedFuncWithCreator<EdiJumpCaret2BOL> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;
    } ;

    struct EdiJumpCaret2EOL : public BindedFuncWithCreator<EdiJumpCaret2EOL> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;
    } ;

    struct EdiNJumpCaret2Line_DfBOF : public BindedFuncWithCreator<EdiNJumpCaret2Line_DfBOF> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;
    } ;

    struct EdiNJumpCaret2Line_DfEOF : public BindedFuncWithCreator<EdiNJumpCaret2Line_DfEOF> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;
    } ;
}
#endif
