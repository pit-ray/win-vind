#ifndef _HOTKEYS_HPP
#define _HOTKEYS_HPP

#include "io/keybrd.hpp"

#include <initializer_list>
#include <memory>

namespace vind
{
    class ScopedKeySet {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit ScopedKeySet(const Command& cmd) ;
        explicit ScopedKeySet(Command&& cmd) ;

        virtual ~ScopedKeySet() noexcept ;

        ScopedKeySet(ScopedKeySet&&) ;
        ScopedKeySet& operator=(ScopedKeySet&&) ;

        ScopedKeySet(const ScopedKeySet&)            = delete ;
        ScopedKeySet& operator=(const ScopedKeySet&) = delete ;
    } ;

    class Hotkeys {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit Hotkeys() ;
        virtual ~Hotkeys() noexcept ;

        Hotkeys(Hotkeys&&) ;
        Hotkeys& operator=(Hotkeys&&) ;

        Hotkeys(const Hotkeys&)            = delete ;
        Hotkeys& operator=(const Hotkeys&) = delete ;
    } ;
}

#endif
