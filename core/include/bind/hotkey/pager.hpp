#ifndef _PAGER_HPP
#define _PAGER_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct Move2NextPage : public BindedFuncWithCreator<Move2NextPage> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct Move2PrevPage : public BindedFuncWithCreator<Move2PrevPage> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}
#endif
