#include "change_mode.hpp"

#include <iostream>

#include <windows.h>

#include "io/keybrd_eventer.hpp"
#include "io/mouse_eventer.hpp"
#include "key/key_absorber.hpp"
#include "key/virtual_key_fwd.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "text/simple_text_selecter.hpp"
#include "text/text_analyzer.hpp"
#include "utility.hpp"


namespace vind
{
    //Change2Normal
    const std::string Change2Normal::sname() noexcept {
        return "change_to_normal" ;
    }

    void Change2Normal::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace mode ;
        if(!first_call) return ;

        const auto m = get_mode() ;
        if(m == Mode::Normal) return ;

        if(m == Mode::Visual) 
            mouse::click(VKC_MOUSE_LEFT) ; //release holding mouse
        else if(is_edi_visual())
            textselect::unselect() ; //release shifting

        //When this function is called, binded key is down.
        //Thus, its key is needed to be up before absorbing key.
        keyabsorb::close_all_ports_with_refresh() ;
        keyabsorb::absorb() ;

        change_mode(Mode::Normal) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- GUI NORMAL --") ;
    }


    //Change2Insert
    const std::string Change2Insert::sname() noexcept {
        return "change_to_insert" ;
    }

    void Change2Insert::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace mode ;
        if(!first_call) return ;

        if(mode::get_mode() == mode::Mode::Normal)
            mouse::click(VKC_MOUSE_LEFT) ;

        keyabsorb::close_all_ports() ;
        keyabsorb::unabsorb() ;
        change_mode(Mode::Insert) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- GUI INSERT --") ;
    }


    //Change2Visual
    const std::string Change2Visual::sname() noexcept {
        return "change_to_visual" ;
    }

    void Change2Visual::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const bool vclmodeout) {
        using namespace mode ;
        if(!first_call) return ;
        change_mode(Mode::Visual) ;
        if(vclmodeout) VirtualCmdLine::msgout("-- GUI VISUAL --") ;
        mouse::press(VKC_MOUSE_LEFT) ;
    }

    //MyConfigWindowNormal
    const std::string MyConfigWindowNormal::sname() noexcept {
        return "my_config_window_normal" ;
    }

    void MyConfigWindowNormal::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        using namespace mode ;
        if(!first_call) return ;

        keyabsorb::close_all_ports_with_refresh() ;
        keyabsorb::absorb() ;
        change_mode(Mode::MyConfigWindowNormal) ;
    }

    //MyConfigWindowInsert
    const std::string MyConfigWindowInsert::sname() noexcept {
        return "my_config_window_insert" ;
    }

    void MyConfigWindowInsert::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        using namespace mode ;
        if(!first_call) return ;

        keyabsorb::close_all_ports_with_refresh() ;
        keyabsorb::unabsorb() ;
        change_mode(Mode::MyConfigWindowInsert) ;
    }
}
