#include "edi_change_mode.hpp"

#include <iostream>
#include <windows.h>

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "key_absorber.hpp"
#include "alternative_text_selecter.hpp"


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
        if(!AlternativeTextSelecter::is_unselect()) {
            return false ;
        }
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
    using namespace ModeManager ;

    if(!first_call) {
        return true ;
    }
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
    using namespace ModeManager ;

    if(!first_call) {
        return true ;
    }

    using namespace KeybrdEventer ;
    if(!is_pushup(VKC_RIGHT)) {
        return false ;
    }

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
    using namespace ModeManager ;

    if(!first_call) {
        return true ;
    }

    using namespace KeybrdEventer ;
    if(!is_pushup(VKC_END)) {
        return false ;
    }

    if(!is_pushup(VKC_ENTER)) {
        return false ;
    }

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
    using namespace ModeManager ;

    if(!first_call) {
        return true ;
    }

    if(!AlternativeTextSelecter::is_select_words()) {
        return false ;
    }

    change_mode(Mode::EdiVisual) ;
    return true ;
}


//Change2EdiLineVisual
const string Change2EdiLineVisual::sname() noexcept
{
    return "change_to_edi_line_visual" ;
}
bool Change2EdiLineVisual::sprocess(const bool first_call)
{
    using namespace ModeManager ;

    if(!first_call) {
        return true ;
    }

    if(!AlternativeTextSelecter::is_select_line_EOL2BOL()) {
        return false ;
    }

    change_mode(Mode::EdiLineVisual) ;
    return true ;
}