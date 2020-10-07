#ifndef _FILER_HPP
#define _FILER_HPP

#include "binded_func_with_creator.hpp"

struct SaveOpenedFile : public BindedFuncWithCreator<SaveOpenedFile>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct CloseOpenedFile : public BindedFuncWithCreator<CloseOpenedFile>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct OpenOtherFile : public BindedFuncWithCreator<OpenOtherFile>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct MakeDir : public BindedFuncWithCreator<MakeDir>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

#endif
