#include "options_ctrl.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/sizer.h>
#include "enable_gcc_warning.hpp"

#include "io_params.hpp"
#include "ui_translator.hpp"

#include "ctrl_assets/checkbox_wl.hpp"

namespace wxGUI
{
    struct OptionsCtrl::Impl {
        CheckBoxWithLabel check ;

        explicit Impl(wxWindow* const self)
        : check(self)
        {}
    } ;

    OptionsCtrl::OptionsCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>(this))
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;
        auto root_sizer = new wxBoxSizer(wxVERTICAL) ;

        pimpl->check.set_sizer(root_sizer, flags) ;

        pimpl->check.create("autotrack_popup") ;
        pimpl->check.create("dedicate_to_window") ;
        pimpl->check.create("enable_char_cache") ;
        pimpl->check.create("enable_esyclk_hwnd_cache") ;
        pimpl->check.create("suppress_for_vim") ;
        pimpl->check.create("virtual_cmd_line") ;

        SetSizerAndFit(root_sizer) ;
    }
    OptionsCtrl::~OptionsCtrl() noexcept = default ;

    void OptionsCtrl::translate() {
        pimpl->check.translate() ;
    }

    void OptionsCtrl::do_load_config() {
        pimpl->check.load_config() ;
    }

    void OptionsCtrl::do_load_config_default() {
        pimpl->check.load_config_default() ;
    }

    void OptionsCtrl::do_save_config() {
        pimpl->check.save_config() ;
    }

    const wxString OptionsCtrl::name() noexcept {
        return UITrans::trans("notify/preferences/settings/options") ;
    }
}
