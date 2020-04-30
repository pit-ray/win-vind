#include "window_ctrl.hpp"
#include "keybrd_eventer.hpp"
#include "interval_timer.hpp"
#include "key_logger.hpp"

#include "move_cursor.hpp"

#include <iostream>


using namespace std ;

//CloseCurrentWindow
const string CloseCurrentWindow::sname() noexcept
{
    return "close_current_window" ;
}

bool CloseCurrentWindow::common_process()
{
    return KeybrdEventer::is_pushup(VKC_LALT, VKC_F4) ;
}

bool CloseCurrentWindow::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool CloseCurrentWindow::sprocess(const string& cmd)
{
    return common_process() ;
}


//SwitchWindow
const string SwitchWindow::sname() noexcept
{
    return "switch_window" ;
}

bool SwitchWindow::common_process()
{
    using namespace KeybrdEventer ;

    //if funcs is not initialized, these funcs is always uncallable.
    //and, I do not want to use process() of func, but refer key map.
    const auto left_pbf = MoveLeft::create_with_cache() ;
    const auto right_pbf = MoveRight::create_with_cache() ;

    auto palt = make_unique<SmartKey>(VKC_LALT) ;
    auto ptab = make_unique<SmartKey>(VKC_TAB) ;

    if(!palt->is_push()) {
        return false ;
    }

    if(!ptab->is_push()) {
        return false ;
    }
    ptab.reset() ; // release key

    const auto sleep = [] {Sleep(5) ;} ;

    MSG msg ;
    KeyLogger logger{} ;
    while(true) {
        //MessageRoop
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg) ;

            DispatchMessage(&msg) ;
        }

        //check system keys
        if(KeyAbsorber::is_down(VKC_ESC)) {
            return is_release_keystate(VKC_ESC) ;
        }
        if(KeyAbsorber::is_down(VKC_ENTER)) {
            return is_release_keystate(VKC_ENTER) ;
        }

        //is changed ?
        if(!logger.is_changed_and_update()) {
            logger.remove_from_back(1) ;
            sleep() ;
            continue ;
        }

        //inputed keys include key map of MoveLeft or MoveRight.
        auto at_least_exist = false ;
        if(left_pbf->existed_num_and_update(logger.back(), logger.size() - 1)) {
            at_least_exist = true ;
        }
        if(right_pbf->existed_num_and_update(logger.back(), logger.size() - 1)) {
            at_least_exist = true ;
        }
        if(!at_least_exist) { 
            logger.clear() ;
            sleep() ;
            continue ;
        }

        //is callable?
        if(left_pbf->is_callable()) {
            if(KeyAbsorber::is_down(VKC_LEFT)) {
                if(!is_release_keystate(VKC_LEFT)) {
                    return false ;
                }
                if(!is_pushup(VKC_LEFT)) {
                    return false ;
                }
                //undo
                if(!is_push_keystate(VKC_LEFT)) {
                    return false ;
                }
            }
            else {
                if(!is_pushup(VKC_LEFT)) {
                    return false ;
                }
            }
            logger.clear() ;
        }

        if(right_pbf->is_callable()) {
            if(KeyAbsorber::is_down(VKC_RIGHT)) {
                if(!is_release_keystate(VKC_RIGHT)) {
                    return false ;
                }
                if(!is_pushup(VKC_RIGHT)) {
                    return false ;
                }
                //undo
                if(!is_push_keystate(VKC_RIGHT)) {
                    return false ;
                }
            }
            else {
                if(!is_pushup(VKC_RIGHT)) {
                    return false ;
                }
            }
            logger.clear() ;
        }

        sleep() ;
    }

    return true ;
}

bool SwitchWindow::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool SwitchWindow::sprocess(const string& cmd)
{
    if(!KeybrdEventer::is_release_keystate(VKC_ENTER)) {
        return false ;
    }
    return common_process() ;
}


//MaximizeCurrentWindow
const string MaximizeCurrentWindow::sname() noexcept
{
    return "maximize_current_window" ;
}

bool MaximizeCurrentWindow::common_process()
{
    return KeybrdEventer::is_pushup(VKC_LWIN, VKC_UP) ;
}

bool MaximizeCurrentWindow::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool MaximizeCurrentWindow::sprocess(const string& cmd)
{
    return common_process() ;
}


//MinimizeCurrentWindow
const string MinimizeCurrentWindow::sname() noexcept
{
    return "minimize_current_window" ;
}

bool MinimizeCurrentWindow::common_process()
{
    return KeybrdEventer::is_pushup(VKC_LWIN, VKC_DOWN) ;
}

bool MinimizeCurrentWindow::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool MinimizeCurrentWindow::sprocess(const string& cmd)
{
    return common_process() ;
}


//SnapCurrentWindow2Left
const string SnapCurrentWindow2Left::sname() noexcept
{
    return "snap_current_window_to_left" ;
}

bool SnapCurrentWindow2Left::common_process()
{
    return KeybrdEventer::is_pushup(VKC_LWIN, VKC_LEFT) ;
}

bool SnapCurrentWindow2Left::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool SnapCurrentWindow2Left::sprocess(const string& cmd)
{
    return common_process() ;
}


//SnapCurrentWindow2Right
const string SnapCurrentWindow2Right::sname() noexcept
{
    return "snap_current_window_to_right" ;
}

bool SnapCurrentWindow2Right::common_process()
{
    return KeybrdEventer::is_pushup(VKC_LWIN, VKC_RIGHT) ;
}

bool SnapCurrentWindow2Right::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool SnapCurrentWindow2Right::sprocess(const string& cmd)
{
    return common_process() ;
}