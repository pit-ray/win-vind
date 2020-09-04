#include "edi_jump_caret.hpp"

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "simpl_text_selecter.hpp"


//EdiJumpCaret2BOL
const std::string EdiJumpCaret2BOL::sname() noexcept
{
    return "edi_jump_caret_to_BOL" ;
}
bool EdiJumpCaret2BOL::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(ModeManager::is_edi_visual()) {
        return KeybrdEventer::pressup(VKC_LSHIFT, VKC_HOME) ;
    }
    return KeybrdEventer::pressup(VKC_HOME) ;
}


//EdiJumpCaret2EOL
const std::string EdiJumpCaret2EOL::sname() noexcept
{
    return "edi_jump_caret_to_EOL" ;
}
bool EdiJumpCaret2EOL::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(ModeManager::is_edi_visual()) {
        return KeybrdEventer::pressup(VKC_LSHIFT, VKC_END) ;
    }
    return KeybrdEventer::pressup(VKC_END) ;
}


/*
<Schedule>
- implement N Line System





*/


//EdiJumpCaret2NLine_DfBOF
const std::string EdiNJumpCaret2Line_DfBOF::sname() noexcept
{
    return "edi_n_jump_caret_to_line_default_BOF" ;
}
bool EdiNJumpCaret2Line_DfBOF::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(SimplTextSelecter::is_first_line_selection()) {
        if(!SimplTextSelecter::select_line_EOL2BOL()) {
            return false ;
        }
    }
    if(ModeManager::is_edi_visual()) {
        return KeybrdEventer::pressup(VKC_LSHIFT, VKC_LCTRL, VKC_HOME) ;
    }
    return KeybrdEventer::pressup(VKC_LCTRL, VKC_HOME) ;
}


//EdiJumpCaret2NLine_DfEOF
const std::string EdiNJumpCaret2Line_DfEOF::sname() noexcept
{
    return "edi_n_jump_caret_to_line_default_EOF" ;
}
bool EdiNJumpCaret2Line_DfEOF::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(ModeManager::is_edi_visual()) {
        if(SimplTextSelecter::is_first_line_selection()) {
            if(!SimplTextSelecter::select_line_BOL2EOL()) {
                return false ;
            }
        }
        if(!KeybrdEventer::pressup(VKC_LSHIFT, VKC_LCTRL, VKC_END)) {
            return false ;
        }
        if(ModeManager::get_mode() != ModeManager::Mode::EdiLineVisual) {
            if(!KeybrdEventer::pressup(VKC_LSHIFT, VKC_HOME)) {
                return false ;
            }
        }
        return true ;
    }
    return KeybrdEventer::pressup(VKC_LCTRL, VKC_END) ;
}