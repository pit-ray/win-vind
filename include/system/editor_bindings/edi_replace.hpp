#ifndef _EDI_REPLACE_HPP
#define _EDI_REPLACE_HPP

#include "binded_func_with_creator.hpp"

struct EdiNReplaceChar : public BindedFuncWithCreator<EdiNReplaceChar>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNReplaceSequence : public BindedFuncWithCreator<EdiNReplaceSequence>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

#endif
