#include "edi_change_mode.hpp"

#include <iostream>
#include <windows.h>

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "key_absorber.hpp"
#include "simpl_text_selecter.hpp"
#include "options/virtual_cmd_line.hpp"
#include "utility.hpp"


using namespace std ;

//Change2EdiNormal
const string Change2EdiNormal::sname() noexcept
{
    return "change_to_edi_normal" ;
}
void Change2EdiNormal::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) {
        return ;
    }

    using namespace ModeManager ;
    if(ModeManager::get_mode() == Mode::EdiNormal) {
        return ;
    }

    if(is_edi_visual()) {
        SimplTextSelecter::unselect() ;
    }

    KeyAbsorber::close_with_refresh() ;

    change_mode(Mode::EdiNormal) ;
    VirtualCmdLine::msgout("-- EDI NORMAL --") ;
}


//Change2EdiInsert
const string Change2EdiInsert::sname() noexcept
{
    return "change_to_edi_insert" ;
}
void Change2EdiInsert::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    using namespace ModeManager ;

    if(!first_call) {
        return ;
    }
    KeyAbsorber::open() ;
    change_mode(Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
}


//Change2EdiBOLInsert
const string Change2EdiBOLInsert::sname() noexcept
{
    return "change_to_edi_BOLinsert" ;
}
void Change2EdiBOLInsert::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) {
        return ;
    }
    KeybrdEventer::pushup(VKC_HOME) ;
    KeyAbsorber::open() ;
    ModeManager::change_mode(ModeManager::Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
}


//Change2EdiBkInsert
const string Change2EdiBkInsert::sname() noexcept
{
    return "change_to_edi_bkinsert" ;
}
void Change2EdiBkInsert::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) {
        return ;
    }
    KeybrdEventer::pushup(VKC_RIGHT) ;
    KeyAbsorber::open() ;
    ModeManager::change_mode(ModeManager::Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
}


//Change2EdiEOLInsert
const string Change2EdiEOLInsert::sname() noexcept
{
    return "change_to_edi_EOLinsert" ;
}
void Change2EdiEOLInsert::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) {
        return ;
    }
    KeybrdEventer::pushup(VKC_END) ;
    KeyAbsorber::open() ;
    ModeManager::change_mode(ModeManager::Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
}


//Change2EdiNlInsertBelow
const string Change2EdiNlInsertBelow::sname() noexcept
{
    return "change_to_edi_nlinsert_below" ;
}
void Change2EdiNlInsertBelow::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) {
        return ;
    }
    KeybrdEventer::pushup(VKC_END) ;
    KeybrdEventer::pushup(VKC_ENTER) ;
    KeyAbsorber::open() ;
    ModeManager::change_mode(ModeManager::Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
}


//Change2EdiNlInsertAbove
const string Change2EdiNlInsertAbove::sname() noexcept
{
    return "change_to_edi_nlinsert_above" ;
}
void Change2EdiNlInsertAbove::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) {
        return ;
    }
    KeybrdEventer::pushup(VKC_HOME) ;
    KeybrdEventer::pushup(VKC_ENTER) ;
    KeybrdEventer::pushup(VKC_UP) ;
    KeyAbsorber::open() ;
    ModeManager::change_mode(ModeManager::Mode::EdiInsert) ;
    VirtualCmdLine::msgout("-- EDI INSERT --") ;
}


//Change2EdiVisual
const string Change2EdiVisual::sname() noexcept
{
    return "change_to_edi_visual" ;
}
void Change2EdiVisual::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) {
        return ;
    }
    SimplTextSelecter::select_words() ;
    ModeManager::change_mode(ModeManager::Mode::EdiVisual) ;
    VirtualCmdLine::msgout("-- EDI VISUAL --") ;
}


//Change2EdiLineVisual
const string Change2EdiLineVisual::sname() noexcept
{
    return "change_to_edi_line_visual" ;
}
void Change2EdiLineVisual::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) {
        return ;
    }
    SimplTextSelecter::select_line_EOL2BOL() ;
    ModeManager::change_mode(ModeManager::Mode::EdiLineVisual) ;
    VirtualCmdLine::msgout("-- EDI VISUAL LINE--") ;
}
