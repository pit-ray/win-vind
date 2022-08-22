#ifndef _MIN_MAX_WINDOW_HPP
#define _MIN_MAX_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct MinimizeCurrentWindow : public BindedFuncVoid<MinimizeCurrentWindow> {
            explicit MinimizeCurrentWindow() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MaximizeCurrentWindow : public BindedFuncVoid<MaximizeCurrentWindow> {
            explicit MaximizeCurrentWindow() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
