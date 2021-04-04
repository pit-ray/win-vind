#include "search_pattern.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

#include <iostream>

namespace vind
{
    //SearchPattern
    const std::string SearchPattern::sname() noexcept {
        return "search_pattern" ;
    }

    void SearchPattern::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        keybrd::pushup(KEYCODE_F3) ;
    }
}
