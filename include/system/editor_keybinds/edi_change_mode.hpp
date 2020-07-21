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

struct Change2EdiBOLInsert : public BindedFunctionWithCreator<Change2EdiBOLInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiBkInsert : public BindedFunctionWithCreator<Change2EdiBkInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiEOLInsert : public BindedFunctionWithCreator<Change2EdiEOLInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiNlInsertBelow : public BindedFunctionWithCreator<Change2EdiNlInsertBelow>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiNlInsertAbove : public BindedFunctionWithCreator<Change2EdiNlInsertAbove>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiVisual : public BindedFunctionWithCreator<Change2EdiVisual>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct Change2EdiLineVisual : public BindedFunctionWithCreator<Change2EdiLineVisual>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif