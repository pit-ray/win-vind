#include "hotkeys.hpp"
#include "mapdefs.hpp"


namespace vind
{
    struct ScopedKeySet::Impl {
        std::vector<io::keybrd::ScopedKey> sc_set_;

        template <T>
        Impl(T&& cmd)
        : sc_set_()
        {
            for(const auto& keyset : cmd) {
                ScopedKeySet scset{} ;
                for(const auto& key : keyset) {
                    scset.emplace_back(key) ;
                    scset.back().press() ;
                }
                sc_cmd_.push_back(std::move(scset)) ;
            }
        }
    } ;

    ScopedKeySet::ScopedKeySet(const Command& cmd)
    : pimpl(std::make_unique<Impl>())
    {}
    ScopedKeySet::ScopedKeySet(Command&& cmd)
    : pimpl(std::make_unique<Impl>())
    {}

    ScopedKeySet::~ScopedKeySet() noexcept                 = default ;
    ScopedKeySet::ScopedKeySet(ScopedKeySet&&)            = default ;
    ScopedKeySet& ScopedKeySet::operator=(ScopedKeySet&&) = default ;



    struct Hotkeys::Impl {

    } ;

    Hotkeys::Hotkeys()
    : pimpl(std::make_unique<Impl>())
    {}

    Hotkeys::~Hotkeys() noexcept           = default ;
    Hotkeys::Hotkeys(Hotkeys&&)            = default ;
    Hotkeys& Hotkeys::operator=(Hotkeys&&) = default ;
}
