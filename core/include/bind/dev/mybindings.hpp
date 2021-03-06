#if defined(DEBUG)

#ifndef MY_BINDING_HPP
#define MY_BINDING_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct MyBinding : public BindedFuncCreator<MyBinding> {
        explicit MyBinding() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif

#endif
