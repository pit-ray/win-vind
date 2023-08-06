#ifndef _AUTOCMD_BIND_HPP
#define _AUTOCMD_BIND_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct AutoCmdAdd : public BindedFuncVoid<AutoCmdAdd> {
            explicit AutoCmdAdd() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct AutoCmdRemove : public BindedFuncVoid<AutoCmdRemove> {
            explicit AutoCmdRemove() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
