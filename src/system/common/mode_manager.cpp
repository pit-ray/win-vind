#include "mode_manager.hpp"

namespace ModeManager
{
    static Mode system_mode{Mode::Normal} ;

    void change_mode(const Mode mode) noexcept {
        system_mode = mode ;
    }

    Mode get_mode() noexcept {
        return system_mode ;
    }

    bool is_edi_visual() noexcept {
        return system_mode == Mode::EdiVisual || system_mode == Mode::EdiLineVisual ;
    }

    bool is_editor() noexcept {
        return system_mode > Mode::_EditorModeThreshold ;
    }
}