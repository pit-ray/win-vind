#include "bind/hotkey/pager.hpp"
#include "bind/safe_repeater.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

#include "key/ntype_logger.hpp"

namespace vind
{
    //GotoNextPage
    GotoNextPage::GotoNextPage()
    : BindedFuncCreator("goto_next_page")
    {}
    void GotoNextPage::sprocess(unsigned int repeat_num) {
        repeater::safe_for(repeat_num, [] {
            keybrd::pushup(KEYCODE_LALT, KEYCODE_RIGHT) ;
        }) ;
    }
    void GotoNextPage::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void GotoNextPage::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //GotoPrevPage
    GotoPrevPage::GotoPrevPage()
    : BindedFuncCreator("goto_prev_page")
    {}
    void GotoPrevPage::sprocess(unsigned int repeat_num) {
        repeater::safe_for(repeat_num, [] {
            keybrd::pushup(KEYCODE_LALT, KEYCODE_LEFT) ;
        }) ;
    }
    void GotoPrevPage::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void GotoPrevPage::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }
}
