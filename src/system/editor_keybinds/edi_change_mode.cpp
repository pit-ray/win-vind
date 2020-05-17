#include "edi_change_mode.hpp"
#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "key_absorber.hpp"

#include <iostream>
#include <windows.h>

using namespace std ;
using namespace ModeManager ;


//Change2EdiNormal
const string Change2EdiNormal::sname() noexcept
{
    return "change_to_edi_normal" ;
}

bool Change2EdiNormal::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(get_mode() == Mode::EdiVisual) {
        const auto hwnd = GetForegroundWindow() ;
        //cancel selecting
        SendMessage(hwnd, EM_SETSEL, -1, 0) ;
    }

    if(!KeyAbsorber::is_close_with_refresh()) {
        return false ;
    }

    change_mode(Mode::EdiNormal) ;
    return true ;
}


//Change2EdiInsert
const string Change2EdiInsert::sname() noexcept
{
    return "change_to_edi_insert" ;
}

bool Change2EdiInsert::sprocess(const bool first_call)
{
    cout << "change2insert\n" ;
    if(!first_call) return true ;
    KeyAbsorber::open() ;
    change_mode(Mode::EdiInsert) ;
    return true ;
}


//Change2EdiBkInsert
const string Change2EdiBkInsert::sname() noexcept
{
    return "change_to_edi_bkinsert" ;
}

bool Change2EdiBkInsert::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    KeyAbsorber::open() ;
    change_mode(Mode::EdiInsert) ;
    return true ;
}


//Change2EdiNlInsert
const string Change2EdiNlInsert::sname() noexcept
{
    return "change_to_edi_nlinsert" ;
}

bool Change2EdiNlInsert::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    KeyAbsorber::open() ;
    change_mode(Mode::EdiInsert) ;
    return true ;
}


//Change2EdiVisual
const string Change2EdiVisual::sname() noexcept
{
    return "change_to_edi_visual" ;
}

bool Change2EdiVisual::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    change_mode(Mode::EdiVisual) ;
    return true ;
}