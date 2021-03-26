#include "ctrl_core.hpp"

namespace wxGUI
{
    CtrlCore::CtrlCore(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id)
    {}
    CtrlCore::~CtrlCore() noexcept = default ;

    void CtrlCore::load_config() {
        translate() ;
        wxPanel::Fit() ;
        do_load_config() ;
    }

    void CtrlCore::load_config_default() {
        translate() ;
        wxPanel::Fit() ;
        do_load_config_default() ;
    }

    void CtrlCore::save_config() {
        do_save_config() ;
        translate() ;
        wxPanel::Fit() ;
    }
}
