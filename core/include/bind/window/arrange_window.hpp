#ifndef _ARRANGE_WINDOW_HPP
#define _ARRANGE_WINDOW_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct ArrangeWindows : public BindedFuncWithCreator<ArrangeWindows> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}
#endif
