#ifndef _RESIZE_WINDOW_HPP
#define _RESIZE_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct ResizeWindowWidth: public BindedFuncVoid<ResizeWindowWidth> {
            explicit ResizeWindowWidth() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct IncreaseWindowWidth : public BindedFuncVoid<IncreaseWindowWidth> {
            explicit IncreaseWindowWidth() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct DecreaseWindowWidth: public BindedFuncVoid<DecreaseWindowWidth> {
            explicit DecreaseWindowWidth() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct ResizeWindowHeight : public BindedFuncVoid<ResizeWindowHeight> {
            explicit ResizeWindowHeight() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct IncreaseWindowHeight : public BindedFuncVoid<IncreaseWindowHeight> {
            explicit IncreaseWindowHeight() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct DecreaseWindowHeight : public BindedFuncVoid<DecreaseWindowHeight> {
            explicit DecreaseWindowHeight() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
