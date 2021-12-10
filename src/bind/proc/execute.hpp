#ifndef _EXECUTE_HPP
#define _EXECUTE_HPP

#include "bind/binded_func.hpp"

#include <filesystem>


namespace vind
{
    namespace bind
    {
        struct Execute : public BindedFuncVoid<Execute> {
            explicit Execute() ;
            static void sprocess(std::filesystem::path filepath="") ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
