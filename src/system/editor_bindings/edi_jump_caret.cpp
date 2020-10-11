#include "edi_jump_caret.hpp"

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "simpl_text_selecter.hpp"


//EdiJumpCaret2BOL
const std::string EdiJumpCaret2BOL::sname() noexcept
{
    return "edi_jump_caret_to_BOL" ;
}
void EdiJumpCaret2BOL::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(!first_call) {
        return ;
    }
    if(ModeManager::is_edi_visual()) {
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_HOME) ;
    }
    else {
        KeybrdEventer::pushup(VKC_HOME) ;
    }
}


//EdiJumpCaret2EOL
const std::string EdiJumpCaret2EOL::sname() noexcept
{
    return "edi_jump_caret_to_EOL" ;
}
void EdiJumpCaret2EOL::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(!first_call) {
        return ;
    }
    if(ModeManager::is_edi_visual()) {
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_END) ;
    }
    else {
        KeybrdEventer::pushup(VKC_END) ;
    }
}


//EdiJumpCaret2NLine_DfBOF
const std::string EdiNJumpCaret2Line_DfBOF::sname() noexcept
{
    return "edi_n_jump_caret_to_line_default_BOF" ;
}
void EdiNJumpCaret2Line_DfBOF::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(!first_call) {
        return ;
    }
    if(SimplTextSelecter::is_first_line_selection()) {
        SimplTextSelecter::select_line_EOL2BOL() ;
    }
    if(ModeManager::is_edi_visual()) {
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_LCTRL, VKC_HOME) ;
    }
    else {
        KeybrdEventer::pushup(VKC_LCTRL, VKC_HOME) ;
    }
}


//EdiJumpCaret2NLine_DfEOF
const std::string EdiNJumpCaret2Line_DfEOF::sname() noexcept
{
    return "edi_n_jump_caret_to_line_default_EOF" ;
}
void EdiNJumpCaret2Line_DfEOF::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger)
{
    if(!first_call) {
        return ;
    }
    if(ModeManager::is_edi_visual()) {
        if(SimplTextSelecter::is_first_line_selection()) {
            SimplTextSelecter::select_line_BOL2EOL() ;
        }
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_LCTRL, VKC_END) ;

        if(ModeManager::get_mode() != ModeManager::Mode::EdiLineVisual) {
            KeybrdEventer::pushup(VKC_LSHIFT, VKC_HOME) ;
        }
    }
    else {
        KeybrdEventer::pushup(VKC_LCTRL, VKC_END) ;
    }
}
