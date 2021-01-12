#include "edi_jump_caret.hpp"

#include "key_logger.hpp"
#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "simple_text_selecter.hpp"
#include "utility.hpp"


//EdiJumpCaret2BOL
const std::string EdiJumpCaret2BOL::sname() noexcept
{
    return "edi_jump_caret_to_BOL" ;
}
bool EdiJumpCaret2BOL::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiJumpCaret2BOL::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    if(ModeManager::is_edi_visual())
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_HOME) ;
    else
        KeybrdEventer::pushup(VKC_HOME) ;
}


//EdiJumpCaret2EOL
const std::string EdiJumpCaret2EOL::sname() noexcept
{
    return "edi_jump_caret_to_EOL" ;
}
bool EdiJumpCaret2EOL::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiJumpCaret2EOL::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    //down caret N - 1
    for(unsigned int i = 1 ; i < repeat_num ; i ++)
        KeybrdEventer::pushup(VKC_DOWN) ;

    if(ModeManager::is_edi_visual()) {
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_END) ;
    }
    else {
        KeybrdEventer::pushup(VKC_END) ;
        KeybrdEventer::pushup(VKC_LEFT) ;
    }
}


//EdiJumpCaret2NLine_DfBOF
const std::string EdiNJumpCaret2Line_DfBOF::sname() noexcept
{
    return "edi_n_jump_caret_to_line_default_BOF" ;
}
bool EdiNJumpCaret2Line_DfBOF::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiNJumpCaret2Line_DfBOF::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const parent_charlgr)
{
    if(!first_call) return ;

    if(SimpleTextSelecter::is_first_line_selection())
        SimpleTextSelecter::select_line_EOL2BOL() ;

    using KeybrdEventer::pushup ;

    auto rn = repeat_num ;
    if(parent_charlgr != nullptr) {
        auto str = KyLgr::log2str(*parent_charlgr) ;
        if(str.empty()) return ;
        rn = KyLgr::extract_from_str(str) ;
    }

    if(ModeManager::is_edi_visual()) {
        pushup(VKC_LSHIFT, VKC_LCTRL, VKC_HOME) ;

        //down caret N - 1
        for(unsigned int i = 1 ; i < rn ; i ++)
            pushup(VKC_LSHIFT, VKC_DOWN) ;
    }
    else {
        KeybrdEventer::pushup(VKC_LCTRL, VKC_HOME) ;

        //down caret N - 1
        for(unsigned int i = 1 ; i < rn ; i ++)
            pushup(VKC_DOWN) ;
    }
}


//EdiJumpCaret2NLine_DfEOF
const std::string EdiNJumpCaret2Line_DfEOF::sname() noexcept
{
    return "edi_n_jump_caret_to_line_default_EOF" ;
}
bool EdiNJumpCaret2Line_DfEOF::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiNJumpCaret2Line_DfEOF::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const parent_charlgr)
{
    if(!first_call) return ;

    using KeybrdEventer::pushup ;
    using namespace ModeManager ;

    if(parent_charlgr != nullptr) {
        EdiNJumpCaret2Line_DfBOF::sprocess(true, 1, nullptr, parent_charlgr) ;
        return ;
    }

    if(repeat_num == 1) {
        if(is_edi_visual()) {
            if(SimpleTextSelecter::is_first_line_selection())
                SimpleTextSelecter::select_line_BOL2EOL() ;

            pushup(VKC_LSHIFT, VKC_LCTRL, VKC_END) ;

            if(get_mode() != Mode::EdiLineVisual)
                pushup(VKC_LSHIFT, VKC_HOME) ;
        }
        else {
            KeybrdEventer::pushup(VKC_LCTRL, VKC_END) ;
        }
    }
    else {
        EdiNJumpCaret2Line_DfBOF::sprocess(true, repeat_num, nullptr, parent_charlgr) ;
    }
}
