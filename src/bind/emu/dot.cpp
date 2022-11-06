#include "bind/emu/dot.hpp"

#include "bind/bindedfunc.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        //
        // NOTE:
        // In the original Vim, "last change" include from the moment enter insert mode
        // to the moment exit insert mode. However win-vind is not capturing in insert mode yet,
        // so we repeat the last change of only commands to edit.
        // Recording in insert mode will be a future work due to load concerns.
        //
        BindedFunc* RepeatLastChange::lastchange_ = nullptr ;
        std::uint16_t RepeatLastChange::repeat_count_ = 1 ;

        RepeatLastChange::RepeatLastChange()
        : BindedFuncFlex("repeat_last_change")
        {}
        SystemCall RepeatLastChange::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            if(!lastchange_) {
                return SystemCall::SUCCEEDED ;
            }
            if(repeat_count_ > 1 && count == 1) {
                count = repeat_count_ ;
            }
            return lastchange_->process(count) ;
        }
    }
}
