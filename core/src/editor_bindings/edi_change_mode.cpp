#include "edi_change_mode.hpp"

#include <iostream>
#include <windows.h>

#include "key_absorber.hpp"
#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "mouse_eventer.hpp"
#include "options/virtual_cmd_line.hpp"
#include "simple_text_selecter.hpp"
#include "utility.hpp"


namespace vind
{
    //Change2EdiNormal
    const std::string Change2EdiNormal::sname() noexcept {
        return "change_to_edi_normal" ;
    }
    void Change2EdiNormal::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;

        if(!ModeManager::is_editor())
            MouseEventer::click(VKC_MOUSE_LEFT) ;

        using namespace ModeManager ;
        if(get_mode() == Mode::EdiNormal) return ;

        if(is_edi_visual())
            SimpleTextSelecter::unselect() ;

        KeyAbsorber::close_all_ports_with_refresh() ;
        KeyAbsorber::absorb() ;

        change_mode(Mode::EdiNormal) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI NORMAL --") ;
    }


    //Change2EdiInsert
    const std::string Change2EdiInsert::sname() noexcept {
        return "change_to_edi_insert" ;
    }
    void Change2EdiInsert::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace ModeManager ;

        if(!first_call) return ;
        KeyAbsorber::close_all_ports() ;
        KeyAbsorber::unabsorb() ;
        change_mode(Mode::EdiInsert) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI INSERT --") ;
    }


    //Change2EdiBOLInsert
    const std::string Change2EdiBOLInsert::sname() noexcept {
        return "change_to_edi_BOLinsert" ;
    }
    void Change2EdiBOLInsert::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;
        KeybrdEventer::pushup(VKC_HOME) ;
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr, vclmodeout) ;
    }


    //Change2EdiBkInsert
    const std::string Change2EdiBkInsert::sname() noexcept {
        return "change_to_edi_bkinsert" ;
    }
    void Change2EdiBkInsert::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;
        KeybrdEventer::pushup(VKC_RIGHT) ;
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr, vclmodeout) ;
    }


    //Change2EdiEOLInsert
    const std::string Change2EdiEOLInsert::sname() noexcept {
        return "change_to_edi_EOLinsert" ;
    }
    void Change2EdiEOLInsert::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;
        KeybrdEventer::pushup(VKC_END) ;
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr, vclmodeout) ;
    }


    //Change2EdiNlInsertBelow
    const std::string Change2EdiNlInsertBelow::sname() noexcept {
        return "change_to_edi_nlinsert_below" ;
    }
    void Change2EdiNlInsertBelow::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;
        KeybrdEventer::pushup(VKC_END) ;
        KeybrdEventer::pushup(VKC_ENTER) ;
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr, vclmodeout) ;
    }


    //Change2EdiNlInsertAbove
    const std::string Change2EdiNlInsertAbove::sname() noexcept {
        return "change_to_edi_nlinsert_above" ;
    }
    void Change2EdiNlInsertAbove::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;
        KeybrdEventer::pushup(VKC_HOME) ;
        KeybrdEventer::pushup(VKC_ENTER) ;
        KeybrdEventer::pushup(VKC_UP) ;
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr, vclmodeout) ;
    }


    //Change2EdiVisual
    const std::string Change2EdiVisual::sname() noexcept {
        return "change_to_edi_visual" ;
    }
    void Change2EdiVisual::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace ModeManager ;
        if(!first_call) return ;
        SimpleTextSelecter::select_words() ;
        change_mode(Mode::EdiVisual) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI VISUAL --") ;
    }


    //Change2EdiLineVisual
    const std::string Change2EdiLineVisual::sname() noexcept {
        return "change_to_edi_line_visual" ;
    }
    void Change2EdiLineVisual::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace ModeManager ;
        if(!first_call) return ;
        SimpleTextSelecter::select_line_EOL2BOL() ;
        change_mode(Mode::EdiLineVisual) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI VISUAL LINE--") ;
    }
}
