#ifndef _EXCHANGE_WINDOW_HPP
#define _EXCHANGE_WINDOW_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct ExchangeWindowWithNextOne : public BindedFuncWithCreator<ExchangeWindowWithNextOne> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
