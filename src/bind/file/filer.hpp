#ifndef _FILER_HPP
#define _FILER_HPP

#include "bind/binded_func.hpp"

namespace vind
{
    namespace bind
    {
        struct Save : public BindedFuncVoid<Save> {
            explicit Save() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct Open : public BindedFuncVoid<Open> {
            explicit Open() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
