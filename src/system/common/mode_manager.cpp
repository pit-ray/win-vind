#include "mode_manager.hpp"

namespace ModeManager
{
    static Mode m{Mode::Normal} ;

    void change_mode(const Mode mode) noexcept {
        m = mode ;
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
}