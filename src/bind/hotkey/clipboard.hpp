#ifndef _CLIPBOARD_HPP
#define _CLIPBOARD_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct HotkeyCopy : public BindedFuncVoid<HotkeyCopy> {
            explicit HotkeyCopy() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct HotkeyPaste : public BindedFuncVoid<HotkeyPaste> {
            explicit HotkeyPaste() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct HotkeyCut : public BindedFuncVoid<HotkeyCut> {
            explicit HotkeyCut() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct HotkeyDelete : public BindedFuncVoid<HotkeyDelete> {
            explicit HotkeyDelete() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct HotkeyBackspace : public BindedFuncVoid<HotkeyBackspace> {
            explicit HotkeyBackspace() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}
#endif
