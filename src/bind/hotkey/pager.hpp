#ifndef _PAGER_HPP
#define _PAGER_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct GotoNextPage : public BindedFuncVoid<GotoNextPage> {
            explicit GotoNextPage() ;
            static void sprocess(unsigned int count=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct GotoPrevPage : public BindedFuncVoid<GotoPrevPage> {
            explicit GotoPrevPage() ;
            static void sprocess(unsigned int count=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}
#endif
