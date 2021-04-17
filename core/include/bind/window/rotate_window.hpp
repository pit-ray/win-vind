#ifndef _ROTATE_WINDOW_HPP
#define _ROTATE_WINDOW_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    struct RotateWindows : public BindedFuncCreator<RotateWindows> {
        explicit RotateWindows() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct RotateWindowsInReverse: public BindedFuncCreator<RotateWindowsInReverse> {
        explicit RotateWindowsInReverse() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
