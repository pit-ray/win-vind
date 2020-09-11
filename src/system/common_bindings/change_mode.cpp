#include "change_mode.hpp"

#include <iostream>

#include <windows.h>

#include "mouse_eventer.hpp"
#include "key_absorber.hpp"
#include "text_analyzer.hpp"
#include "msg_logger.hpp"
#include "mode_manager.hpp"
#include "keybrd_eventer.hpp"
#include "virtual_key_fwd.hpp"
#include "simpl_text_selecter.hpp"
#include "options/virtual_cmd_line.hpp"

using namespace std ;

using namespace ModeManager ;

//Change2Normal
const string Change2Normal::sname() noexcept
{
    return "change_to_normal" ;
}

bool Change2Normal::sprocess(const bool first_call, const bool vclmodeout)
{
    if(!first_call) return true ;

    const auto m = get_mode() ;
    if(m == Mode::Normal) {
        return true ;
    }
    if(m == Mode::Visual) {
        if(!MouseEventer::click(VKC_MOUSE_LEFT)) {
            return false ;
        }
    }
    else if(is_edi_visual()) {
        if(!SimplTextSelecter::unselect()) {
            return false ;
        }
    }

    //When this function is called, binded key is down.
    //Thus, its key is needed to be up before absorbing key.
    if(!KeyAbsorber::close_with_refresh()) {
        return false ;
    }
    change_mode(Mode::Normal) ;
    if(vclmodeout) VirtualCmdLine::msgout("-- GUI NORMAL --") ;
    return true ;
}


//Change2Insert
const string Change2Insert::sname() noexcept
{
    return "change_to_insert" ;
}

bool Change2Insert::sprocess(const bool first_call, const bool vclmodeout)
{
    if(!first_call) return true ;

    if(ModeManager::get_mode() ==ModeManager::Mode::Normal) {
        if(!MouseEventer::click(VKC_MOUSE_LEFT)) {
            return false ;
        }
    }

    KeyAbsorber::open() ;
    change_mode(Mode::Insert) ;
    if(vclmodeout) VirtualCmdLine::msgout("-- GUI INSERT --") ;
    return true ;
}


//Change2Visual
const string Change2Visual::sname() noexcept
{
    return "change_to_visual" ;
}

bool Change2Visual::sprocess(const bool first_call, const bool vclmodeout)
{
    if(!first_call) return true ;
    change_mode(Mode::Visual) ;
    if(vclmodeout) VirtualCmdLine::msgout("-- GUI VISUAL --") ;
    return MouseEventer::press(VKC_MOUSE_LEFT) ;
}


//Change2Editor
const string Change2Editor::sname() noexcept
{
    return "change_to_editor" ;
}

bool Change2Editor::sprocess(const bool first_call, const bool vclmodeout)
{
    if(!first_call) return true ;
    if(!MouseEventer::click(VKC_MOUSE_LEFT)) {
        return false ;
    }
    if(!KeyAbsorber::close_with_refresh()) {
        return false ;
    }
    change_mode(Mode::EdiNormal) ;
    if(vclmodeout) VirtualCmdLine::msgout("-- EDI NORMAL --") ;
    return true ;
}


//Change2Command
const string Change2Command::sname() noexcept
{
    return "change_to_command" ;
}

bool Change2Command::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    const auto mode = ModeManager::get_mode() ;
    VirtualCmdLine::reset() ;

    if(mode == Mode::Normal) {
        change_mode(Mode::Command) ;
        return true ;
    }
    if(mode == Mode::EdiNormal) {
        change_mode(Mode::EdiCommand) ;
        return true ;
    }

    change_mode(Mode::Normal) ;
    return true ;
}
