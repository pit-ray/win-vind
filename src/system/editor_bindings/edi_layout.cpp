#include "edi_layout.hpp"

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "keystroke_repeater.hpp"

struct EdiNRemoveEOL::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNRemoveEOL::EdiNRemoveEOL()
: pimpl(std::make_unique<Impl>())
{}

EdiNRemoveEOL::~EdiNRemoveEOL() noexcept = default ;
EdiNRemoveEOL::EdiNRemoveEOL(EdiNRemoveEOL&&) noexcept = default ;
EdiNRemoveEOL& EdiNRemoveEOL::operator=(EdiNRemoveEOL&&) noexcept = default ;
const std::string EdiNRemoveEOL::sname() noexcept
{
    return "edi_n_remove_EOL" ;
}
bool EdiNRemoveEOL::sprocess(const bool first_call) const
{
    auto remove = [] {
        if(!KeybrdEventer::pushup(VKC_END)) {
            return false ;
        }
        if(!KeybrdEventer::pushup(VKC_DELETE)) {
            return false ;
        }
        ModeManager::change_mode(ModeManager::Mode::EdiNormal) ;
        return true ;
    } ;
    if(first_call) {
        pimpl->ksr.reset() ;
        return remove() ;
    }
    if(pimpl->ksr.is_pressed()) {
        return remove() ;
    }
    return true ;
}