#include "ctrl_core.hpp"

namespace wxGUI
{
    CtrlCore::CtrlCore(wxWindow* parent, wxWindowID id)
    : wxWindow(parent, id, wxDefaultPosition, wxDefaultSize)
    {}
    CtrlCore::~CtrlCore() noexcept = default ;

    void CtrlCore::load_config() noexcept {
        translate() ;
        do_load_config() ;
    }

    void CtrlCore::load_config_default() noexcept {
        translate() ;
        do_load_config_default() ;
    }

    void CtrlCore::save_config() noexcept {
        do_save_config() ;
        translate() ;
    }
}