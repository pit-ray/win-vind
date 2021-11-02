#ifndef _EXECUTE_HPP
#define _EXECUTE_HPP

#include "bind/binded_func_creator.hpp"

#include <filesystem>


namespace vind
{
    struct Execute : public BindedFuncCreator<Execute> {
        explicit Execute() ;
        static void sprocess(std::filesystem::path filepath="") ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
