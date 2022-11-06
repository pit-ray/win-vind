#ifndef _SNAP_WINDOW_HPP
#define _SNAP_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct SnapCurrentWindow2Left : public BindedFuncVoid<SnapCurrentWindow2Left> {
            explicit SnapCurrentWindow2Left() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct SnapCurrentWindow2Right : public BindedFuncVoid<SnapCurrentWindow2Right> {
            explicit SnapCurrentWindow2Right() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct SnapCurrentWindow2Top : public BindedFuncVoid<SnapCurrentWindow2Top> {
            explicit SnapCurrentWindow2Top() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct SnapCurrentWindow2Bottom : public BindedFuncVoid<SnapCurrentWindow2Bottom> {
            explicit SnapCurrentWindow2Bottom() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
