#ifndef _FILER_HPP
#define _FILER_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct Save : public BindedFuncVoid<Save> {
            explicit Save() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct Open : public BindedFuncVoid<Open> {
            explicit Open() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
