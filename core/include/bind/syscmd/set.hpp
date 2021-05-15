#ifndef _SET_HPP
#define _SET_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct SyscmdSet : BindedFuncCreator<SyscmdSet> {
        explicit SyscmdSet() ;
        static void sprocess(const std::string& args="") ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
