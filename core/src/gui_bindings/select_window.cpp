#include "select_window.hpp"

#include "jump_cursor.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "key_logger.hpp"
#include "keybrd_eventer.hpp"
#include "move_cursor.hpp"
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
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

    KeyLogger logger{} ;
    while(win_vind::update_background()) {
        if(KeyAbsorber::is_pressed(VKC_ESC)) {
            break ;
        }
        if(KeyAbsorber::is_pressed(VKC_ENTER)) {
            break ;
        }

        if(!KyLgr::log_as_vkc(logger)) {
            Utility::remove_from_back(logger, 1) ;
            continue ;
        }
        if(KeyBinder::is_invalid_log(logger.back(),
                    KeyBinder::InvalidPolicy::UnbindedSystemKey)) {

            Utility::remove_from_back(logger, 1) ;
            continue ;
        }

        auto matched_func = KeyBinder::find_func(
                logger, nullptr, false,
                ModeManager::Mode::Normal) ;

        if(!matched_func) {
            logger.clear() ;
            continue ;
        }

        if(matched_func->is_callable()) {
            const auto name = matched_func->name() ;
            logger.clear() ;
            if(name == MoveLeft::sname()) {
                preserve_pushup(VKC_LEFT) ;
                continue ;
            }
            if(name == MoveRight::sname()) {
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


//SelectLeftWindow
const std::string SelectLeftWindow::sname() noexcept
{
    return "select_left_window" ;
}
void SelectLeftWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{

    /* NOT IMPLEMENTED */

}

//SelectRightWindow
const std::string SelectRightWindow::sname() noexcept
{
    return "select_right_window" ;
}
void SelectRightWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{

    /* NOT IMPLEMENTED */

}

//SelectUpperWindow
const std::string SelectUpperWindow::sname() noexcept
{
    return "select_upper_window" ;
}
void SelectUpperWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{

    /* NOT IMPLEMENTED */

}

//SelectLowerWindow
const std::string SelectLowerWindow::sname() noexcept
{
    return "select_lower_window" ;
}
void SelectLowerWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{

    /* NOT IMPLEMENTED */

}

//SelectWindowWithKeypush
const std::string SelectWindowWithKeypush::sname() noexcept
{
    return "select_window_with_keypush" ;
}
void SelectWindowWithKeypush::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{

    /* NOT IMPLEMENTED */

}
