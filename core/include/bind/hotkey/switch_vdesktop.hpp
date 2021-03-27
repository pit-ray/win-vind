#ifndef _SWITCH_TASKVIEW_HPP
#define _SWITCH_TASKVIEW_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct SwitchVDesktop2Left : public BindedFuncWithCreator<SwitchVDesktop2Left> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct SwitchVDesktop2Right : public BindedFuncWithCreator<SwitchVDesktop2Right> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CreateNewVDesktop : public BindedFuncWithCreator<CreateNewVDesktop> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CloseCurrentVDesktop : public BindedFuncWithCreator<CloseCurrentVDesktop> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct TaskView : public BindedFuncWithCreator<TaskView> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
