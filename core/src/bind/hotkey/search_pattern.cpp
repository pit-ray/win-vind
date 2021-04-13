#include "bind/hotkey/search_pattern.hpp"
#include "bind/base/char_logger.hpp"
#include "bind/base/ntype_logger.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

#include <iostream>

namespace vind
{
    //SearchPattern
    const std::string SearchPattern::sname() noexcept {
        return "search_pattern" ;
    }
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
