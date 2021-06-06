#ifndef _CLIPBOARD_HPP
#define _CLIPBOARD_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct HotkeyCopy : public BindedFuncCreator<HotkeyCopy> {
        explicit HotkeyCopy() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct HotkeyPaste : public BindedFuncCreator<HotkeyPaste> {
        explicit HotkeyPaste() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct HotkeyCut : public BindedFuncCreator<HotkeyCut> {
        explicit HotkeyCut() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct HotkeyDelete : public BindedFuncCreator<HotkeyDelete> {
        explicit HotkeyDelete() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct HotkeyBackspace : public BindedFuncCreator<HotkeyBackspace> {
        explicit HotkeyBackspace() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}
#endif
