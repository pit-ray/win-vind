#ifndef _EDI_CHANGE_MOVE_HPP
#define _EDI_CHANGE_MOVE_HPP

#include "key_binding_with_creator.hpp"

struct Change2EdiNormal : public KeyBindingWithCreator<Change2EdiNormal>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct Change2EdiInsert : public KeyBindingWithCreator<Change2EdiInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiBOLInsert : public KeyBindingWithCreator<Change2EdiBOLInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiBkInsert : public KeyBindingWithCreator<Change2EdiBkInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiEOLInsert : public KeyBindingWithCreator<Change2EdiEOLInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiNlInsertBelow : public KeyBindingWithCreator<Change2EdiNlInsertBelow>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiNlInsertAbove : public KeyBindingWithCreator<Change2EdiNlInsertAbove>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Change2EdiVisual : public KeyBindingWithCreator<Change2EdiVisual>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct Change2EdiLineVisual : public KeyBindingWithCreator<Change2EdiLineVisual>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif
