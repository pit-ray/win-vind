#include "bind/emu/edi_change_mode.hpp"

#include <iostream>
#include <windows.h>

#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "key/key_absorber.hpp"
#include "bind/base/mode.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "text/simple_text_selecter.hpp"
#include "util/def.hpp"


namespace vind
{
    //Change2EdiNormal
    const std::string Change2EdiNormal::sname() noexcept {
        return "change_to_edi_normal" ;
    }
    void Change2EdiNormal::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;

        if(!mode::is_editor())
            mouse::click(KEYCODE_MOUSE_LEFT) ;

        using namespace mode ;
        if(get_global_mode() == Mode::EdiNormal) return ;

        if(is_edi_visual())
            textselect::unselect() ;

        keyabsorber::close_all_ports_with_refresh() ;
        keyabsorber::absorb() ;

        change_mode(Mode::EdiNormal) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI NORMAL --") ;
    }


    //Change2EdiInsert
    const std::string Change2EdiInsert::sname() noexcept {
        return "change_to_edi_insert" ;
    }
    void Change2EdiInsert::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace mode ;

        if(!first_call) return ;
        keyabsorber::close_all_ports() ;
        keyabsorber::unabsorb() ;
        change_mode(Mode::EdiInsert) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI INSERT --") ;
    }


    //Change2EdiBOLInsert
    const std::string Change2EdiBOLInsert::sname() noexcept {
        return "change_to_edi_BOLinsert" ;
    }
    void Change2EdiBOLInsert::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;
        keybrd::pushup(KEYCODE_HOME) ;
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr, vclmodeout) ;
    }


    //Change2EdiBkInsert
    const std::string Change2EdiBkInsert::sname() noexcept {
        return "change_to_edi_bkinsert" ;
    }
    void Change2EdiBkInsert::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;
        keybrd::pushup(KEYCODE_RIGHT) ;
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr, vclmodeout) ;
    }


    //Change2EdiEOLInsert
    const std::string Change2EdiEOLInsert::sname() noexcept {
        return "change_to_edi_EOLinsert" ;
    }
    void Change2EdiEOLInsert::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;
        keybrd::pushup(KEYCODE_END) ;
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr, vclmodeout) ;
    }


    //Change2EdiNlInsertBelow
    const std::string Change2EdiNlInsertBelow::sname() noexcept {
        return "change_to_edi_nlinsert_below" ;
    }
    void Change2EdiNlInsertBelow::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;
        keybrd::pushup(KEYCODE_END) ;
        keybrd::pushup(KEYCODE_ENTER) ;
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr, vclmodeout) ;
    }


    //Change2EdiNlInsertAbove
    const std::string Change2EdiNlInsertAbove::sname() noexcept {
        return "change_to_edi_nlinsert_above" ;
    }
    void Change2EdiNlInsertAbove::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        if(!first_call) return ;
        keybrd::pushup(KEYCODE_HOME) ;
        keybrd::pushup(KEYCODE_ENTER) ;
        keybrd::pushup(KEYCODE_UP) ;
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr, vclmodeout) ;
    }


    //Change2EdiVisual
    const std::string Change2EdiVisual::sname() noexcept {
        return "change_to_edi_visual" ;
    }
    void Change2EdiVisual::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace mode ;
        if(!first_call) return ;
        textselect::select_words() ;
        change_mode(Mode::EdiVisual) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI VISUAL --") ;
    }


    //Change2EdiLineVisual
    const std::string Change2EdiLineVisual::sname() noexcept {
        return "change_to_edi_line_visual" ;
    }
    void Change2EdiLineVisual::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace mode ;
        if(!first_call) return ;
        textselect::select_line_EOL2BOL() ;
        change_mode(Mode::EdiLineVisual) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI VISUAL LINE--") ;
    }
}
