#include "edi_change_mode.hpp"

#include <windows.h>

#include "bind/mode/change_mode.hpp"
#include "core/inputgate.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "opt/vcmdline.hpp"
#include "simple_text_selecter.hpp"
#include "util/def.hpp"
#include "util/mouse.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{
    namespace bind
    {
        //ToInsertBOL
        ToInsertBOL::ToInsertBOL()
        : ChangeBaseCreator("to_insert_BOL")
        {}
        void ToInsertBOL::sprocess(bool vclmodeout) {
            core::InputGate::get_instance().pushup(KEYCODE_HOME) ;
            ToInsert::sprocess(vclmodeout) ;
        }
        void ToInsertBOL::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(true) ;
            }
        }
        void ToInsertBOL::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(true) ;
        }


        //Change2EdiBkInsert
        Change2EdiBkInsert::Change2EdiBkInsert()
        : ChangeBaseCreator("to_insert_append")
        {}
        void Change2EdiBkInsert::sprocess(bool vclmodeout) {
            core::InputGate::get_instance().pushup(KEYCODE_RIGHT) ;
            ToInsert::sprocess(vclmodeout) ;
        }
        void Change2EdiBkInsert::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(true) ;
            }
        }
        void Change2EdiBkInsert::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(true) ;
        }


        //ToInsertEOL
        ToInsertEOL::ToInsertEOL()
        : ChangeBaseCreator("to_insert_EOL")
        {}
        void ToInsertEOL::sprocess(bool vclmodeout) {
            core::InputGate::get_instance().pushup(KEYCODE_END) ;
            ToInsert::sprocess(vclmodeout) ;
        }
        void ToInsertEOL::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(true) ;
            }
        }
        void ToInsertEOL::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(true) ;
        }


        //ToInsertNLBelow
        ToInsertNLBelow::ToInsertNLBelow()
        : ChangeBaseCreator("to_insert_nlbelow")
        {}
        void ToInsertNLBelow::sprocess(bool vclmodeout) {
            auto& igate = core::InputGate::get_instance() ;
            igate.pushup(KEYCODE_END) ;
            igate.pushup(KEYCODE_ENTER) ;
            ToInsert::sprocess(vclmodeout) ;
        }
        void ToInsertNLBelow::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(true) ;
            }
        }
        void ToInsertNLBelow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(true) ;
        }


        //ToInsertNLAbove
        ToInsertNLAbove::ToInsertNLAbove()
        : ChangeBaseCreator("to_insert_nlabove")
        {}
        void ToInsertNLAbove::sprocess(bool vclmodeout) {
            auto& igate = core::InputGate::get_instance() ;
            igate.pushup(KEYCODE_HOME) ;
            igate.pushup(KEYCODE_ENTER) ;
            igate.pushup(KEYCODE_UP) ;
            ToInsert::sprocess(vclmodeout) ;
        }
        void ToInsertNLAbove::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(true) ;
            }
        }
        void ToInsertNLAbove::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(true) ;
        }
    }
}
