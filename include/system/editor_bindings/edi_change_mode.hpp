#ifndef _EDI_CHANGE_MOVE_HPP
#define _EDI_CHANGE_MOVE_HPP

#include "binded_func_with_creator.hpp"

struct Change2EdiNormal : public BindedFuncWithCreator<Change2EdiNormal>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct Change2EdiInsert : public BindedFuncWithCreator<Change2EdiInsert>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiBOLInsert : public BindedFuncWithCreator<Change2EdiBOLInsert>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiBkInsert : public BindedFuncWithCreator<Change2EdiBkInsert>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiEOLInsert : public BindedFuncWithCreator<Change2EdiEOLInsert>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiNlInsertBelow : public BindedFuncWithCreator<Change2EdiNlInsertBelow>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiNlInsertAbove : public BindedFuncWithCreator<Change2EdiNlInsertAbove>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiVisual : public BindedFuncWithCreator<Change2EdiVisual>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct Change2EdiLineVisual : public BindedFuncWithCreator<Change2EdiLineVisual>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

#endif
