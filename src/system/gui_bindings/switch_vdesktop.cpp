#include "switch_vdesktop.hpp"
#include "keybrd_eventer.hpp"
#include "utility.hpp"

using namespace std ;

//SwitchVDesktop2Left
const string SwitchVDesktop2Left::sname() noexcept
{
    return "switch_to_left_vdesktop" ;
}

bool SwitchVDesktop2Left::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(!first_call) return true ;
    if(!KeybrdEventer::pushup(VKC_LCTRL, VKC_LWIN, VKC_LEFT)) {
        return false ;
    }
    return true ;
}


//SwitchVDesktop2Right
const string SwitchVDesktop2Right::sname() noexcept
{
    return "switch_to_right_vdesktop" ;
}

bool SwitchVDesktop2Right::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(!first_call) return true ;
    if(!KeybrdEventer::pushup(VKC_LCTRL, VKC_LWIN, VKC_RIGHT)) {
        return false ;
    }
    return true ;
}


//CreateNewVDesktop
const std::string CreateNewVDesktop::sname() noexcept
{
    return "create_new_vdesktop" ;
}
bool CreateNewVDesktop::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(!first_call) return false ;
    return KeybrdEventer::pushup(VKC_LCTRL, VKC_LWIN, VKC_D) ;
}


//CloseCurrentVDesktop
const std::string CloseCurrentVDesktop::sname() noexcept
{
    return "close_current_vdesktop" ;
}
bool CloseCurrentVDesktop::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_LCTRL, VKC_LWIN, VKC_F4) ;
}


//TaskView
const std::string TaskView::sname() noexcept
{
    return "task_view" ;
}

bool TaskView::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_LWIN, VKC_TAB) ;
}
