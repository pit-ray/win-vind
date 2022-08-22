#ifndef _JUMP_CURSOR_HPP
#define _JUMP_CURSOR_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct JumpToLeft : public BindedFuncVoid<JumpToLeft> {
            explicit JumpToLeft() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct JumpToRight : public BindedFuncVoid<JumpToRight> {
            explicit JumpToRight() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct JumpToTop : public BindedFuncVoid<JumpToTop> {
            explicit JumpToTop() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct JumpToBottom : public BindedFuncVoid<JumpToBottom> {
            explicit JumpToBottom() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct JumpToHorizontalCenter : public BindedFuncVoid<JumpToHorizontalCenter> {
            explicit JumpToHorizontalCenter() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct JumpToVerticalCenter : public BindedFuncVoid<JumpToVerticalCenter> {
            explicit JumpToVerticalCenter() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}
#endif
