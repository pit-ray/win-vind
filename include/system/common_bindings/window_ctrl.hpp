#ifndef _WINDOW_CTRL_HPP
#define _WINDOW_CTRL_HPP

#include "binded_func_with_creator.hpp"

struct CloseCurrentWindow : public BindedFuncWithCreator<CloseCurrentWindow>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct SwitchWindow : public BindedFuncWithCreator<SwitchWindow>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct MaximizeCurrentWindow : public BindedFuncWithCreator<MaximizeCurrentWindow>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct MinimizeCurrentWindow : public BindedFuncWithCreator<MinimizeCurrentWindow>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct SnapCurrentWindow2Left : public BindedFuncWithCreator<SnapCurrentWindow2Left>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct SnapCurrentWindow2Right : public BindedFuncWithCreator<SnapCurrentWindow2Right>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct OpenNewCurrentWindow : public BindedFuncWithCreator<OpenNewCurrentWindow>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct ReloadCurrentWindow : public BindedFuncWithCreator<ReloadCurrentWindow>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Switch2LeftTab : public BindedFuncWithCreator<Switch2LeftTab>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Switch2RightTab : public BindedFuncWithCreator<Switch2RightTab>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;
#endif
