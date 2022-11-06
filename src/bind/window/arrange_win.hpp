#ifndef _ARRANGE_WINDOW_HPP
#define _ARRANGE_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct ArrangeWindows : public BindedFuncVoid<ArrangeWindows> {
            explicit ArrangeWindows() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
            void reconstruct() override ;
        } ;
    }
}
#endif
