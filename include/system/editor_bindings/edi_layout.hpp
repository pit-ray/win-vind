#ifndef _EDI_LAYOUT_HPP
#define _EDI_LAYOUT_HPP

#include "binded_function_with_creator.hpp"

struct EdiNRemoveEOL : public BindedFunctionWithCreator<EdiNRemoveEOL>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif