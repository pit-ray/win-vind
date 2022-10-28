#ifndef _SET_HPP
#define _SET_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct Set : public BindedFuncFlex<Set> {
            explicit Set() ;
            static SystemCall sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
