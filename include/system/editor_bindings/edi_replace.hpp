#ifndef _EDI_REPLACE_HPP
#define _EDI_REPLACE_HPP

#include "binded_func_with_creator.hpp"

struct EdiNReplaceChar : public BindedFuncWithCreator<EdiNReplaceChar>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNReplaceSequence : public BindedFuncWithCreator<EdiNReplaceSequence>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

#endif
