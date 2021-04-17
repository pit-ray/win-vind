#include "bind/hotkey/pager.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

#include "bind/base/ntype_logger.hpp"

namespace vind
{
    //Move2NextPage
    Move2NextPage::Move2NextPage()
    : BindedFuncCreator("move_to_next_page")
    {}
    void Move2NextPage::sprocess(unsigned int repeat_num) {
        for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_LALT, KEYCODE_RIGHT) ;
        }
    }
    void Move2NextPage::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void Move2NextPage::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //Move2PrevPage
    Move2PrevPage::Move2PrevPage()
    : BindedFuncCreator("move_to_prev_page")
    {}
    void Move2PrevPage::sprocess(unsigned int repeat_num) {
        for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_LALT, KEYCODE_LEFT) ;
        }
    }
    void Move2PrevPage::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void Move2PrevPage::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }
}
