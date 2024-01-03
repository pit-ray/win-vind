#ifndef _MOVEWINDOW_HPP
#define _MOVEWINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct MoveWindowLeft : public BindedFuncVoid<MoveWindowLeft> {
            explicit MoveWindowLeft() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveWindowRight : public BindedFuncVoid<MoveWindowRight> {
            explicit MoveWindowRight() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveWindowUp : public BindedFuncVoid<MoveWindowUp> {
            explicit MoveWindowUp() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveWindowDown : public BindedFuncVoid<MoveWindowDown> {
            explicit MoveWindowDown() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
