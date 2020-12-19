#include "easy_click_ctrl.hpp"

#include <unordered_map>
#include <sstream>

#include "disable_gcc_warning.hpp"
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include <wx/colour.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include "enable_gcc_warning.hpp"

#include "wx_constant.hpp"
#include "io_params.hpp"
#include "ui_translator.hpp"
#include "utility.hpp"

namespace wxGUI
{
    struct EasyClcikCtrl::Impl {
        std::unordered_map<std::string, wxSpinCtrl*> scs ;
        std::unordered_map<std::string, wxColourPickerCtrl*> cpcs ;
        std::unordered_map<std::string, wxSlider*> sls ;
        std::unordered_map<std::string, wxStaticText*> lbs ;

        explicit Impl(wxWindow* const UNUSED(self))
        : scs(),
          cpcs(),
          sls(),
          lbs()
        {}

        void update_labels() noexcept {
            for(auto& l : lbs) {
                l.second->SetLabel(ioParams::get_label(l.first)) ;
            }
        }

        template <typename T1, typename T2>
        void load_config_common(const T1& get_vi_func, const T2& get_vs_func) noexcept {
            for(auto& p : scs) {
                p.second->SetValue(get_vi_func(p.first)) ;
            }

            for(auto& p : sls) {
                p.second->SetValue(get_vi_func(p.first)) ;
            }

            const auto hex2wxColour = [](auto hex) {
                auto [r, g, b] = Utility::hex2rgb(hex) ;
                return wxColour(r, g, b) ;
            } ;
            for(auto& p : cpcs) {
                p.second->SetColour(hex2wxColour(get_vs_func(p.first))) ;
            }
        }
    } ;

    EasyClcikCtrl::EasyClcikCtrl(wxWindow* parent, wxWindowID id)
    : CtrlCore(parent, id),
      pimpl(std::make_unique<Impl>(this))
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto root_sizer = new wxFlexGridSizer(2) ;

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

        auto create_cp = [this, &flags, root_sizer](const auto name) {
            pimpl->lbs[name] = new wxStaticText(this, wxID_ANY, name) ;
            root_sizer->Add(pimpl->lbs[name], flags) ;

            pimpl->cpcs[name] = new wxColourPickerCtrl(this, wxID_ANY) ;
            root_sizer->Add(pimpl->cpcs[name], flags) ;
        } ;

        auto create_sl = [this, &flags, root_sizer](const auto name, const auto min, const auto max, const auto init) {
            pimpl->lbs[name] = new wxStaticText(this, wxID_ANY, name) ;
            root_sizer->Add(pimpl->lbs[name], flags) ;

            pimpl->sls[name] = new wxSlider(
                this, wxID_ANY, init, min, max,
                wxDefaultPosition, wxSize(CTRL_WIDTH, wxDefaultCoord)
            ) ;
            root_sizer->Add(pimpl->sls[name], flags) ;
        } ;

        create_sc("easy_click_font_size", 5, 200, 14) ;
        create_sl("easy_click_font_weight", 0, 1000, 500) ;
        create_cp("easy_click_font_color") ;
        create_cp("easy_click_font_bkcolor") ;
        create_sc("easy_click_matching_color_decay", 0, 255, 100) ;

        SetSizerAndFit(root_sizer) ;
    }
    EasyClcikCtrl::~EasyClcikCtrl() noexcept = default ;

    void EasyClcikCtrl::translate() noexcept {
        pimpl->update_labels() ;
    }

    void EasyClcikCtrl::do_load_config() noexcept {
        pimpl->load_config_common(ioParams::get_vi, ioParams::get_vs) ;
    }

    void EasyClcikCtrl::do_load_config_default() noexcept {
        pimpl->load_config_common(ioParams::Default::get_vi, ioParams::Default::get_vs) ;
    }

    void EasyClcikCtrl::do_save_config() noexcept {
        for(const auto& p : pimpl->scs) {
            ioParams::set(p.first, p.second->GetValue()) ;
        }
        for(const auto& p : pimpl->sls) {
            ioParams::set(p.first, p.second->GetValue()) ;
        }

        auto to_hex = [](unsigned char val) {
            std::stringstream ss ;
            ss << std::hex << static_cast<int>(val) ;
            if(val == 0) {
                return "0" + ss.str() ;
            }
            return ss.str() ;
        } ;

        //wxColourPickerCtrl
        for(const auto& p : pimpl->cpcs) {
            const auto col = p.second->GetColour() ;
            auto&& chex = to_hex(col.Red()) + to_hex(col.Green()) + to_hex(col.Blue()) ;
            ioParams::set(p.first, std::move(chex)) ;
        }
    }

    const wxString EasyClcikCtrl::name() noexcept {
        return UITrans::trans("notify/preferences/settings/easy_click") ;
    }
}
