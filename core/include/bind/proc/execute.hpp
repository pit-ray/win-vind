#ifndef _EXECUTE_HPP
#define _EXECUTE_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct Execute : public BindedFuncCreator<Execute> {
        explicit Execute() ;
        static void sprocess(std::string filepath="") ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
