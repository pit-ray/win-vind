#include "pager.hpp"

#include "bind/saferepeat.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "util/def.hpp"


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
                core::InputGate::get_instance().pushup(
                        KEYCODE_LALT, KEYCODE_RIGHT) ;
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
                core::InputGate::get_instance().pushup(
                        KEYCODE_LALT, KEYCODE_LEFT) ;
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
