#include "layouttext.hpp"

#include "bind/saferepeat.hpp"
#include "core/inputgate.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "util/def.hpp"
#include "util/keystroke_repeater.hpp"


namespace vind
{
    namespace bind
    {
        JoinNextLine::JoinNextLine()
        : ChangeBaseCreator("join_next_line")
        {}
        void JoinNextLine::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            safe_for(count, [] {
                auto& igate = core::InputGate::get_instance() ;
                igate.pushup(KEYCODE_END) ;
                igate.pushup(KEYCODE_DELETE) ;
            }) ;
        }
    }
}
