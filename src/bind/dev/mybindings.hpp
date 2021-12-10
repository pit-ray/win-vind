#if defined(DEBUG)

#ifndef MY_BINDING_HPP
#define MY_BINDING_HPP

#include "bind/binded_func.hpp"

namespace vind
{
    namespace bind
    {
        struct MyBinding : public BindedFuncVoid<MyBinding> {
            explicit MyBinding() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif

#endif
