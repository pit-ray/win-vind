#ifndef _EDI_JUMP_CARET_HPP
#define _EDI_JUMP_CARET_HPP

#include "movebase.hpp"

namespace vind
{
    namespace bind
    {
        struct JumpCaretToBOL : public MoveBaseCreator<JumpCaretToBOL> {
            explicit JumpCaretToBOL() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct JumpCaretToEOL : public MoveBaseCreator<JumpCaretToEOL> {
            explicit JumpCaretToEOL() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct JumpCaretToBOF : public MoveBaseCreator<JumpCaretToBOF> {
            explicit JumpCaretToBOF() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct JumpCaretToEOF : public MoveBaseCreator<JumpCaretToEOF> {
            explicit JumpCaretToEOF() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}
#endif
