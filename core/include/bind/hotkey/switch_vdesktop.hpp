#ifndef _SWITCH_TASKVIEW_HPP
#define _SWITCH_TASKVIEW_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    struct SwitchVDesktop2Left : public BindedFuncCreator<SwitchVDesktop2Left> {
        explicit SwitchVDesktop2Left() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct SwitchVDesktop2Right : public BindedFuncCreator<SwitchVDesktop2Right> {
        explicit SwitchVDesktop2Right() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct CreateNewVDesktop : public BindedFuncCreator<CreateNewVDesktop> {
        explicit CreateNewVDesktop() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct CloseCurrentVDesktop : public BindedFuncCreator<CloseCurrentVDesktop> {
        explicit CloseCurrentVDesktop() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct TaskView : public BindedFuncCreator<TaskView> {
        explicit TaskView() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
