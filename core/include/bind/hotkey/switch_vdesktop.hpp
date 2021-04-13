#ifndef _SWITCH_TASKVIEW_HPP
#define _SWITCH_TASKVIEW_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct SwitchVDesktop2Left : public BindedFuncCreator<SwitchVDesktop2Left> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct SwitchVDesktop2Right : public BindedFuncCreator<SwitchVDesktop2Right> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CreateNewVDesktop : public BindedFuncCreator<CreateNewVDesktop> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct CloseCurrentVDesktop : public BindedFuncCreator<CloseCurrentVDesktop> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct TaskView : public BindedFuncCreator<TaskView> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
