#ifndef _EXCHANGE_WINDOW_HPP
#define _EXCHANGE_WINDOW_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct ExchangeWindowWithNextOne : public BindedFuncWithCreator<ExchangeWindowWithNextOne> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
