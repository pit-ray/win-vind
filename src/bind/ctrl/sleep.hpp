#ifndef _SLEEP_HPP
#define _SLEEP_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct Sleep : public BindedFuncVoid<Sleep> {
            explicit Sleep() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
