#ifndef _EXECUTE_HPP
#define _EXECUTE_HPP

#include "bind/bindedfunc.hpp"

#include <filesystem>


namespace vind
{
    namespace bind
    {
        struct Execute : public BindedFuncVoid<Execute> {
            explicit Execute() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
