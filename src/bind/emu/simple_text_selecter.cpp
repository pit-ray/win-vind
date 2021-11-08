#include "simple_text_selecter.hpp"

#include "util/keybrd.hpp"

namespace
{
    using namespace vind::textselect ;
    Mode g_mode{Mode::Unselect} ;

    bool g_first_line_selection = false ;
    bool g_first_call = true ;

    inline void _reset() noexcept {
        g_first_call = true ;
        g_first_line_selection = false ;
        g_mode = Mode::Unselect ;
    }
}

namespace vind
{
    namespace textselect
    {
        void select_words() noexcept {
            g_mode = Mode::Word ;
        }

        void select_line_EOL2BOL() {
            g_first_line_selection = g_first_call ;

            util::pushup(KEYCODE_END) ;
            util::pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;

            g_mode = Mode::EOL2BOL ;
            g_first_call = false ;
        }

        void select_line_BOL2EOL() {
            g_first_line_selection = g_first_call ;

            util::pushup(KEYCODE_HOME) ;
            util::pushup(KEYCODE_LSHIFT, KEYCODE_END) ;

            g_mode = Mode::BOL2EOL ;
            g_first_call = false ;
        }

        void unselect() {
            if(g_mode == Mode::EOL2BOL) {
                util::pushup(KEYCODE_LEFT) ;
            }
            else if(g_mode == Mode::BOL2EOL) {
                util::pushup(KEYCODE_RIGHT) ;
            }
            _reset() ;
        }

        Mode getg_mode() noexcept {
            return g_mode ;
        }

        void moving_update() {
            if(g_mode == Mode::BOL2EOL)
                util::pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
            else
                util::pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;
        }

        bool is_first_line_selection() noexcept {
            return g_first_line_selection ;
        }
    }
}
