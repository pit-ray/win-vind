#ifndef _SWITCH_TASKVIEW_HPP
#define _SWITCH_TASKVIEW_HPP

#include "key_binding_with_creator.hpp"
#include "command_with_creator.hpp"

struct SwitchVDesktop2Left : public KeyBindingWithCreator<SwitchVDesktop2Left>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct SwitchVDesktop2Right : public KeyBindingWithCreator<SwitchVDesktop2Right>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct CreateNewVDesktop : public KeyBindingWithCreator<CreateNewVDesktop>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct CloseCurrentVDesktop : public KeyBindingWithCreator<CloseCurrentVDesktop>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct TaskView
: public KeyBindingWithCreator<TaskView>,
  public CommandWithCreator<TaskView>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;
private:
    static bool common_process() ;
} ;
#endif
