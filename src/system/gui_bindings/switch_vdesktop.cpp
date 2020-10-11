#include "switch_vdesktop.hpp"
#include "keybrd_eventer.hpp"
#include "utility.hpp"

using namespace std ;

//SwitchVDesktop2Left
const string SwitchVDesktop2Left::sname() noexcept
{
    return "switch_to_left_vdesktop" ;
}

void SwitchVDesktop2Left::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(first_call) KeybrdEventer::pushup(VKC_LCTRL, VKC_LWIN, VKC_LEFT) ;
}


//SwitchVDesktop2Right
const string SwitchVDesktop2Right::sname() noexcept
{
    return "switch_to_right_vdesktop" ;
}

void SwitchVDesktop2Right::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(first_call) KeybrdEventer::pushup(VKC_LCTRL, VKC_LWIN, VKC_RIGHT) ;
}


//CreateNewVDesktop
const std::string CreateNewVDesktop::sname() noexcept
{
    return "create_new_vdesktop" ;
}
void CreateNewVDesktop::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(first_call) KeybrdEventer::pushup(VKC_LCTRL, VKC_LWIN, VKC_D) ;
}


//CloseCurrentVDesktop
const std::string CloseCurrentVDesktop::sname() noexcept
{
    return "close_current_vdesktop" ;
}
void CloseCurrentVDesktop::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(first_call) KeybrdEventer::pushup(VKC_LCTRL, VKC_LWIN, VKC_F4) ;
}


//TaskView
const std::string TaskView::sname() noexcept
{
    return "task_view" ;
}

void TaskView::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(first_call) KeybrdEventer::pushup(VKC_LWIN, VKC_TAB) ;
}
