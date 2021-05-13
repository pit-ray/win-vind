#ifndef _FILER_HPP
#define _FILER_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct SaveOpenedFile : public BindedFuncCreator<SaveOpenedFile> {
        explicit SaveOpenedFile() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct OpenOtherFile : public BindedFuncCreator<OpenOtherFile> {
        explicit OpenOtherFile() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
