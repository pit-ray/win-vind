#include "scroll_page_ctrl.hpp"

#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

#include "io_params.hpp"
#include "ui_translator.hpp"

namespace wxGUI
{
    struct  ScrollPageCtrl::Impl {
        std::unordered_map<std::string, wxSlider*> sls{} ;
        std::unordered_map<std::string, wxSpinCtrlDouble*> scsdbl{} ;

        std::unordered_map<std::string, wxStaticText*> lbs{} ;

        explicit Impl() : sls(), scsdbl(), lbs() {}

        void update_labels() noexcept {
            for(auto& l : lbs) {
                l.second->SetLabel(ioParams::get_label(l.first)) ;
            }
        }

        template <typename T1, typename T2>
        void load_config_core(const T1& get_vi_func, const T2& get_vd_func) noexcept {
            for(auto& s : sls) {
                s.second->SetValue(get_vi_func(s.first)) ;
            }
            for(auto& s : scsdbl) {
                s.second->SetValue(get_vd_func(s.first)) ;
            }
        }

        ~Impl() noexcept = default ;
        Impl(Impl&&)                 = delete ;
        Impl& operator=(Impl&&)      = delete ;
        Impl(const Impl&)            = delete ;
        Impl& operator=(const Impl&) = delete ;
    } ;

    ScrollPageCtrl::ScrollPageCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>())
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER).Expand() ;
        auto root_sizer = new wxFlexGridSizer(2) ;

        auto create_sl = [this, &flags, &root_sizer](const auto name, const auto min, const auto max, const auto init) {
            pimpl->lbs[name] = new wxStaticText(this, wxID_ANY, name) ;
            root_sizer->Add(pimpl->lbs[name], flags) ;

            pimpl->sls[name] = new wxSlider(
                this, wxID_ANY, init, min, max,
                wxDefaultPosition, wxSize(CTRL_WIDTH, wxDefaultCoord)
            ) ;
            root_sizer->Add(pimpl->sls[name], flags) ;
        } ;

        auto create_ratio = [this, &flags, &root_sizer](const auto name, const auto init) {
            pimpl->lbs[name] = new wxStaticText(this, wxID_ANY, name) ;
            root_sizer->Add(pimpl->lbs[name], flags) ;

            pimpl->scsdbl[name] = new wxSpinCtrlDouble(
                this, wxID_ANY,
                wxEmptyString, wxDefaultPosition,
                wxSize(CTRL_WIDTH, wxDefaultCoord),
                wxSP_ARROW_KEYS, 0.0, 1.0, init, 0.001
            ) ;
            root_sizer->Add(pimpl->scsdbl[name], flags) ;
        } ;

        create_sl("yscroll_speed", 1, 10, 4) ;
        create_sl("xscroll_speed", 1, 10, 4) ;
        create_ratio("yscroll_screen_ratio", 0.125) ;
        create_ratio("xscroll_screen_ratio", 0.125) ;

        SetSizerAndFit(root_sizer) ;
    }
    ScrollPageCtrl::~ScrollPageCtrl() noexcept = default ;

    void ScrollPageCtrl::translate() noexcept {
        pimpl->update_labels() ;
    }

    void ScrollPageCtrl::do_load_config() noexcept {
        pimpl->load_config_core(ioParams::get_vi, ioParams::get_vd) ;
    }

    void ScrollPageCtrl::do_load_config_default() noexcept {
        pimpl->load_config_core(ioParams::Default::get_vi, ioParams::Default::get_vd) ;
    }

    void ScrollPageCtrl::do_save_config() noexcept {
        for(const auto& s : pimpl->sls) {
            ioParams::set(s.first, s.second->GetValue()) ;
        }
        for(const auto& s : pimpl->scsdbl) {
            ioParams::set(s.first, s.second->GetValue()) ;
        }
    }

    const wxString ScrollPageCtrl::name() noexcept {
        return UITrans::trans("notify/preferences/settings/scroll_page") ;
    }
}
