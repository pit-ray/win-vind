#include "search_pattern.hpp"
#include "keybrd_eventer.hpp"
#include "utility.hpp"

#include <iostream>

namespace vind
{
    //SearchPattern
    const std::string SearchPattern::sname() noexcept {
        return "search_pattern" ;
    }

    void SearchPattern::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        KeybrdEventer::pushup(VKC_F3) ;
    }
}
