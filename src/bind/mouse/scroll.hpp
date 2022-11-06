#ifndef _SCROLL_HPP
#define _SCROLL_HPP
#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct ScrollUp : public BindedFuncVoid<ScrollUp> {
            explicit ScrollUp() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;

        } ;

        struct ScrollDown : public BindedFuncVoid<ScrollDown> {
            explicit ScrollDown() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct ScrollUpHalfPage : public BindedFuncVoid<ScrollUpHalfPage> {
            explicit ScrollUpHalfPage() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct ScrollDownHalfPage : public BindedFuncVoid<ScrollDownHalfPage> {
            explicit ScrollDownHalfPage() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct ScrollUpOnePage : public BindedFuncVoid<ScrollUpOnePage> {
            explicit ScrollUpOnePage() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct ScrollDownOnePage : public BindedFuncVoid<ScrollDownOnePage> {
            explicit ScrollDownOnePage() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct ScrollLeft : public BindedFuncVoid<ScrollLeft> {
            explicit ScrollLeft() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct ScrollRight : public BindedFuncVoid<ScrollRight> {
            explicit ScrollRight() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct ScrollLeftHalfPage : public BindedFuncVoid<ScrollLeftHalfPage> {
            explicit ScrollLeftHalfPage() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct ScrollRightHalfPage : public BindedFuncVoid<ScrollRightHalfPage> {
            explicit ScrollRightHalfPage() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}
#endif
