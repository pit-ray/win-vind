#ifndef _EASY_CLICK_HPP
#define _EASY_CLICK_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    namespace easyclick {
        void initialize() ;
    }

    struct EasyClickLeft : public BindedFuncWithCreator<EasyClickLeft> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EasyClickRight : public BindedFuncWithCreator<EasyClickRight> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EasyClickMid : public BindedFuncWithCreator<EasyClickMid> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EasyClickHover : public BindedFuncWithCreator<EasyClickHover> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct UpdateEasyClick : public BindedFuncWithCreator<UpdateEasyClick> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
