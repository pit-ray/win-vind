#include "common_ctrl.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/sizer.h>
#include "enable_gcc_warning.hpp"

#include "io_params.hpp"
#include "ui_translator.hpp"

#include "ctrl_assets/spin_wl.hpp"
#include "ctrl_assets/choice_wl.hpp"
#include "ctrl_assets/text_wl.hpp"
#include "ctrl_assets/filepicker_wl.hpp"

namespace wxGUI
{
    struct CommonCtrl::Impl {
        SpinWithLabel spin ;
        ChoiceWithLabel choice ;
        TextWithLabel text ;
        FilePickerWithLabel file ;

        explicit Impl(wxWindow* const self)
        : spin(self),
          choice(self),
          text(self),
          file(self)
        {}
    } ;

    CommonCtrl::CommonCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>(this))
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto root_sizer = new wxFlexGridSizer(2) ;

        pimpl->choice.set_sizer(root_sizer, flags) ;
        pimpl->spin.set_sizer(root_sizer, flags) ;
        pimpl->text.set_sizer(root_sizer, flags) ;
        pimpl->file.set_sizer(root_sizer, flags) ;

        pimpl->choice.create("ui_lang") ;
        pimpl->choice.create("icon_style") ;
        pimpl->choice.create("kb_type") ;
        pimpl->choice.create("initial_mode") ;
        pimpl->file.create("gvim_exe_path") ;
        pimpl->spin.create("gui_font_size", 2, 20, 9) ;
        pimpl->text.create("gui_font_name") ;

        SetSizerAndFit(root_sizer) ;
    }

    CommonCtrl::~CommonCtrl() noexcept = default ;

    void CommonCtrl::translate() {
        pimpl->spin.translate() ;
        pimpl->choice.translate() ;
        pimpl->text.translate() ;
        pimpl->file.translate() ;
    }

    void CommonCtrl::do_load_config() {
        pimpl->spin.load_config() ;
        pimpl->choice.load_config() ;
        pimpl->text.load_config() ;
        pimpl->file.load_config() ;
    }

    void CommonCtrl::do_load_config_default() {
        pimpl->spin.load_config_default() ;
        pimpl->choice.load_config_default() ;
        pimpl->text.load_config_default() ;
        pimpl->file.load_config_default() ;
    }

    void CommonCtrl::do_save_config() {
        pimpl->spin.save_config() ;
        pimpl->choice.save_config() ;
        pimpl->text.save_config() ;
        pimpl->file.save_config() ;
    }

    const wxString CommonCtrl::name() {
        return UITrans::trans("notify/preferences/settings/common") ;
    }

}
