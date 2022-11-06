#ifndef _PAGER_HPP
#define _PAGER_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct GotoNextPage : public BindedFuncVoid<GotoNextPage> {
            explicit GotoNextPage() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct GotoPrevPage : public BindedFuncVoid<GotoPrevPage> {
            explicit GotoPrevPage() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}
#endif
