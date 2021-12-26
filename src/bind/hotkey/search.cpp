#include "search.hpp"

#include "core/charlogger.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        //SearchPattern
        SearchPattern::SearchPattern()
        : BindedFuncVoid("search_pattern")
        {}
        void SearchPattern::sprocess() {
            core::InputGate::get_instance().pushup(KEYCODE_F3) ;
        }
        void SearchPattern::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void SearchPattern::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
