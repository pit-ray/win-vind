#include "jump_cursor.hpp"

#include "screen_metrics.hpp"
#include "i_params.hpp"

#include "utility.hpp"

//Jump2Left
const std::string Jump2Left::sname() noexcept
{
    return "jump_to_left" ;
}

void Jump2Left::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(0, pos.y) ;
}


//Jump2Right
const std::string Jump2Right::sname() noexcept
{
    return "jump_to_right" ;
}

void Jump2Right::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;

    RECT rect ;
    ScreenMetrics::get_conbined_metrics(&rect) ;

    SetCursorPos(ScreenMetrics::width(rect) - iParams::get_i("screen_pos_buf"), pos.y) ;
}


//Jump2Top
const std::string Jump2Top::sname() noexcept
{
    return "jump_to_top" ;
}

void Jump2Top::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(pos.x, 0) ;
}


//Jump2Bottom
const std::string Jump2Bottom::sname() noexcept
{
    return "jump_to_bottom" ;
}

void Jump2Bottom::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;

    RECT rect ;
    ScreenMetrics::get_conbined_metrics(&rect) ;
    SetCursorPos(pos.x, ScreenMetrics::height(rect) - iParams::get_i("screen_pos_buf")) ;
}


//Jump2XCenter
const std::string Jump2XCenter::sname() noexcept
{
    return "jump_to_xcenter" ;
}

void Jump2XCenter::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;

    RECT rect ;
    ScreenMetrics::get_conbined_metrics(&rect) ;
    SetCursorPos(ScreenMetrics::width(rect) / 2, pos.y) ;
}


//Jump2YCenter
const std::string Jump2YCenter::sname() noexcept
{
    return "jump_to_ycenter" ;
}

void Jump2YCenter::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;

    RECT rect ;
    ScreenMetrics::get_conbined_metrics(&rect) ;
    SetCursorPos(pos.x, ScreenMetrics::height(rect) / 2) ;
}
