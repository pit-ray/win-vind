#ifndef _PAGER_HPP
#define _PAGER_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct GotoNextPage : public BindedFuncCreator<GotoNextPage> {
        explicit GotoNextPage() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct GotoPrevPage : public BindedFuncCreator<GotoPrevPage> {
        explicit GotoPrevPage() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}
#endif
