#include "change_mode.hpp"
#include "mouse_eventer.hpp"
#include "key_absorber.hpp"
#include "text_analyzer.hpp"
#include "msg_logger.hpp"
#include "system.hpp"
#include "keybrd_eventer.hpp"
#include "virtual_key_fwd.hpp"
#include <windows.h>

#include <iostream>
using namespace std ;

using namespace System ;

//Change2Normal
const string Change2Normal::sname() noexcept
{
    return "change_to_normal" ;
}

bool Change2Normal::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    if(get_mode() == Mode::Visual) {
        if(!MouseEventer::is_click(MouseEventer::Button::LEFT)) {
            return false ;
        }
    }

    KeyAbsorber::close() ;
    System::change_mode(System::Mode::Normal) ;
    return true ;
}


//Change2Insert
const string Change2Insert::sname() noexcept
{
    return "change_to_insert" ;
}

bool Change2Insert::sprocess(const bool first_call)
{
    using namespace MouseEventer ;
    if(!first_call) return true ;
    if(!is_click(Button::LEFT)) {
        return false ;
    }
    KeyAbsorber::open() ;
    change_mode(Mode::Insert) ;
    return true ;
}


//Change2Visual
const string Change2Visual::sname() noexcept
{
    return "change_to_visual" ;
}

bool Change2Visual::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    change_mode(Mode::Visual) ;
    return MouseEventer::is_down(MouseEventer::Button::LEFT) ;
}


//Change2Editor
const string Change2Editor::sname() noexcept
{
    return "change_to_editor" ;
}

bool Change2Editor::sprocess(const bool first_call)
{
    using namespace MouseEventer ;
    if(!first_call) return true ;
    if(!is_click(Button::LEFT)) {
        return false ;
    }
    change_mode(Mode::EdiNormal) ;

    TextAnalyzer::is_update() ;
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
    System::change_mode(System::Mode::Command) ;
    return true ;
}