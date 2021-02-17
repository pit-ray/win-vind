#include "resize_window_ctrl.hpp"

#include "disable_gcc_warning.hpp"
#include <memory>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include "enable_gcc_warning.hpp"

#include "io_params.hpp"
#include "ui_translator.hpp"

namespace wxGUI
{
    struct ResizeWindowCtrl::Impl {
        std::unordered_map<std::string, wxSpinCtrl*> scs{} ;
        std::unordered_map<std::string, wxStaticText*> lbs{} ;

        void update_labels() noexcept {
            for(auto& l : lbs) {
                l.second->SetLabel(ioParams::get_label(l.first)) ;
            }
        }
    } ;

    ResizeWindowCtrl::ResizeWindowCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>())
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto root_sizer = new wxFlexGridSizer(2) ;

        auto create_sc = [this, &flags, root_sizer](
                const auto name,
                const auto min,
                const auto max,
                const auto init) {
            pimpl->lbs[name] = new wxStaticText(this, wxID_ANY, name) ;
            root_sizer->Add(pimpl->lbs[name], flags) ;

            pimpl->scs[name] = new wxSpinCtrl(
                this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                wxSize(CTRL_WIDTH, -1),
                wxSP_ARROW_KEYS, min, max, init
            ) ;
            root_sizer->Add(pimpl->scs[name], flags) ;
        } ;

        create_sc("window_width_delta", 1, 10000, 10) ;
        create_sc("window_height_delta", 1, 10000, 10) ;

        SetSizerAndFit(root_sizer) ;
    }
    ResizeWindowCtrl::~ResizeWindowCtrl() noexcept = default ;

    void ResizeWindowCtrl::translate() noexcept {
        pimpl->update_labels() ;
    }

    void ResizeWindowCtrl::do_load_config() noexcept {
        for(auto& p : pimpl->scs) {
            p.second->SetValue(ioParams::get_vi(p.first)) ;
        }
    }

    void ResizeWindowCtrl::do_load_config_default() noexcept {
        for(auto& p : pimpl->scs) {
            p.second->SetValue(ioParams::Default::get_vi(p.first)) ;
        }
    }

    void ResizeWindowCtrl::do_save_config() noexcept {
        for(const auto& p : pimpl->scs) {
            ioParams::set(p.first, p.second->GetValue()) ;
        }
    }

    const wxString ResizeWindowCtrl::name() noexcept {
        return UITrans::trans("notify/preferences/settings/resize_window") ;
    }

}
