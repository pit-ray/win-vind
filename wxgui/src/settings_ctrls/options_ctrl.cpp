#include "options_ctrl.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include "enable_gcc_warning.hpp"

#include "io_params.hpp"
#include "ui_translator.hpp"

namespace wxGUI
{
    struct OptionsCtrl::Impl {
        std::unordered_map<std::string, wxCheckBox*> cbs ;

        explicit Impl() : cbs() {}
        void update_checkbox() noexcept {
            for(auto& c : cbs) {
                c.second->SetLabel(ioParams::get_label(c.first)) ;
            }
        }

        template <typename T>
        void load_config_core(const T& get_vb_func) noexcept {
            for(auto& c : cbs) {
                c.second->SetValue(get_vb_func(c.first)) ;
            }
        }

        ~Impl() noexcept = default ;
        Impl(Impl&&)                 = delete ;
        Impl& operator=(Impl&&)      = delete ;
        Impl(const Impl&)            = delete ;
        Impl& operator=(const Impl&) = delete ;
    } ;

    OptionsCtrl::OptionsCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>())
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;
        auto root_sizer = new wxBoxSizer(wxVERTICAL) ;

        auto create_cb = [this, &flags, root_sizer](const auto name) {
            pimpl->cbs[name] = new wxCheckBox(this, wxID_ANY, name) ;
            root_sizer->Add(pimpl->cbs[name], flags) ;
        } ;

        create_cb("autotrack_popup") ;
        create_cb("dedicate_to_window") ;
        create_cb("enable_char_cache") ;
        create_cb("enable_esyclk_hwnd_cache") ;
        create_cb("suppress_for_vim") ;
        create_cb("virtual_cmd_line") ;

        SetSizerAndFit(root_sizer) ;
    }
    OptionsCtrl::~OptionsCtrl() noexcept = default ;

    void OptionsCtrl::translate() noexcept {
        pimpl->update_checkbox() ;
    }

    void OptionsCtrl::do_load_config() noexcept {
        pimpl->load_config_core(ioParams::get_vb) ;
    }

    void OptionsCtrl::do_load_config_default() noexcept {
        pimpl->load_config_core(ioParams::Default::get_vb) ;
    }

    void OptionsCtrl::do_save_config() noexcept {
        for(const auto& c : pimpl->cbs) {
            ioParams::set(c.first, c.second->GetValue()) ;
        }
    }

    const wxString OptionsCtrl::name() noexcept {
        return UITrans::trans("notify/preferences/settings/options") ;
    }
}
