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
        void GotoNextPage::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LALT, KEYCODE_RIGHT) ;
            }) ;
        }

        //GotoPrevPage
        GotoPrevPage::GotoPrevPage()
        : BindedFuncVoid("goto_prev_page")
        {}
        void GotoPrevPage::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LALT, KEYCODE_LEFT) ;
            }) ;
        }
    }
}
