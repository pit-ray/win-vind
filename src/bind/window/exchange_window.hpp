#ifndef _EXCHANGE_WINDOW_HPP
#define _EXCHANGE_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace bind
    {
        struct ExchangeWindowWithNearest : public BindedFuncCreator<ExchangeWindowWithNearest> {
            explicit ExchangeWindowWithNearest() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
