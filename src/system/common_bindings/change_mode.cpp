#include "change_mode.hpp"

#include <iostream>

#include <windows.h>

#include "key_absorber.hpp"
#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "mouse_eventer.hpp"
#include "msg_logger.hpp"
#include "options/virtual_cmd_line.hpp"
#include "simpl_text_selecter.hpp"
#include "text_analyzer.hpp"
#include "virtual_key_fwd.hpp"
#include "utility.hpp"

using namespace std ;

using namespace ModeManager ;

//Change2Normal
const string Change2Normal::sname() noexcept
{
    return "change_to_normal" ;
}

void Change2Normal::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger),  const bool vclmodeout)
{
    if(!first_call) return ;

    const auto m = get_mode() ;
    if(m == Mode::Normal) return ;

    if(m == Mode::Visual) {
        MouseEventer::click(VKC_MOUSE_LEFT) ;
    }
    else if(is_edi_visual()) {
        SimplTextSelecter::unselect() ;
    }

    //When this function is called, binded key is down.
    //Thus, its key is needed to be up before absorbing key.
    KeyAbsorber::close_with_refresh() ;

    change_mode(Mode::Normal) ;
    if(vclmodeout) VirtualCmdLine::msgout("-- GUI NORMAL --") ;
}


//Change2Insert
const string Change2Insert::sname() noexcept
{
    return "change_to_insert" ;
}

void Change2Insert::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger, const bool vclmodeout)
{
    if(!first_call) return ;

    if(ModeManager::get_mode() ==ModeManager::Mode::Normal) {
        MouseEventer::click(VKC_MOUSE_LEFT) ;
    }

    KeyAbsorber::open() ;
    change_mode(Mode::Insert) ;
    if(vclmodeout) VirtualCmdLine::msgout("-- GUI INSERT --") ;
}


//Change2Visual
const string Change2Visual::sname() noexcept
{
    return "change_to_visual" ;
}

void Change2Visual::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger, const bool vclmodeout)
{
    if(!first_call) return ;
    change_mode(Mode::Visual) ;
    if(vclmodeout) VirtualCmdLine::msgout("-- GUI VISUAL --") ;
    MouseEventer::press(VKC_MOUSE_LEFT) ;
}


//Change2Editor
const string Change2Editor::sname() noexcept
{
    return "change_to_editor" ;
}

void Change2Editor::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger), const bool vclmodeout)
{
    if(!first_call) return ;
    MouseEventer::click(VKC_MOUSE_LEFT) ;
    KeyAbsorber::close_with_refresh() ;
    change_mode(Mode::EdiNormal) ;
    if(vclmodeout) VirtualCmdLine::msgout("-- EDI NORMAL --") ;
}
