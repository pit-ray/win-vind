#ifndef _FILER_HPP
#define _FILER_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct SaveOpenedFile : public BindedFuncCreator<SaveOpenedFile> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct OpenOtherFile : public BindedFuncCreator<OpenOtherFile> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
