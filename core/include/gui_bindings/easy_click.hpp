#ifndef _EASY_CLICK_HPP
#define _EASY_CLICK_HPP

#include "binded_func_with_creator.hpp"

namespace vind
{
    namespace EasyClick {
        void initialize() ;
    }

    struct EasyClickLeft : public BindedFuncWithCreator<EasyClickLeft> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EasyClickRight : public BindedFuncWithCreator<EasyClickRight> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EasyClickMid : public BindedFuncWithCreator<EasyClickMid> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EasyClickHover : public BindedFuncWithCreator<EasyClickHover> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct UpdateEasyClick : public BindedFuncWithCreator<UpdateEasyClick> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
