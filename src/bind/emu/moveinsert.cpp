#include "moveinsert.hpp"

#include <windows.h>

#include "bind/mode/change_mode.hpp"
#include "core/inputgate.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "opt/vcmdline.hpp"
#include "textsel.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/mouse.hpp"


namespace vind
{
    namespace bind
    {
        //ToInsertBOL
        ToInsertBOL::ToInsertBOL()
        : ChangeBaseCreator("to_insert_BOL")
        {}
        void ToInsertBOL::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args),
                bool vclmodeout) {
            core::InputGate::get_instance().pushup(KEYCODE_HOME) ;
            ToInsert::sprocess(1, "", vclmodeout) ;
        }

        //Change2EdiBkInsert
        Change2EdiBkInsert::Change2EdiBkInsert()
        : ChangeBaseCreator("to_insert_append")
        {}
        void Change2EdiBkInsert::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args),
                bool vclmodeout) {
            core::InputGate::get_instance().pushup(KEYCODE_RIGHT) ;
            ToInsert::sprocess(1, "", vclmodeout) ;
        }

        //ToInsertEOL
        ToInsertEOL::ToInsertEOL()
        : ChangeBaseCreator("to_insert_EOL")
        {}
        void ToInsertEOL::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args),
                bool vclmodeout) {
            core::InputGate::get_instance().pushup(KEYCODE_END) ;
            ToInsert::sprocess(1, "", vclmodeout) ;
        }

        //ToInsertNLBelow
        ToInsertNLBelow::ToInsertNLBelow()
        : ChangeBaseCreator("to_insert_nlbelow")
        {}
        void ToInsertNLBelow::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args),
                bool vclmodeout) {
            auto& igate = core::InputGate::get_instance() ;
            igate.pushup(KEYCODE_END) ;
            igate.pushup(KEYCODE_ENTER) ;
            ToInsert::sprocess(1, "", vclmodeout) ;
        }

        //ToInsertNLAbove
        ToInsertNLAbove::ToInsertNLAbove()
        : ChangeBaseCreator("to_insert_nlabove")
        {}
        void ToInsertNLAbove::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args),
                bool vclmodeout) {
            auto& igate = core::InputGate::get_instance() ;
            igate.pushup(KEYCODE_HOME) ;
            igate.pushup(KEYCODE_ENTER) ;
            igate.pushup(KEYCODE_UP) ;
            ToInsert::sprocess(1, "", vclmodeout) ;
        }
    }
}
