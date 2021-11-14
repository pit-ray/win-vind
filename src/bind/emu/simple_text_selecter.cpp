#include "simple_text_selecter.hpp"

#include "util/keybrd.hpp"

namespace
{
    using namespace vind::bind ;
    SelectMode g_mode{SelectMode::Unselect} ;

    bool g_first_line_selection = false ;
    bool g_first_call = true ;

    inline void _reset() noexcept {
        g_first_call = true ;
        g_first_line_selection = false ;
        g_mode = SelectMode::Unselect ;
    }
}

namespace vind
{
    namespace bind
    {
        void select_words() noexcept {
            g_mode = SelectMode::Word ;
        }

        void select_line_EOL2BOL() {
            g_first_line_selection = g_first_call ;

            util::pushup(KEYCODE_END) ;
            util::pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;

            g_mode = SelectMode::EOL2BOL ;
            g_first_call = false ;
        }

        void select_line_BOL2EOL() {
            g_first_line_selection = g_first_call ;

            util::pushup(KEYCODE_HOME) ;
            util::pushup(KEYCODE_LSHIFT, KEYCODE_END) ;

            g_mode = SelectMode::BOL2EOL ;
            g_first_call = false ;
        }

        void unselect() {
            if(g_mode == SelectMode::EOL2BOL) {
                util::pushup(KEYCODE_LEFT) ;
            }
            else if(g_mode == SelectMode::BOL2EOL) {
                util::pushup(KEYCODE_RIGHT) ;
            }
            _reset() ;
        }

        SelectMode get_global_select_mode() noexcept {
            return g_mode ;
        }

        void moving_update() {
            if(g_mode == SelectMode::BOL2EOL)
                util::pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
            else
                util::pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;
        }

        bool is_first_line_selection() noexcept {
            return g_first_line_selection ;
        }
    }
}
