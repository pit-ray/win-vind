#ifndef _EASY_CLICK_HPP
#define _EASY_CLICK_HPP

#include "binded_func_with_creator.hpp"

namespace EsyClk
{
    void initialize() ;
}

struct EasyClick : public BindedFuncWithCreator<EasyClick>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;


struct UpdateEasyClick : public BindedFuncWithCreator<UpdateEasyClick>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

#endif
