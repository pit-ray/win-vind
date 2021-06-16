#include "bind/hotkey/search_pattern.hpp"
#include "io/keybrd.hpp"
#include "key/char_logger.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"

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
        keybrd::pushup(KEYCODE_F3) ;
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
