#ifndef _EDI_MOVE_CARET_HPP
#define _EDI_MOVE_CARET_HPP

#include "movebase.hpp"


namespace vind
{
    namespace bind
    {
        struct MoveCaretLeft : public MoveBaseCreator<MoveCaretLeft> {
            explicit MoveCaretLeft() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ; } ;

        struct MoveCaretRight : public MoveBaseCreator<MoveCaretRight> {
            explicit MoveCaretRight() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveCaretUp : public MoveBaseCreator<MoveCaretUp> {
            explicit MoveCaretUp() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveCaretDown : public MoveBaseCreator<MoveCaretDown> {
            explicit MoveCaretDown() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct MoveCaretWordForward : public MoveBaseCreator<MoveCaretWordForward> {
            explicit MoveCaretWordForward() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveCaretWordBackward : public MoveBaseCreator<MoveCaretWordBackward> {
            explicit MoveCaretWordBackward() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct MoveCaretNonBlankWordForward : public MoveBaseCreator<MoveCaretNonBlankWordForward> {
            explicit MoveCaretNonBlankWordForward() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveCaretNonBlankWordBackward : public MoveBaseCreator<MoveCaretNonBlankWordBackward> {
            explicit MoveCaretNonBlankWordBackward() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
