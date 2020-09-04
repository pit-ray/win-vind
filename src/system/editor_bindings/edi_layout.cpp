#include "edi_layout.hpp"

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"

const std::string EdiNRemoveEOL::sname() noexcept
{
    return "edi_n_remove_EOL" ;
}
bool EdiNRemoveEOL::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(!KeybrdEventer::pressup(VKC_END)) {
        return false ;
    }
    if(!KeybrdEventer::pressup(VKC_DELETE)) {
        return false ;
    }
    ModeManager::change_mode(ModeManager::Mode::EdiNormal) ;
    return true ;
}