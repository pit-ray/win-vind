#ifndef _EDI_CHANGE_MOVE_HPP
#define _EDI_CHANGE_MOVE_HPP

#include "binded_func_with_creator.hpp"

struct Change2EdiNormal : public BindedFuncWithCreator<Change2EdiNormal>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;


struct Change2EdiInsert : public BindedFuncWithCreator<Change2EdiInsert>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiBOLInsert : public BindedFuncWithCreator<Change2EdiBOLInsert>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiBkInsert : public BindedFuncWithCreator<Change2EdiBkInsert>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiEOLInsert : public BindedFuncWithCreator<Change2EdiEOLInsert>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiNlInsertBelow : public BindedFuncWithCreator<Change2EdiNlInsertBelow>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiNlInsertAbove : public BindedFuncWithCreator<Change2EdiNlInsertAbove>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiVisual : public BindedFuncWithCreator<Change2EdiVisual>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;


struct Change2EdiLineVisual : public BindedFuncWithCreator<Change2EdiLineVisual>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const bool vclmodeout=true) ;
    static const std::string sname() noexcept ;
} ;

#endif
