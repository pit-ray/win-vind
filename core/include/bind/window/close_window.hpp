#ifndef _CLOSE_WINDOW_HPP
#define _CLOSE_WINDOW_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct CloseCurrentWindow : public BindedFuncCreator<CloseCurrentWindow> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
