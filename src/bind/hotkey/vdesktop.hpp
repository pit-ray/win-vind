#ifndef _SWITCH_TASKVIEW_HPP
#define _SWITCH_TASKVIEW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct SwitchVDesktop2Left : public BindedFuncVoid<SwitchVDesktop2Left> {
            explicit SwitchVDesktop2Left() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct SwitchVDesktop2Right : public BindedFuncVoid<SwitchVDesktop2Right> {
            explicit SwitchVDesktop2Right() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct CreateNewVDesktop : public BindedFuncVoid<CreateNewVDesktop> {
            explicit CreateNewVDesktop() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct CloseCurrentVDesktop : public BindedFuncVoid<CloseCurrentVDesktop> {
            explicit CloseCurrentVDesktop() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct TaskView : public BindedFuncVoid<TaskView> {
            explicit TaskView() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
