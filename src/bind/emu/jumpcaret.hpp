#ifndef _EDI_JUMP_CARET_HPP
#define _EDI_JUMP_CARET_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct JumpCaretToBOL : public BindedFuncVoid<JumpCaretToBOL> {
            explicit JumpCaretToBOL() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct JumpCaretToEOL : public BindedFuncVoid<JumpCaretToEOL> {
            explicit JumpCaretToEOL() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct JumpCaretToBOF : public BindedFuncVoid<JumpCaretToBOF> {
            explicit JumpCaretToBOF() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct JumpCaretToEOF : public BindedFuncVoid<JumpCaretToEOF> {
            explicit JumpCaretToEOF() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}
#endif
