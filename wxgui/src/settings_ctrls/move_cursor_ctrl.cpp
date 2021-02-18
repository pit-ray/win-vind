#include "move_cursor_ctrl.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/slider.h>
#include "enable_gcc_warning.hpp"

#include "io_params.hpp"
#include "ui_translator.hpp"

#include "ctrl_assets/spin_wl.hpp"
#include "ctrl_assets/slider_wl.hpp"

namespace wxGUI
{
    struct MoveCursorCtrl::Impl {
        SpinWithLabel spin ;
        SliderWithLabel slider ;

        explicit Impl(wxWindow* const self)
        : spin(self),
          slider(self)
        {}
    } ;

    MoveCursorCtrl::MoveCursorCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>(this))
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER).Expand() ;
        auto root_sizer = new wxFlexGridSizer(2) ;

        pimpl->spin.set_sizer(root_sizer, flags) ;
        pimpl->slider.set_sizer(root_sizer, flags) ;

        pimpl->spin.create("screen_pos_buf", 0, 128, 10) ;
        pimpl->slider.create("cursor_acceleration", 1, 128, 64) ;
        pimpl->slider.create("cursor_weight", 1, 1024, 512) ;
        pimpl->slider.create("cursor_max_velocity", 1, 20, 10) ;

        SetSizerAndFit(root_sizer) ;
    }
    MoveCursorCtrl::~MoveCursorCtrl() noexcept = default ;

    void MoveCursorCtrl::translate() {
        pimpl->spin.translate() ;
        pimpl->slider.translate() ;
    }

    void MoveCursorCtrl::do_load_config() {
        pimpl->spin.load_config() ;
        pimpl->slider.load_config() ;
    }

    void MoveCursorCtrl::do_load_config_default() {
        pimpl->spin.load_config_default() ;
        pimpl->slider.load_config_default() ;
    }

    void MoveCursorCtrl::do_save_config() {
        pimpl->spin.save_config() ;
        pimpl->slider.save_config() ;
    }

    const wxString MoveCursorCtrl::name() noexcept {
        return UITrans::trans("notify/preferences/settings/move_cursor") ;
    }
}
