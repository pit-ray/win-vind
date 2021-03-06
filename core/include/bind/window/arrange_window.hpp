#ifndef _ARRANGE_WINDOW_HPP
#define _ARRANGE_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct ArrangeWindows : public BindedFuncCreator<ArrangeWindows> {
        explicit ArrangeWindows() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}
#endif
