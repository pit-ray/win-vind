#include "switch_window.hpp"

#include "edi_move_caret.hpp"
#include "jump_actwin.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "keybrd_eventer.hpp"
#include "utility.hpp"
#include "win_vind.hpp"

//SwitchWindow
const std::string SwitchWindow::sname() noexcept
{
    return "switch_window" ;
}

void SwitchWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    using namespace KeybrdEventer ;
    if(!first_call) return ;

    KeyAbsorber::InstantKeyAbsorber ika ;

    SmartKey alt(VKC_LALT) ;
    alt.press() ;
    KeyAbsorber::release_virtually(VKC_LALT) ;

    pushup(VKC_TAB) ;

    auto preserve_pushup = [] (const auto vkc) {
        using namespace KeybrdEventer ;
        if(!KeyAbsorber::is_pressed(vkc)) {
            pushup(vkc) ;
            return ;
        }

        release_keystate(vkc) ;
        pushup(vkc) ;
        press_keystate(vkc) ;
    } ;

    VKCLogger logger{} ;
    while(win_vind::update_background()) {
        if(KeyAbsorber::is_pressed(VKC_ESC)) {
            break ;
        }
        if(KeyAbsorber::is_pressed(VKC_ENTER)) {
            break ;
        }

        logger.update() ;
        if(!logger.is_changed()) {
            logger.remove_from_back(1) ;
            continue ;
        }
        if(KeyBinder::is_invalid_log(logger.latest(),
                    KeyBinder::InvalidPolicy::UnbindedSystemKey)) {

            logger.remove_from_back(1) ;
            continue ;
        }

        auto matched_func = KeyBinder::find_func(
                &logger, nullptr, false,
                ModeManager::Mode::EdiNormal) ;

        if(!matched_func) {
            logger.clear() ;
            continue ;
        }

        if(matched_func->is_callable()) {
            const auto name = matched_func->name() ;
            logger.clear() ;
            if(name == EdiMoveCaretLeft::sname()) {
                preserve_pushup(VKC_LEFT) ;
                continue ;
            }
            if(name == EdiMoveCaretRight::sname()) {
                preserve_pushup(VKC_RIGHT) ;
                continue ;
            }
        }
    }

    KeyAbsorber::release_virtually(VKC_ESC) ;
    KeyAbsorber::release_virtually(VKC_ENTER) ;

    alt.release() ;

    //jump cursor to a selected window after releasing alt and tab.
    Sleep(50) ; //send select-message to OS(wait)
    Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
}

