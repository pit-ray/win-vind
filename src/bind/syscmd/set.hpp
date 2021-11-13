#ifndef _SET_HPP
#define _SET_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct SyscmdSet : BindedFuncCreator<SyscmdSet> {
        explicit SyscmdSet() ;
        static void sprocess(
                const std::string& args="",
                bool reload_config=false) ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;
}

#endif
