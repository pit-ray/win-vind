#ifndef _EXCHANGE_WINDOW_HPP
#define _EXCHANGE_WINDOW_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    struct ExchangeWindowWithNextOne : public BindedFuncCreator<ExchangeWindowWithNextOne> {
        explicit ExchangeWindowWithNextOne() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
