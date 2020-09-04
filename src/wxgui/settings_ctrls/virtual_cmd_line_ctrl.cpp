#include "virtual_cmd_line_ctrl.hpp"

#include <unordered_map>
#include <sstream>

#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include <wx/colour.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "choices_manager.hpp"

#include "wx_constant.hpp"
#include "io_params.hpp"
#include "ui_translator.hpp"

namespace wxGUI
{
    struct VirtualCmdLineCtrl::Impl {
        std::unordered_map<std::string, wxSpinCtrl*> scs ;
        std::unordered_map<std::string, wxColourPickerCtrl*> cpcs ;
        std::unordered_map<std::string, wxSlider*> sls ;
        ChoicesManager chm ;
        std::unordered_map<std::string, wxStaticText*> lbs ;

        explicit Impl(wxWindow* const self)
        : scs(),
          cpcs(),
          sls(),
          chm(self),
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
                if(hex.front() == '#') {
                    hex.erase(0, 1) ;
                }
                unsigned char r = 0 ;
                unsigned char g = 0 ;
                unsigned char b = 0 ;

                if(hex.length() == 6) {
                    auto r_hex = hex.substr(0, 2) ;
                    auto g_hex = hex.substr(2, 2) ;
                    auto b_hex = hex.substr(4, 2) ;
                    r = static_cast<unsigned char>(strtol(r_hex.c_str(), nullptr, 16)) ;
                    g = static_cast<unsigned char>(strtol(g_hex.c_str(), nullptr, 16)) ;
                    b = static_cast<unsigned char>(strtol(b_hex.c_str(), nullptr, 16)) ;
                }

                return wxColour(r, g, b) ;
            } ;
            for(auto& p : cpcs) {
                p.second->SetColour(hex2wxColour(get_vs_func(p.first))) ;
            }
        }

        ~Impl() noexcept = default ;
        Impl(Impl&&)                 = delete ;
        Impl& operator=(Impl&&)      = delete ;
        Impl(const Impl&)            = delete ;
        Impl& operator=(const Impl&) = delete ;
    } ;

    VirtualCmdLineCtrl::VirtualCmdLineCtrl(wxWindow* parent, wxWindowID id)
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
        auto create_ch = [this, root_sizer, &flags](const auto obj_name) {
            pimpl->lbs[obj_name] = new wxStaticText(this, wxID_ANY, obj_name) ;
            root_sizer->Add(pimpl->lbs[obj_name], flags) ;

            root_sizer->Add(pimpl->chm.create(obj_name), flags) ;
        } ;

        create_sc("cmd_max_char", 5, 1024, 32) ;
        create_sc("cmd_max_history_num", 1, 1024, 10) ;
        create_sc("cmd_font_size", 5, 200, 20) ;
        create_sl("cmd_font_weight", 0, 1000, 600) ;
        create_cp("cmd_font_color") ;
        create_cp("cmd_font_bkcolor") ;
        create_sc("cmd_font_extra", 0, 100, 3) ;
        create_ch("cmd_pos") ;
        create_sc("cmd_xmargin", -500, 500, 32) ;
        create_sc("cmd_ymargin", -500, 500, 64) ;

        SetSizerAndFit(root_sizer) ;
    }
    VirtualCmdLineCtrl::~VirtualCmdLineCtrl() noexcept = default ;

    void VirtualCmdLineCtrl::translate() noexcept {
        pimpl->update_labels() ;
        pimpl->chm.update_config() ;
    }

    void VirtualCmdLineCtrl::do_load_config() noexcept {
        pimpl->load_config_common(ioParams::get_vi, ioParams::get_vs) ;
        pimpl->chm.load_config() ;
    }

    void VirtualCmdLineCtrl::do_load_config_default() noexcept {
        pimpl->load_config_common(ioParams::Default::get_vi, ioParams::Default::get_vs) ;
        pimpl->chm.load_config_default() ;
    }

    void VirtualCmdLineCtrl::do_save_config() noexcept {
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

        pimpl->chm.save_config() ;
    }

    const wxString VirtualCmdLineCtrl::name() noexcept {
        return UITrans::trans("notify/preferences/settings/virtual_cmdline") ;
    }
}