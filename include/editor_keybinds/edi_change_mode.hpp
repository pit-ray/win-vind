#ifndef _EDI_CHANGE_MOVE_HPP
#define _EDI_CHANGE_MOVE_HPP

#include "binded_function_with_creator.hpp"

struct Change2EdiNormal : public BindedFunctionWithCreator<Change2EdiNormal>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct Change2EdiInsert : public BindedFunctionWithCreator<Change2EdiInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct Change2EdiBkInsert : public BindedFunctionWithCreator<Change2EdiBkInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ; 


struct Change2EdiNlInsert : public BindedFunctionWithCreator<Change2EdiNlInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ; 


struct Change2EdiVisual : public BindedFunctionWithCreator<Change2EdiVisual>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif