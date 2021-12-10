#ifndef _SET_HPP
#define _SET_HPP

#include "bind/binded_func.hpp"

namespace vind
{
    namespace bind
    {
        struct SyscmdSet : BindedFuncFlex<SyscmdSet> {
            explicit SyscmdSet() ;
            static core::SystemCall sprocess(const std::string& args="") ;
            static core::SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static core::SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
