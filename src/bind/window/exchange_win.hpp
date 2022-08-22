#ifndef _EXCHANGE_WINDOW_HPP
#define _EXCHANGE_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct ExchangeWindowWithNearest : public BindedFuncVoid<ExchangeWindowWithNearest> {
            explicit ExchangeWindowWithNearest() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
