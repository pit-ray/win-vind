#include "search_pattern.hpp"

#include "core/char_logger.hpp"
#include "core/inputgate.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"


#if defined(DEBUG)
#include <iostream>
#endif

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
