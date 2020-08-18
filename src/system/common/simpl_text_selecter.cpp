#include "simpl_text_selecter.hpp"

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"

namespace SimplTextSelecter
{
    static Mode _mode{Mode::Unselect} ;

    static bool _first_line_selection = false ;
    static bool _first_call = true ;

    bool select_words() noexcept {
        _mode = Mode::Word ;
        return true ;
    }

    bool select_line_EOL2BOL() noexcept {
        _first_line_selection = _first_call ;

        if(!KeybrdEventer::pressup(VKC_END)) {
            return false ;
        }
        if(!KeybrdEventer::pressup(VKC_LSHIFT, VKC_HOME)) {
            return false ;
        }

        _mode = Mode::EOL2BOL ;
        _first_call = false ;
        return true ;
    }

    bool select_line_BOL2EOL() noexcept {
        _first_line_selection = _first_call ;
        if(!KeybrdEventer::pressup(VKC_HOME)) {
            return false ;
        }
        if(!KeybrdEventer::pressup(VKC_LSHIFT, VKC_END)) {
            return false ;
        }

        _mode = Mode::BOL2EOL ;
        _first_call = false ;
        return true ;
    }

    inline static void reset() noexcept {
        _first_call = true ;
        _first_line_selection = false ;

        _mode = Mode::Unselect ;
    }

    bool unselect() noexcept {
        if(_mode == Mode::EOL2BOL) {
            if(!KeybrdEventer::pressup(VKC_LEFT)) {
                return false ;
            }
        }
        else if(_mode == Mode::BOL2EOL) {
            if(!KeybrdEventer::pressup(VKC_RIGHT)) {
                return false ;
            }
        }

        reset() ;
        return true ;
    }

    Mode get_mode() noexcept {
        return _mode ;
    }

    bool moving_update() noexcept {
        if(_mode == Mode::BOL2EOL) {
            if(!KeybrdEventer::pressup(VKC_LSHIFT, VKC_END)) {
                return false ;
            }
        }
        else {
            if(!KeybrdEventer::pressup(VKC_LSHIFT, VKC_HOME)) {
                return false ;
            }
        }

        return true ;
    }

    bool is_first_line_selection() noexcept {
        return _first_line_selection ;
    }
}