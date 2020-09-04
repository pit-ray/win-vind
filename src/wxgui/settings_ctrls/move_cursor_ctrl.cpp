#include "move_cursor_ctrl.hpp"

#include <wx/spinctrl.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

#include "io_params.hpp"
#include "ui_translator.hpp"

namespace wxGUI
{
    struct MoveCursorCtrl::Impl {
        std::unordered_map<std::string, wxSlider*> sls ;
        std::unordered_map<std::string, wxSpinCtrl*> scsint ;

        std::unordered_map<std::string, wxStaticText*> lbs ;

        explicit Impl() : sls(), scsint(), lbs() {}

        void update_labels() noexcept {
            for(auto& l : lbs) {
                l.second->SetLabel(ioParams::get_label(l.first)) ;
            }
        }

        template <typename T>
        void load_config_core(const T& get_vi_func) noexcept {
            for(auto& s : sls) {
                s.second->SetValue(get_vi_func(s.first)) ;
            }
            for(auto& s : scsint) {
                s.second->SetValue(get_vi_func(s.first)) ;
            }
        }

        ~Impl() noexcept = default ;
        Impl(Impl&&)                 = delete ;
        Impl& operator=(Impl&&)      = delete ;
        Impl(const Impl&)            = delete ;
        Impl& operator=(const Impl&) = delete ;
    } ;

    MoveCursorCtrl::MoveCursorCtrl(wxWindow* parent, wxWindowID id)
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

        auto create_scint = [this, &flags, &root_sizer](const auto name, const auto min, const auto max, const auto init) {
            pimpl->lbs[name] = new wxStaticText(this, wxID_ANY, name) ;
            root_sizer->Add(pimpl->lbs[name], flags) ;

            pimpl->scsint[name] = new wxSpinCtrl(
                this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                wxSize(CTRL_WIDTH, wxDefaultCoord),
                wxSP_ARROW_KEYS, min, max, init
            ) ;
            root_sizer->Add(pimpl->scsint[name], flags) ;
        } ;

        create_scint("screen_pos_buf", 0, 128, 10) ;
        create_sl("cursor_acceleration", 1, 128, 64) ;
        create_sl("cursor_weight", 1, 1024, 512) ;
        create_sl("cursor_max_velocity", 1, 20, 10) ;

        SetSizerAndFit(root_sizer) ;
    }
    MoveCursorCtrl::~MoveCursorCtrl() noexcept = default ;

    void MoveCursorCtrl::translate() noexcept {
        pimpl->update_labels() ;
    }

    void MoveCursorCtrl::do_load_config() noexcept {
        pimpl->load_config_core(ioParams::get_vi) ;
    }

    void MoveCursorCtrl::do_load_config_default() noexcept {
        pimpl->load_config_core(ioParams::Default::get_vi) ;
    }

    void MoveCursorCtrl::do_save_config() noexcept {
        for(const auto& s : pimpl->sls) {
            ioParams::set(s.first, s.second->GetValue()) ;
        }
        for(const auto& s : pimpl->scsint) {
            ioParams::set(s.first, s.second->GetValue()) ;
        }
    }

    const wxString MoveCursorCtrl::name() noexcept {
        return UITrans::trans("notify/preferences/settings/move_cursor") ;
    }
}