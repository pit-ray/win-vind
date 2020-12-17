#include "common_ctrl.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "enable_gcc_warning.hpp"

#include "choices_manager.hpp"

#include "io_params.hpp"
#include "ui_translator.hpp"

namespace wxGUI
{
    struct CommonCtrl::Impl {
        ChoicesManager chm ;
        std::unordered_map<std::string, wxSpinCtrl*> scs ;
        std::unordered_map<std::string, wxStaticText*> lbs ;

        explicit Impl(wxWindow* const self)
        : chm(self),
          scs(),
          lbs()
        {}

        void update_labels() noexcept {
            for(auto& l : lbs) {
                l.second->SetLabel(ioParams::get_label(l.first)) ;
            }
        }

        template <typename T>
        void load_config_common(const T& get_vi_func) noexcept {
            for(auto& p : scs) {
                p.second->SetValue(get_vi_func(p.first)) ;
            }
        }

        ~Impl() noexcept = default ;
        Impl(Impl&&)                 = delete ;
        Impl& operator=(Impl&&)      = delete ;
        Impl(const Impl&)            = delete ;
        Impl& operator=(const Impl&) = delete ;
    } ;

    CommonCtrl::CommonCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>(this))
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER).Expand() ;

        auto root_sizer = new wxFlexGridSizer(2) ;

        auto create_ch = [this, root_sizer, &flags](const auto obj_name) {
            pimpl->lbs[obj_name] = new wxStaticText(this, wxID_ANY, obj_name) ;
            root_sizer->Add(pimpl->lbs[obj_name], flags) ;

            root_sizer->Add(pimpl->chm.create(obj_name), flags) ;
        } ;
        auto create_sc = [this, &flags, root_sizer](const auto name, const auto min, const auto max, const auto init) {
            pimpl->lbs[name] = new wxStaticText(this, wxID_ANY, name) ;
            root_sizer->Add(pimpl->lbs[name], flags) ;

            pimpl->scs[name] = new wxSpinCtrl(
                this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                wxSize(CTRL_WIDTH, wxDefaultCoord),
                wxSP_ARROW_KEYS, min, max, init
            ) ;
            root_sizer->Add(pimpl->scs[name], flags) ;
        } ;

        create_ch("ui_lang") ;
        create_ch("icon_style") ;
        create_ch("kb_type") ;
        create_sc("ppi_factor", 1, 12, 6) ;
        create_ch("initial_mode") ;
        SetSizerAndFit(root_sizer) ;
    }
    CommonCtrl::~CommonCtrl() noexcept = default ;

    void CommonCtrl::translate() noexcept {
        pimpl->update_labels() ;
        pimpl->chm.update_config() ;
    }

    void CommonCtrl::do_load_config() noexcept {
        pimpl->load_config_common(ioParams::get_vi) ;
        pimpl->chm.load_config() ;
    }

    void CommonCtrl::do_load_config_default() noexcept {
        pimpl->load_config_common(ioParams::Default::get_vi) ;
        pimpl->chm.load_config_default() ;
    }

    void CommonCtrl::do_save_config() noexcept {
        pimpl->chm.save_config() ;
        for(const auto& p : pimpl->scs) {
            ioParams::set(p.first, p.second->GetValue()) ;
        }
    }

    const wxString CommonCtrl::name() noexcept {
        return UITrans::trans("notify/preferences/settings/common") ;
    }

}
