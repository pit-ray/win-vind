#include "bind/window/switch_window.hpp"

#include <windows.h>

#include "entry.hpp"

#include "io/keybrd.hpp"
#include "key/key_absorber.hpp"
#include "util/def.hpp"

#include "bind/bind.hpp"

#include "bind/emu/edi_move_caret.hpp"
#include "bind/mouse/jump_actwin.hpp"
#include "bind/base/ntype_logger.hpp"
#include "bind/base/keycode_logger.hpp"

namespace vind
{
    //SwitchWindow
    const std::string SwitchWindow::sname() noexcept {
        return "switch_window" ;
    }

    void SwitchWindow::sprocess() {
        using namespace keybrd ;
        keyabsorber::InstantKeyAbsorber ika ;

        SmartKey alt(KEYCODE_LALT) ;
        alt.press() ;
        keyabsorber::release_virtually(KEYCODE_LALT) ;

        pushup(KEYCODE_TAB) ;

        auto preserve_pushup = [] (const auto keycode) {
            using namespace keybrd ;
            if(!keyabsorber::is_pressed(keycode)) {
                pushup(keycode) ;
                return ;
            }

            release_keystate(keycode) ;
            pushup(keycode) ;
            press_keystate(keycode) ;
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
                continue ;
            }
            /*
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
            */
        }

        keyabsorber::release_virtually(KEYCODE_ESC) ;
        keyabsorber::release_virtually(KEYCODE_ENTER) ;

        alt.release() ;

        //jump cursor to a selected window after releasing alt and tab.
        Sleep(50) ; //send select-message to OS(wait)
        Jump2ActiveWindow::sprocess() ;
    }
    void SwitchWindow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SwitchWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
