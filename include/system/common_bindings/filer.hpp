#ifndef _FILER_HPP
#define _FILER_HPP

#include "binded_func_with_creator.hpp"

struct SaveOpenedFile : public BindedFuncWithCreator<SaveOpenedFile>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct CloseOpenedFile : public BindedFuncWithCreator<CloseOpenedFile>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct OpenOtherFile : public BindedFuncWithCreator<OpenOtherFile>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct MakeDir : public BindedFuncWithCreator<MakeDir>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

#endif
