#ifndef _CLOSE_WINDOW_HPP
#define _CLOSE_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct CloseCurrentWindow : public BindedFuncVoid<CloseCurrentWindow> {
            explicit CloseCurrentWindow() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
