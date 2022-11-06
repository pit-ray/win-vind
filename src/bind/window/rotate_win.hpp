#ifndef _ROTATE_WINDOW_HPP
#define _ROTATE_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct RotateWindows : public BindedFuncVoid<RotateWindows> {
            explicit RotateWindows() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct RotateWindowsInReverse: public BindedFuncVoid<RotateWindowsInReverse> {
            explicit RotateWindowsInReverse() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
