#include "switch_window.hpp"

#include <windows.h>

#include "entry.hpp"

#include "io/keybrd.hpp"
#include "key/key_absorber.hpp"
#include "util/def.hpp"

#include "bind.hpp"

#include "emu/edi_move_caret.hpp"
#include "mouse/jump_actwin.hpp"

namespace vind
{
    //SwitchWindow
    const std::string SwitchWindow::sname() noexcept {
        return "switch_window" ;
    }

    void SwitchWindow::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        using namespace keybrd ;
        if(!first_call) return ;

        keyabsorber::InstantKeyAbsorber ika ;

        SmartKey alt(KEYCODE_LALT) ;
        alt.press() ;
        keyabsorber::release_virtually(KEYCODE_LALT) ;

        pushup(KEYCODE_TAB) ;

        auto preserve_pushup = [] (const auto vkc) {
            using namespace keybrd ;
            if(!keyabsorber::is_pressed(vkc)) {
                pushup(vkc) ;
                return ;
            }

            release_keystate(vkc) ;
            pushup(vkc) ;
            press_keystate(vkc) ;
        } ;

        KeycodeLogger logger{} ;
        while(vind::update_background()) {
            if(keyabsorber::is_pressed(KEYCODE_ESC)) {
                break ;
            }
            if(keyabsorber::is_pressed(KEYCODE_ENTER)) {
                break ;
            }

            logger.update() ;
            if(!logger.is_changed()) {
                logger.remove_from_back(1) ;
                continue ;
            }
            if(keybind::is_invalid_log(logger.latest(),
                        keybind::InvalidPolicy::UnbindedSystemKey)) {

                logger.remove_from_back(1) ;
                continue ;
            }

            auto matched_func = keybind::find_func(
                    logger, nullptr, false,
                    mode::Mode::EdiNormal) ;

            if(!matched_func) {
                logger.clear() ;
                continue ;
            }

            if(matched_func->is_callable()) {
                const auto name = matched_func->name() ;
                logger.clear() ;
                if(name == EdiMoveCaretLeft::sname()) {
                    preserve_pushup(KEYCODE_LEFT) ;
                    continue ;
                }
                if(name == EdiMoveCaretRight::sname()) {
                    preserve_pushup(KEYCODE_RIGHT) ;
                    continue ;
                }
            }
        }

        keyabsorber::release_virtually(KEYCODE_ESC) ;
        keyabsorber::release_virtually(KEYCODE_ENTER) ;

        alt.release() ;

        //jump cursor to a selected window after releasing alt and tab.
        Sleep(50) ; //send select-message to OS(wait)
        Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
    }
}
