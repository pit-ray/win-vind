#ifndef _PAGER_HPP
#define _PAGER_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace bind
    {
        struct GotoNextPage : public BindedFuncCreator<GotoNextPage> {
            explicit GotoNextPage() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct GotoPrevPage : public BindedFuncCreator<GotoPrevPage> {
            explicit GotoPrevPage() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}
#endif
