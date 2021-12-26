#ifndef _SWITCH_TASKVIEW_HPP
#define _SWITCH_TASKVIEW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct SwitchVDesktop2Left : public BindedFuncVoid<SwitchVDesktop2Left> {
            explicit SwitchVDesktop2Left() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct SwitchVDesktop2Right : public BindedFuncVoid<SwitchVDesktop2Right> {
            explicit SwitchVDesktop2Right() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct CreateNewVDesktop : public BindedFuncVoid<CreateNewVDesktop> {
            explicit CreateNewVDesktop() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct CloseCurrentVDesktop : public BindedFuncVoid<CloseCurrentVDesktop> {
            explicit CloseCurrentVDesktop() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct TaskView : public BindedFuncVoid<TaskView> {
            explicit TaskView() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
