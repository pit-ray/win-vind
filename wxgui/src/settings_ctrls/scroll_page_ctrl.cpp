#include "scroll_page_ctrl.hpp"

#include "disable_gcc_warning.hpp"

#include <wx/sizer.h>

#include "enable_gcc_warning.hpp"

#include "io_params.hpp"
#include "ui_translator.hpp"

#include "ctrl_assets/slider_wl.hpp"
#include "ctrl_assets/spin_wl.hpp"

namespace wxGUI
{
    struct  ScrollPageCtrl::Impl {
        SpinWithLabel spin ;
        SliderWithLabel slider ;

        explicit Impl(wxWindow* const self)
        : spin(self),
          slider(self)
        {}
    } ;

    ScrollPageCtrl::ScrollPageCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>(this))
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER).Expand() ;
        auto root_sizer = new wxFlexGridSizer(2) ;

        pimpl->spin.set_sizer(root_sizer, flags) ;
        pimpl->slider.set_sizer(root_sizer, flags) ;

        pimpl->slider.create("yscroll_speed", 1, 50, 4) ;
        pimpl->slider.create("xscroll_speed", 1, 50, 4) ;
        pimpl->spin.create("yscroll_screen_ratio", 0.0, 1.0, 0.125, 0.001) ;
        pimpl->spin.create("xscroll_screen_ratio", 0.0, 1.0, 0.125, 0.001) ;

        SetSizerAndFit(root_sizer) ;
    }
    ScrollPageCtrl::~ScrollPageCtrl() noexcept = default ;

    void ScrollPageCtrl::translate() {
        pimpl->spin.translate() ;
        pimpl->slider.translate() ;
    }

    void ScrollPageCtrl::do_load_config() {
        pimpl->spin.load_config() ;
        pimpl->slider.translate() ;
    }

    void ScrollPageCtrl::do_load_config_default() {
        pimpl->spin.load_config_default() ;
        pimpl->slider.load_config_default() ;
    }

    void ScrollPageCtrl::do_save_config() {
        pimpl->spin.save_config() ;
        pimpl->slider.save_config() ;
    }

    const wxString ScrollPageCtrl::name() {
        return UITrans::trans("notify/preferences/settings/scroll_page") ;
    }
}
