#ifndef _MKDIR_HPP
#define _MKDIR_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct MakeDir : public BindedFuncCreator<MakeDir> {
        static void sprocess(std::string path="New folder") ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
