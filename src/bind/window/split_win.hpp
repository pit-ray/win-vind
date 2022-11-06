#ifndef _SPLIT_OPEN_WINDOW_HPP
#define _SPLIT_OPEN_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct OpenNewWindowWithHorizontalSplit : public BindedFuncVoid<OpenNewWindowWithHorizontalSplit> {
            explicit OpenNewWindowWithHorizontalSplit() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct OpenNewWindowWithVerticalSplit : public BindedFuncVoid<OpenNewWindowWithVerticalSplit> {
            explicit OpenNewWindowWithVerticalSplit() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
