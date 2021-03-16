#ifndef _FILER_HPP
#define _FILER_HPP

#include "binded_func_with_creator.hpp"

struct SaveOpenedFile : public BindedFuncWithCreator<SaveOpenedFile>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const parent_vkclgr,
            const CharLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct OpenOtherFile : public BindedFuncWithCreator<OpenOtherFile>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const parent_vkclgr,
            const CharLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

#endif
