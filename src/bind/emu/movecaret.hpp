#ifndef _EDI_MOVE_CARET_HPP
#define _EDI_MOVE_CARET_HPP

#include "bind/bindedfunc.hpp"


namespace vind
{
    namespace bind
    {
        struct MoveCaretLeft : public BindedFuncVoid<MoveCaretLeft> {
            explicit MoveCaretLeft() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ; } ;

        struct MoveCaretRight : public BindedFuncVoid<MoveCaretRight> {
            explicit MoveCaretRight() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveCaretUp : public BindedFuncVoid<MoveCaretUp> {
            explicit MoveCaretUp() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveCaretDown : public BindedFuncVoid<MoveCaretDown> {
            explicit MoveCaretDown() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct MoveCaretWordForward : public BindedFuncVoid<MoveCaretWordForward> {
            explicit MoveCaretWordForward() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveCaretWordBackward : public BindedFuncVoid<MoveCaretWordBackward> {
            explicit MoveCaretWordBackward() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct MoveCaretNonBlankWordForward : public BindedFuncVoid<MoveCaretNonBlankWordForward> {
            explicit MoveCaretNonBlankWordForward() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveCaretNonBlankWordBackward : public BindedFuncVoid<MoveCaretNonBlankWordBackward> {
            explicit MoveCaretNonBlankWordBackward() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
