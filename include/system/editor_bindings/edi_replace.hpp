#ifndef _EDI_REPLACE_HPP
#define _EDI_REPLACE_HPP

#include "key_binding_with_creator.hpp"

struct EdiNReplaceChar : public KeyBindingWithCreator<EdiNReplaceChar>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNReplaceSequence : public KeyBindingWithCreator<EdiNReplaceSequence>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif