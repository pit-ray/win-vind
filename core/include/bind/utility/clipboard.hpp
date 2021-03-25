#ifndef _CLIPBOARD_HPP
#define _CLIPBOARD_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct CBCopy : public BindedFuncWithCreator<CBCopy> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CBPaste : public BindedFuncWithCreator<CBPaste> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CBCut : public BindedFuncWithCreator<CBCut> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CBDelete : public BindedFuncWithCreator<CBDelete> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CBBackSpace : public BindedFuncWithCreator<CBBackSpace> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}
#endif
