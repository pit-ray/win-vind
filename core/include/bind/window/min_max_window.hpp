#ifndef _MIN_MAX_WINDOW_HPP
#define _MIN_MAX_WINDOW_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct MinimizeCurrentWindow : public BindedFuncCreator<MinimizeCurrentWindow> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct MaximizeCurrentWindow : public BindedFuncCreator<MaximizeCurrentWindow> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
