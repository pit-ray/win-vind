#include "mode_manager.hpp"

namespace ModeManager
{
    static Mode m{Mode::Normal} ;

    void change_mode(const Mode mode) noexcept {
        m = mode ;
    }
    void change_mode(const int mode) noexcept {
        m = static_cast<Mode>(mode) ;
    }
    void change_mode(const unsigned char mode) noexcept {
        m = static_cast<Mode>(mode) ;
    }

    Mode get_mode() noexcept {
        return m ;
    }

    bool is_edi_visual() noexcept {
        return m == Mode::EdiVisual || m == Mode::EdiLineVisual ;
    }

    bool is_editor() noexcept {
        return m > Mode::_EditorModeThreshold ;
    }

    bool is_command() noexcept {
        return m == Mode::Command || m == Mode::EdiCommand ;
    }
}