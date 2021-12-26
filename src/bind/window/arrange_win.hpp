#ifndef _ARRANGE_WINDOW_HPP
#define _ARRANGE_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct ArrangeWindows : public BindedFuncVoid<ArrangeWindows> {
            explicit ArrangeWindows() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;

            void reconstruct() override ;
        } ;
    }
}
#endif
