#ifndef _MKDIR_HPP
#define _MKDIR_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct MakeDir : public BindedFuncCreator<MakeDir> {
        explicit MakeDir() ;
        static void sprocess(const std::string& path="New folder") ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
