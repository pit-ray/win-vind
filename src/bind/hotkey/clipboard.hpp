#ifndef _CLIPBOARD_HPP
#define _CLIPBOARD_HPP

#include "bind/binded_func.hpp"

namespace vind
{
    namespace bind
    {
        struct HotkeyCopy : public BindedFuncVoid<HotkeyCopy> {
            explicit HotkeyCopy() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct HotkeyPaste : public BindedFuncVoid<HotkeyPaste> {
            explicit HotkeyPaste() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct HotkeyCut : public BindedFuncVoid<HotkeyCut> {
            explicit HotkeyCut() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct HotkeyDelete : public BindedFuncVoid<HotkeyDelete> {
            explicit HotkeyDelete() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct HotkeyBackspace : public BindedFuncVoid<HotkeyBackspace> {
            explicit HotkeyBackspace() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}
#endif
