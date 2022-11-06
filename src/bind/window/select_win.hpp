#ifndef _SELECT_WINDOW_HPP
#define _SELECT_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct SelectLeftWindow : public BindedFuncVoid<SelectLeftWindow> {
            explicit SelectLeftWindow() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct SelectRightWindow : public BindedFuncVoid<SelectRightWindow> {
            explicit SelectRightWindow() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct SelectUpperWindow : public BindedFuncVoid<SelectUpperWindow> {
            explicit SelectUpperWindow() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct SelectLowerWindow : public BindedFuncVoid<SelectLowerWindow> {
            explicit SelectLowerWindow() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
