#ifndef _HELP_HPP
#define _HELP_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct Help : public BindedFuncVoid<Help> {
            explicit Help() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
