#include "simple_text_selecter.hpp"

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"

namespace SimpleTextSelecter
{
    static Mode g_mode{Mode::Unselect} ;

    static bool g_first_line_selection = false ;
    static bool g_first_call = true ;

    void select_words() noexcept {
        g_mode = Mode::Word ;
    }

    void select_line_EOL2BOL() {
        g_first_line_selection = g_first_call ;

        KeybrdEventer::pushup(VKC_END) ;
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_HOME) ;

        g_mode = Mode::EOL2BOL ;
        g_first_call = false ;
    }

    void select_line_BOL2EOL() {
        g_first_line_selection = g_first_call ;

        KeybrdEventer::pushup(VKC_HOME) ;
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_END) ;

        g_mode = Mode::BOL2EOL ;
        g_first_call = false ;
    }

    inline static void _reset() noexcept {
        g_first_call = true ;
        g_first_line_selection = false ;
        g_mode = Mode::Unselect ;
    }

    void unselect() {
        if(g_mode == Mode::EOL2BOL) {
            KeybrdEventer::pushup(VKC_LEFT) ;
        }
        else if(g_mode == Mode::BOL2EOL) {
            KeybrdEventer::pushup(VKC_RIGHT) ;
        }
        _reset() ;
    }

    Mode getg_mode() noexcept {
        return g_mode ;
    }

    void moving_update() {
        if(g_mode == Mode::BOL2EOL)
            KeybrdEventer::pushup(VKC_LSHIFT, VKC_END) ;
        else
            KeybrdEventer::pushup(VKC_LSHIFT, VKC_HOME) ;
    }

    bool is_first_line_selection() noexcept {
        return g_first_line_selection ;
    }
}
