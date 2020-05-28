#include "alternative_text_selecter.hpp"

#include "keybrd_eventer.hpp"

namespace AlternativeTextSelecter
{
    static Mode _mode{Mode::UNSELECT} ;

    bool is_select_words() noexcept {
        _mode = Mode::WORD ;
        return true ;
    }

    bool is_select_line_EOL2BOL() noexcept {
        using namespace KeybrdEventer ;

        if(!is_pushup(VKC_END)) {
            return false ;
        }
        if(!is_pushup(VKC_LSHIFT, VKC_HOME)) {
            return false ;
        }

        _mode = Mode::EOL2BOL ;
        return true ;
    }

    bool is_select_line_BOL2EOL() noexcept {
        using namespace KeybrdEventer ;

        if(!is_pushup(VKC_HOME)) {
            return false ;
        }
        if(!is_pushup(VKC_LSHIFT, VKC_END)) {
            return false ;
        }

        _mode = Mode::BOL2EOL ;
        return true ;
    }

    bool is_unselect() noexcept {
        using namespace KeybrdEventer ;

        if(_mode == Mode::EOL2BOL) {
            if(!is_pushup(VKC_LEFT)) {
                return false ;
            }
        }
        else if(_mode == Mode::BOL2EOL) {
            if(!is_pushup(VKC_RIGHT)) {
                return false ;
            }
        }

        _mode = Mode::UNSELECT ;
        return true ;
    }

    Mode get_mode() noexcept {
        return _mode ;
    }
}