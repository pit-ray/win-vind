#ifndef _FILER_HPP
#define _FILER_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct Save : public BindedFuncCreator<Save> {
        explicit Save() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct Open : public BindedFuncCreator<Open> {
        explicit Open() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;
}

#endif
