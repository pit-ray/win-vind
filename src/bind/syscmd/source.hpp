#ifndef _SOURCE_HPP
#define _SOURCE_HPP

#include "bind/bindedfunc.hpp"
#include "core/path.hpp"

namespace vind
{
    namespace bind
    {
        struct Source : public BindedFuncFlex<Source> {
            explicit Source() ;

            static SystemCall sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
