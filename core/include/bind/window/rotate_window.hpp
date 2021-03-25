#ifndef _ROTATE_WINDOW_HPP
#define _ROTATE_WINDOW_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct RotateWindows : public BindedFuncWithCreator<RotateWindows> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct RotateWindowsInReverse: public BindedFuncWithCreator<RotateWindowsInReverse> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
