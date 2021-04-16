#ifndef _PAGER_HPP
#define _PAGER_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct Move2NextPage : public BindedFuncCreator<Move2NextPage> {
        explicit Move2NextPage() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct Move2PrevPage : public BindedFuncCreator<Move2PrevPage> {
        explicit Move2PrevPage() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}
#endif
