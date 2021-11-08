#include "search_pattern.hpp"

#include "core/char_logger.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    //SearchPattern
    SearchPattern::SearchPattern()
    : BindedFuncCreator("search_pattern")
    {}
    void SearchPattern::sprocess() {
        util::pushup(KEYCODE_F3) ;
    }
    void SearchPattern::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SearchPattern::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
