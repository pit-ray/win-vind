#include "bind/emu/edi_dot.hpp"

#include "bind/binded_func.hpp"
#include "core/ntype_logger.hpp"


namespace vind
{
    const BindedFunc* RepeatLastChange::lastchange_ = nullptr ;
    unsigned int RepeatLastChange::repeat_count_ = 1 ;

    RepeatLastChange::RepeatLastChange()
    : BindedFuncCreator("repeat_last_change")
    {}

    //
    // In the original Vim, "last change" include from the moment enter insert mode
    // to the moment exit insert mode. However win-vind is not capturing in insert mode yet,
    // so we repeat the last change of only commands to edit.
    // Recording in insert mode will be a future work due to load concerns.
    //
    void RepeatLastChange::sprocess() {
        if(lastchange_) {
            lastchange_->process() ;
        }
    }

    void RepeatLastChange::sprocess(core::NTypeLogger& parent_lgr) {
        if(lastchange_) {
            if(repeat_count_ > 1 && parent_lgr.get_head_num() == 1) {
                parent_lgr.set_head_num(repeat_count_) ;
            }
            lastchange_->process(parent_lgr) ;
        }
    }

    void RepeatLastChange::sprocess(const core::CharLogger& parent_lgr) {
        if(lastchange_) {
            lastchange_->process(parent_lgr) ;
        }
    }
}
