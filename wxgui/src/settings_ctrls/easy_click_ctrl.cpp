#include "easy_click_ctrl.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/sizer.h>
#include "enable_gcc_warning.hpp"

#include "io_params.hpp"
#include "ui_translator.hpp"

#include "ctrl_assets/spin_wl.hpp"
#include "ctrl_assets/slider_wl.hpp"
#include "ctrl_assets/clrpicker_wl.hpp"

namespace wxGUI
{
    struct EasyClcikCtrl::Impl {
        SpinWithLabel spin ;
        SliderWithLabel slider ;
        ColourPickerWithLabel color ;

        explicit Impl(wxWindow* const self)
        : spin(self),
          slider(self),
          color(self)
        {}
    } ;

    EasyClcikCtrl::EasyClcikCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>(this))
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto root_sizer = new wxFlexGridSizer(2) ;

        pimpl->spin.set_sizer(root_sizer, flags) ;
        pimpl->slider.set_sizer(root_sizer, flags) ;
        pimpl->color.set_sizer(root_sizer, flags) ;

        pimpl->spin.create("easy_click_font_size", 5, 200, 14) ;
        pimpl->slider.create("easy_click_font_weight", 0, 1000, 500) ;
        pimpl->color.create("easy_click_font_color") ;
        pimpl->color.create("easy_click_font_bkcolor") ;
        pimpl->spin.create("easy_click_matching_color_decay", 0, 255, 100) ;

        SetSizerAndFit(root_sizer) ;
    }
    EasyClcikCtrl::~EasyClcikCtrl() noexcept = default ;

    void EasyClcikCtrl::translate() {
        pimpl->spin.translate() ;
        pimpl->slider.translate() ;
        pimpl->color.translate() ;
    }

    void EasyClcikCtrl::do_load_config() {
        pimpl->spin.load_config() ;
        pimpl->slider.load_config() ;
        pimpl->color.load_config() ;
    }

    void EasyClcikCtrl::do_load_config_default() {
        pimpl->spin.load_config_default() ;
        pimpl->slider.load_config_default() ;
        pimpl->color.load_config_default() ;
    }

    void EasyClcikCtrl::do_save_config() {
        pimpl->spin.save_config() ;
        pimpl->slider.save_config() ;
        pimpl->color.save_config() ;
    }

    const wxString EasyClcikCtrl::name() noexcept {
        return UITrans::trans("notify/preferences/settings/easy_click") ;
    }
}
