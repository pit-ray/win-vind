#ifndef _WINDOW_CTRL_HPP
#define _WINDOW_CTRL_HPP

#include "binded_func_with_creator.hpp"

#include <windows.h>

namespace WindowCtrl
{
    bool is_valid_hwnd(HWND hwnd) ;
    bool is_valid_rect(HWND hwnd, RECT& rect) ;
}

struct OpenNewCurrentWindow : public BindedFuncWithCreator<OpenNewCurrentWindow>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct CloseCurrentWindow : public BindedFuncWithCreator<CloseCurrentWindow>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct ReloadCurrentWindow : public BindedFuncWithCreator<ReloadCurrentWindow>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

#endif
