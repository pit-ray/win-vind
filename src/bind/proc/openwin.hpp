#ifndef _OPEN_WINDOW_HPP
#define _OPEN_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct OpenNewWindow : public BindedFuncVoid<OpenNewWindow> {
            explicit OpenNewWindow() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
