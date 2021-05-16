#ifndef _HOTKEYS_HPP
#define _HOTKEYS_HPP

#include <memory>

namespace vind
{
    class ScopedKey
    class Hotkeys {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit Hotkeys() ;

    } ;
}

#endif
