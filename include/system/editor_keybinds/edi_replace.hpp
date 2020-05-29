#ifndef _EDI_REPLACE_HPP
#define _EDI_REPLACE_HPP

#include "binded_function_with_creator.hpp"

struct EdiNReplaceChar : public BindedFunctionWithCreator<EdiNReplaceChar>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNReplaceSequence : public BindedFunctionWithCreator<EdiNReplaceSequence>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif