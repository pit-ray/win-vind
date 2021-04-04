#ifndef _EDI_CHANGE_MOVE_HPP
#define _EDI_CHANGE_MOVE_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct Change2EdiNormal : public BindedFuncWithCreator<Change2EdiNormal> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;


    struct Change2EdiInsert : public BindedFuncWithCreator<Change2EdiInsert> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;

    struct Change2EdiBOLInsert : public BindedFuncWithCreator<Change2EdiBOLInsert> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;

    struct Change2EdiBkInsert : public BindedFuncWithCreator<Change2EdiBkInsert> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;

    struct Change2EdiEOLInsert : public BindedFuncWithCreator<Change2EdiEOLInsert> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;

    struct Change2EdiNlInsertBelow : public BindedFuncWithCreator<Change2EdiNlInsertBelow> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;

    struct Change2EdiNlInsertAbove : public BindedFuncWithCreator<Change2EdiNlInsertAbove> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;

    struct Change2EdiVisual : public BindedFuncWithCreator<Change2EdiVisual> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;


    struct Change2EdiLineVisual : public BindedFuncWithCreator<Change2EdiLineVisual> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr,
                const bool vclmodeout=true) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
