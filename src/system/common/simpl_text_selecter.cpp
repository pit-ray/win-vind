#include "simpl_text_selecter.hpp"

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"

namespace SimplTextSelecter
{
    static Mode _mode{Mode::Unselect} ;

    static bool _first_line_selection = false ;
    static bool _first_call = true ;

    void select_words() noexcept {
        _mode = Mode::Word ;
    }

    void select_line_EOL2BOL() {
        _first_line_selection = _first_call ;

        KeybrdEventer::pushup(VKC_END) ;
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_HOME) ;

        _mode = Mode::EOL2BOL ;
        _first_call = false ;
    }

    void select_line_BOL2EOL() {
        _first_line_selection = _first_call ;

        KeybrdEventer::pushup(VKC_HOME) ;
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_END) ;

        _mode = Mode::BOL2EOL ;
        _first_call = false ;
    }

    inline static void _reset() noexcept {
        _first_call = true ;
        _first_line_selection = false ;
        _mode = Mode::Unselect ;
    }

    void unselect() {
        if(_mode == Mode::EOL2BOL) {
            KeybrdEventer::pushup(VKC_LEFT) ;
        }
        else if(_mode == Mode::BOL2EOL) {
            KeybrdEventer::pushup(VKC_RIGHT) ;
        }
        _reset() ;
    }

    Mode get_mode() noexcept {
        return _mode ;
    }

    void moving_update() {
        if(_mode == Mode::BOL2EOL) {
            KeybrdEventer::pushup(VKC_LSHIFT, VKC_END) ;
        }
        else {
            KeybrdEventer::pushup(VKC_LSHIFT, VKC_HOME) ;
        }
    }

    bool is_first_line_selection() noexcept {
        return _first_line_selection ;
    }
}
