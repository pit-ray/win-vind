#include "pager.hpp"

#include "bind/safe_repeater.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"

namespace vind
{
    namespace bind
    {
        //GotoNextPage
        GotoNextPage::GotoNextPage()
        : BindedFuncVoid("goto_next_page")
        {}
        void GotoNextPage::sprocess(unsigned int repeat_num) {
            safe_for(repeat_num, [] {
                util::pushup(KEYCODE_LALT, KEYCODE_RIGHT) ;
            }) ;
        }
        void GotoNextPage::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void GotoNextPage::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //GotoPrevPage
        GotoPrevPage::GotoPrevPage()
        : BindedFuncVoid("goto_prev_page")
        {}
        void GotoPrevPage::sprocess(unsigned int repeat_num) {
            safe_for(repeat_num, [] {
                util::pushup(KEYCODE_LALT, KEYCODE_LEFT) ;
            }) ;
        }
        void GotoPrevPage::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void GotoPrevPage::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }
    }
}
