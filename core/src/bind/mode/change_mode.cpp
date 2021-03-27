#include "change_mode.hpp"

#include <iostream>

#include <windows.h>

#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "mode.hpp"
#include "err_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "text/simple_text_selecter.hpp"
#include "text/text_analyzer.hpp"
#include "util/def.hpp"


namespace vind
{
    //Change2Normal
    const std::string Change2Normal::sname() noexcept {
        return "change_to_normal" ;
    }

    void Change2Normal::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace mode ;
        if(!first_call) return ;

        const auto m = get_mode() ;
        if(m == Mode::Normal) return ;

        if(m == Mode::Visual) 
            mouse::click(KEYCODE_MOUSE_LEFT) ; //release holding mouse
        else if(is_edi_visual())
            textselect::unselect() ; //release shifting

        //When this function is called, binded key is down.
        //Thus, its key is needed to be up before absorbing key.
        keyabsorber::close_all_ports_with_refresh() ;
        keyabsorber::absorb() ;

        change_mode(Mode::Normal) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- GUI NORMAL --") ;
    }


    //Change2Insert
    const std::string Change2Insert::sname() noexcept {
        return "change_to_insert" ;
    }

    void Change2Insert::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace mode ;
        if(!first_call) return ;

        if(mode::get_mode() == mode::Mode::Normal)
            mouse::click(KEYCODE_MOUSE_LEFT) ;

        keyabsorber::close_all_ports() ;
        keyabsorber::unabsorb() ;
        change_mode(Mode::Insert) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- GUI INSERT --") ;
    }


    //Change2Visual
    const std::string Change2Visual::sname() noexcept {
        return "change_to_visual" ;
    }

    void Change2Visual::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace mode ;
        if(!first_call) return ;
        change_mode(Mode::Visual) ;
        if(vclmodeout) VirtualCmdLine::msgout("-- GUI VISUAL --") ;
        mouse::press(KEYCODE_MOUSE_LEFT) ;
    }

    //MyConfigWindowNormal
    const std::string MyConfigWindowNormal::sname() noexcept {
        return "my_config_window_normal" ;
    }

    void MyConfigWindowNormal::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        using namespace mode ;
        if(!first_call) return ;

        keyabsorber::close_all_ports_with_refresh() ;
        keyabsorber::absorb() ;
        change_mode(Mode::MyConfigWindowNormal) ;
    }

    //MyConfigWindowInsert
    const std::string MyConfigWindowInsert::sname() noexcept {
        return "my_config_window_insert" ;
    }

    void MyConfigWindowInsert::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        using namespace mode ;
        if(!first_call) return ;

        keyabsorber::close_all_ports_with_refresh() ;
        keyabsorber::unabsorb() ;
        change_mode(Mode::MyConfigWindowInsert) ;
    }
}
