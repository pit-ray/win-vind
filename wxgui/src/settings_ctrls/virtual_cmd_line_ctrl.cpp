#include "virtual_cmd_line_ctrl.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/sizer.h>
#include "enable_gcc_warning.hpp"

#include "io_params.hpp"
#include "ui_translator.hpp"

#include "ctrl_assets/spin_wl.hpp"
#include "ctrl_assets/slider_wl.hpp"
#include "ctrl_assets/clrpicker_wl.hpp"
#include "ctrl_assets/choice_wl.hpp"
#include "ctrl_assets/text_wl.hpp"

namespace wxGUI
{
    struct VirtualCmdLineCtrl::Impl {
        SpinWithLabel spin ;
        SliderWithLabel slider ;
        ColourPickerWithLabel color ;
        ChoiceWithLabel choice ;
        TextWithLabel text ;

        explicit Impl(wxWindow* const self)
        : spin(self),
          slider(self),
          color(self),
          choice(self),
          text(self)
        {}
    } ;

    VirtualCmdLineCtrl::VirtualCmdLineCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>(this))
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto root_sizer = new wxFlexGridSizer(2) ;

        pimpl->spin.set_sizer(root_sizer, flags) ;
        pimpl->slider.set_sizer(root_sizer, flags) ;
        pimpl->color.set_sizer(root_sizer, flags) ;
        pimpl->choice.set_sizer(root_sizer, flags) ;
        pimpl->text.set_sizer(root_sizer, flags) ;

        pimpl->spin.create("cmd_max_char", 5, 1024, 32) ;
        pimpl->spin.create("cmd_max_history_num", 1, 1024, 10) ;
        pimpl->spin.create("cmd_font_size", 5, 200, 20) ;
        pimpl->slider.create("cmd_font_weight", 0, 1000, 600) ;
        pimpl->text.create("cmd_font_name") ;
        pimpl->color.create("cmd_font_color") ;
        pimpl->color.create("cmd_font_bkcolor") ;
        pimpl->spin.create("cmd_font_extra", 0, 100, 3) ;
        pimpl->choice.create("cmd_pos") ;
        pimpl->spin.create("cmd_xmargin", -500, 500, 32) ;
        pimpl->spin.create("cmd_ymargin", -500, 500, 64) ;
        pimpl->spin.create("cmd_fadeout_time", 0, 3600, 5) ;

        SetSizerAndFit(root_sizer) ;
    }
    VirtualCmdLineCtrl::~VirtualCmdLineCtrl() noexcept = default ;

    void VirtualCmdLineCtrl::translate() {
        pimpl->spin.translate() ;
        pimpl->slider.translate() ;
        pimpl->color.translate() ;
        pimpl->choice.translate() ;
        pimpl->text.translate() ;
    }

    void VirtualCmdLineCtrl::do_load_config() {
        pimpl->spin.load_config() ;
        pimpl->slider.load_config() ;
        pimpl->color.load_config() ;
        pimpl->choice.load_config() ;
        pimpl->text.load_config() ;
    }

    void VirtualCmdLineCtrl::do_load_config_default() {
        pimpl->spin.load_config_default() ;
        pimpl->slider.load_config_default() ;
        pimpl->color.load_config_default() ;
        pimpl->choice.load_config_default() ;
        pimpl->text.load_config_default() ;
    }

    void VirtualCmdLineCtrl::do_save_config() {
        pimpl->spin.save_config() ;
        pimpl->slider.save_config() ;
        pimpl->color.save_config() ;
        pimpl->choice.save_config() ;
        pimpl->text.save_config() ;
    }

    const wxString VirtualCmdLineCtrl::name() {
        return UITrans::trans("notify/preferences/settings/virtual_cmdline") ;
    }
}
