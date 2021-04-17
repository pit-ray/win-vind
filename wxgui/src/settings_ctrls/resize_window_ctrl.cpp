#include "resize_window_ctrl.hpp"

#include "disable_gcc_warning.hpp"

#include <wx/sizer.h>

#include "enable_gcc_warning.hpp"

#include "ctrl_assets/spin_wl.hpp"
#include "io_params.hpp"
#include "ui_translator.hpp"

namespace wxGUI
{
    struct ResizeWindowCtrl::Impl {
        SpinWithLabel spin ;

        explicit Impl(wxWindow* const self)
        : spin(self)
        {}
    } ;

    ResizeWindowCtrl::ResizeWindowCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>(this))
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto root_sizer = new wxFlexGridSizer(2) ;

        pimpl->spin.set_sizer(root_sizer, flags) ;
        pimpl->spin.create("window_width_delta", 1, 10000, 10) ;
        pimpl->spin.create("window_height_delta", 1, 10000, 10) ;
        SetSizerAndFit(root_sizer) ;
    }
    ResizeWindowCtrl::~ResizeWindowCtrl() noexcept = default ;

    void ResizeWindowCtrl::translate() {
        pimpl->spin.translate() ;
    }

    void ResizeWindowCtrl::do_load_config() {
        pimpl->spin.load_config() ;
    }

    void ResizeWindowCtrl::do_load_config_default() {
        pimpl->spin.load_config_default() ;
    }

    void ResizeWindowCtrl::do_save_config() {
        pimpl->spin.save_config() ;
    }

    const wxString ResizeWindowCtrl::name() {
        return UITrans::trans("notify/preferences/settings/resize_window") ;
    }

}
