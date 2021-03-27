#ifndef _SNAP_WINDOW_HPP
#define _SNAP_WINDOW_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct SnapCurrentWindow2Left : public BindedFuncWithCreator<SnapCurrentWindow2Left> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct SnapCurrentWindow2Right : public BindedFuncWithCreator<SnapCurrentWindow2Right> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct SnapCurrentWindow2Top : public BindedFuncWithCreator<SnapCurrentWindow2Top> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct SnapCurrentWindow2Bottom : public BindedFuncWithCreator<SnapCurrentWindow2Bottom> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
