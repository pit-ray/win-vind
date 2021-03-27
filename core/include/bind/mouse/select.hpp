#ifndef _SELECT_HPP
#define _SELECT_HPP
#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct SelectAll : public BindedFuncWithCreator<SelectAll> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct ForwardUINavigation : public BindedFuncWithCreator<ForwardUINavigation> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct BackwardUINavigation : public BindedFuncWithCreator<BackwardUINavigation> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct DecideFocusedUIObject : public BindedFuncWithCreator<DecideFocusedUIObject> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
