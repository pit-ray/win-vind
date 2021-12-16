#include "simple_text_selecter.hpp"

#include "core/inputgate.hpp"


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

            auto& igate = core::InputGate::get_instance() ;
            igate.pushup(KEYCODE_END) ;
            igate.pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;

            g_mode = SelectMode::EOL2BOL ;
            g_first_call = false ;
        }

        void select_line_BOL2EOL() {
            g_first_line_selection = g_first_call ;

            auto& igate = core::InputGate::get_instance() ;
            igate.pushup(KEYCODE_HOME) ;
            igate.pushup(KEYCODE_LSHIFT, KEYCODE_END) ;

            g_mode = SelectMode::BOL2EOL ;
            g_first_call = false ;
        }

        void unselect() {
            auto& igate = core::InputGate::get_instance() ;
            if(g_mode == SelectMode::EOL2BOL) {
                igate.pushup(KEYCODE_LEFT) ;
            }
            else if(g_mode == SelectMode::BOL2EOL) {
                igate.pushup(KEYCODE_RIGHT) ;
            }
            _reset() ;
        }

        SelectMode get_global_select_mode() noexcept {
            return g_mode ;
        }

        void moving_update() {
            auto& igate = core::InputGate::get_instance() ;
            if(g_mode == SelectMode::BOL2EOL) {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
            }
            else {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_HOME) ;
            }
        }

        bool is_first_line_selection() noexcept {
            return g_first_line_selection ;
        }
    }
}
