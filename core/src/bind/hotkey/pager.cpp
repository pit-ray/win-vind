#include "pager.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

namespace vind
{
    //Move2NextPage
    const std::string Move2NextPage::sname() noexcept {
        return "move_to_next_page" ;
    }

    void Move2NextPage::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++)
                keybrd::pushup(KEYCODE_LALT, KEYCODE_RIGHT) ;
        }
    }


    //Move2PrevPage
    const std::string Move2PrevPage::sname() noexcept {
        return "move_to_prev_page" ;
    }

    void Move2PrevPage::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++)
                keybrd::pushup(KEYCODE_LALT, KEYCODE_LEFT) ;
        }
    }
}
