#include "edi_change_mode.hpp"

#include <iostream>
#include <windows.h>

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "key_absorber.hpp"
#include "simpl_text_selecter.hpp"
#include "options/virtual_cmd_line.hpp"


using namespace std ;

//Change2EdiNormal
const string Change2EdiNormal::sname() noexcept
{
    return "change_to_edi_normal" ;
}
bool Change2EdiNormal::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }

    using namespace ModeManager ;
    if(ModeManager::get_mode() == Mode::EdiNormal) {
        return true ;
    }

    if(is_edi_visual()) {
        if(!SimplTextSelecter::unselect()) {
            return false ;
        }
    }

    if(!KeyAbsorber::close_with_refresh()) {
        return false ;
    }
    change_mode(Mode::EdiNormal) ;
    VirtualCmdLine::msgout("-- EDI NORMAL --") ;
    return true ;
}


//Change2EdiInsert
const string Change2EdiInsert::sname() noexcept
{
    return "change_to_edi_insert" ;
}
bool Change2EdiInsert::sprocess(const bool first_call)
{
    using namespace ModeManager ;

    if(!first_call) {
        return true ;
    }
    KeyAbsorber::open() ;
    change_mode(Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
    return true ;
}


//Change2EdiBOLInsert
const string Change2EdiBOLInsert::sname() noexcept
{
    return "change_to_edi_BOLinsert" ;
}
bool Change2EdiBOLInsert::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(!KeybrdEventer::pushup(VKC_HOME)) {
        return false ;
    }
    KeyAbsorber::open() ;
    ModeManager::change_mode(ModeManager::Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
    return true ;
}


//Change2EdiBkInsert
const string Change2EdiBkInsert::sname() noexcept
{
    return "change_to_edi_bkinsert" ;
}
bool Change2EdiBkInsert::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(!KeybrdEventer::pushup(VKC_RIGHT)) {
        return false ;
    }
    KeyAbsorber::open() ;
    ModeManager::change_mode(ModeManager::Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
    return true ;
}


//Change2EdiEOLInsert
const string Change2EdiEOLInsert::sname() noexcept
{
    return "change_to_edi_EOLinsert" ;
}
bool Change2EdiEOLInsert::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(!KeybrdEventer::pushup(VKC_END)) {
        return false ;
    }
    KeyAbsorber::open() ;
    ModeManager::change_mode(ModeManager::Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
    return true ;
}


//Change2EdiNlInsertBelow
const string Change2EdiNlInsertBelow::sname() noexcept
{
    return "change_to_edi_nlinsert_below" ;
}
bool Change2EdiNlInsertBelow::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(!KeybrdEventer::pushup(VKC_END)) {
        return false ;
    }
    if(!KeybrdEventer::pushup(VKC_ENTER)) {
        return false ;
    }
    KeyAbsorber::open() ;
    ModeManager::change_mode(ModeManager::Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
    return true ;
}


//Change2EdiNlInsertAbove
const string Change2EdiNlInsertAbove::sname() noexcept
{
    return "change_to_edi_nlinsert_above" ;
}
bool Change2EdiNlInsertAbove::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(!KeybrdEventer::pushup(VKC_HOME)) {
        return false ;
    }
    if(!KeybrdEventer::pushup(VKC_ENTER)) {
        return false ;
    }
    if(!KeybrdEventer::pushup(VKC_UP)) {
        return false ;
    }
    KeyAbsorber::open() ;
    ModeManager::change_mode(ModeManager::Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
    return true ;
}


//Change2EdiVisual
const string Change2EdiVisual::sname() noexcept
{
    return "change_to_edi_visual" ;
}
bool Change2EdiVisual::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(!SimplTextSelecter::select_words()) {
        return false ;
    }
    ModeManager::change_mode(ModeManager::Mode::EdiVisual) ;
    VirtualCmdLine::msgout("-- EDI VISUAL --") ;
    return true ;
}


//Change2EdiLineVisual
const string Change2EdiLineVisual::sname() noexcept
{
    return "change_to_edi_line_visual" ;
}
bool Change2EdiLineVisual::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(!SimplTextSelecter::select_line_EOL2BOL()) {
        return false ;
    }
    ModeManager::change_mode(ModeManager::Mode::EdiLineVisual) ;
    VirtualCmdLine::msgout("-- EDI VISUAL LINE--") ;
    return true ;
}