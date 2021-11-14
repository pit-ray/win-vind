#ifndef _RELOAD_WINDOW_HPP
#define _RELOAD_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace bind
    {
        struct ReloadCurrentWindow : public BindedFuncCreator<ReloadCurrentWindow> {
            explicit ReloadCurrentWindow() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
