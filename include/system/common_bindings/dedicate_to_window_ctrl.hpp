#ifndef _DEDICATE_TO_WINDOW_CTRL_HPP
#define _DEDICATE_TO_WINDOW_CTRL_HPP

#include "binded_func_with_creator.hpp"

struct EnableTargetingOfDedicate2Window : public BindedFuncWithCreator<EnableTargetingOfDedicate2Window>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct DisableTargetingOfDedicate2Window : public BindedFuncWithCreator<DisableTargetingOfDedicate2Window>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;
#endif
