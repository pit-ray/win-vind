#ifndef _SELECT_WINDOW_HPP
#define _SELECT_WINDOW_HPP

#include "binded_func_with_creator.hpp"

struct SwitchWindow : public BindedFuncWithCreator<SwitchWindow>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct SelectLeftWindow : public BindedFuncWithCreator<SelectLeftWindow>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct SelectRightWindow : public BindedFuncWithCreator<SelectRightWindow>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct SelectUpperWindow : public BindedFuncWithCreator<SelectUpperWindow>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct SelectLowerWindow : public BindedFuncWithCreator<SelectLowerWindow>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct SelectWindowWithKeypush : public BindedFuncWithCreator<SelectWindowWithKeypush>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;


#endif
