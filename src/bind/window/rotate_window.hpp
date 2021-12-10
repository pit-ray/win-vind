#ifndef _ROTATE_WINDOW_HPP
#define _ROTATE_WINDOW_HPP

#include "bind/binded_func.hpp"

namespace vind
{
    namespace bind
    {
        struct RotateWindows : public BindedFuncVoid<RotateWindows> {
            explicit RotateWindows() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct RotateWindowsInReverse: public BindedFuncVoid<RotateWindowsInReverse> {
            explicit RotateWindowsInReverse() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
