#include "wx_options.hpp"

#include <sstream>
#include <string>

#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/slider.h>
#include <wx/clrpicker.h>
#include <wx/colour.h>
#include <wx/choice.h>

#include "wx_constant.hpp"
#include "msg_logger.hpp"


namespace wxGUI
{
    namespace OptsEvt {
        enum : unsigned int {
            DEFAULT = 10300,
        } ;
    }

    namespace ChoiceTranslater {
        static const std::unordered_map<std::string, std::string> label2val {
            {"UpperLeft",  "UpperLeft"},
            {"UpperMid",   "UpperMid"},
            {"UpperRight", "UpperRight"},
            {"MidLeft",    "MidLeft"},
            {"Center",     "Center"},
            {"MidRight",   "MidRight"},
            {"LowerLeft",  "LowerLeft"},
            {"LowerMid",   "LowerMid"},
            {"LowerRight", "LowerRight"}
        } ;
    }

    struct OptionsPanel::Impl
    {
        ums_t<wxSpinCtrl*>         sc_params{} ;
        ums_t<wxSlider*>           sl_params{} ;
        ums_t<wxColourPickerCtrl*> cp_params{} ;
        ums_t<wxChoice*>           ch_params{} ;

        ums_t<wxCheckBox*> opts{} ;
    } ;

    OptionsPanel::OptionsPanel(wxBookCtrlBase* const p_book_ctrl)
    : wxPanel(p_book_ctrl),
      pimpl(std::make_unique<Impl>())
    {
        p_book_ctrl->AddPage(this, wxT("Options")) ;

        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto opts_sizer_wrapper = new wxBoxSizer(wxVERTICAL) ;

        auto opts_sizer = new wxBoxSizer(wxHORIZONTAL) ;
        {
            //Option Checks
            {
                auto check_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Options")) ;
                auto check_sizer = new wxBoxSizer(wxVERTICAL) ;

                auto add_cb = [this, &flags, &check_sizer, &c = pimpl->opts](auto id, auto label) {
                    c[id] = new wxCheckBox(this, wxID_ANY, label) ;
                    check_sizer->Add(c[id], flags) ;
                } ;

                add_cb("autotrack_popup", wxT("Autotrack Popup")) ;
                add_cb("display_cmd", wxT("Display Commands")) ;

                check_sizer_wrapper->Add(check_sizer, flags) ;

                opts_sizer->Add(check_sizer_wrapper, flags) ;

                opts_sizer_wrapper->Add(opts_sizer, flags) ;
            }

            //Display Commands
            {
                auto cmd_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Display Commands")) ;
                auto cmd_sizer = new wxFlexGridSizer(2) ;

                auto add_cp = [this, &flags, &cmd_sizer, &c = pimpl->cp_params](auto id) {
                    c[id] = new wxColourPickerCtrl(this, wxID_ANY) ;
                    cmd_sizer->Add(c[id], flags) ;
                } ;
                auto add_st = [this, &flags, &cmd_sizer](auto&& text) {
                    cmd_sizer->Add(new wxStaticText(this, wxID_ANY, std::forward<decltype(text)>(text)), flags) ;
                } ;
                auto add_sc = [this, &flags, &cmd_sizer, &c = pimpl->sc_params](auto id) {
                    c[id] = new wxSpinCtrl(this, wxID_ANY) ;
                    cmd_sizer->Add(c[id], flags) ;
                } ;

                add_st(wxT("Font Size")) ;
                add_sc("cmd_font_size") ;

                add_st(wxT("Font Weight")) ;
                auto& rcfw = pimpl->sl_params["cmd_font_weight"] = new wxSlider(this, wxID_ANY, 600, 0, 1000) ;
                cmd_sizer->Add(rcfw, flags) ;

                add_st(wxT("Font Color")) ;
                add_cp("cmd_font_color") ;

                add_st(wxT("Font Background Color")) ;
                add_cp("cmd_font_bkcolor") ;

                add_st(wxT("Font Extra")) ;
                add_sc("cmd_font_extra") ;

                add_st(wxT("Max Char")) ;
                add_sc("cmd_max_char") ;

                add_st(wxT("Position")) ;
                wxArrayString cmp_items ;
                cmp_items.Add(wxT("UpperLeft")) ;
                cmp_items.Add(wxT("UpperMid")) ;
                cmp_items.Add(wxT("UpperRight")) ;
                cmp_items.Add(wxT("MidLeft")) ;
                cmp_items.Add(wxT("Center")) ;
                cmp_items.Add(wxT("MidRight")) ;
                cmp_items.Add(wxT("LowerLeft")) ;
                cmp_items.Add(wxT("LowerMid")) ;
                cmp_items.Add(wxT("LowerRight")) ;
                auto& rch = pimpl->ch_params["cmd_pos"] = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, cmp_items) ;
                cmd_sizer->Add(rch, flags) ;

                add_st(wxT("Position X Margin")) ;
                add_sc("cmd_xmargin") ;

                add_st(wxT("Position Y Margin")) ;
                add_sc("cmd_ymargin") ;

                cmd_sizer_wrapper->Add(cmd_sizer, flags) ;
                opts_sizer->Add(cmd_sizer_wrapper, flags) ;
            }

            opts_sizer_wrapper->AddStretchSpacer() ;

            auto defbtn_flags = flags ;
            defbtn_flags.Align(wxALIGN_RIGHT) ;
            opts_sizer_wrapper->Add(new wxButton(this,
                OptsEvt::DEFAULT, wxT("Return to Default")), defbtn_flags) ;

            SetSizerAndFit(opts_sizer_wrapper) ;
            load_all() ;

            Bind(wxEVT_BUTTON, [this](auto&) {
                load_default() ;
            }, OptsEvt::DEFAULT) ;
        }
    }

    OptionsPanel::~OptionsPanel() = default ;

    void OptionsPanel::load_core(const PrefParser::ums_str_t params, const PrefParser::ums_bool_t flags) {
        auto catch_except = [](auto& e, auto& index) {
            Logger::error_stream << Logger::E << e.what() << ": " \
            << index << "is invalid index. (OptionsPanel::load_core)\n" ;
        } ;

        //params
        //wxSpinCtrl
        for(auto& p : pimpl->sc_params) {
            try {p.second->SetValue(params.at(p.first)) ;}
            catch(std::out_of_range& e) {
                catch_except(e, p.first) ;
                continue ;
            }
        }

        //wxSlider
        for(auto& p : pimpl->sl_params) {
            try {p.second->SetValue(std::stoi(params.at(p.first))) ;}
            catch(std::out_of_range& e) {
                catch_except(e, p.first) ;
                continue ;
            }
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

        //wxColourPickerCtrl
        for(auto& p : pimpl->cp_params) {
            try {p.second->SetColour(hex2wxColour(params.at(p.first))) ;}
            catch(std::out_of_range& e) {
                catch_except(e, p.first) ;
                continue ;
            }
        }

        //wxChoice
        for(auto& p : pimpl->ch_params) {
            try {
                //ini's key
                const auto key = params.at(p.first) ;

                for(const auto& trs : ChoiceTranslater::label2val) {
                    //linear-search same key label
                    if(key != trs.second) {
                        continue ;
                    }

                    const auto index = p.second->FindString(trs.first) ;

                    if(index != wxNOT_FOUND) {
                        p.second->SetSelection(index) ;
                    }
                    break ;
                }

            }
            catch(std::out_of_range& e) {
                catch_except(e, p.first) ;
                continue ;
            }
        }

        //opts
        //wxCheckBox
        for(auto& p : pimpl->opts) {
            try {p.second->SetValue(flags.at(p.first)) ;}
            catch(std::out_of_range& e) {
                catch_except(e, p.first) ;
                continue ;
            }
        }
    }

    void OptionsPanel::load_default() {
        load_core(
            PrefParser::load_default_opts_params(),
            PrefParser::load_default_opts_bool()
        ) ;
    }

    void OptionsPanel::load_all() {
        load_core(
            PrefParser::load_opts_params(),
            PrefParser::load_opts_bool()
        ) ;
    }

    void OptionsPanel::save_all() {
        using namespace PrefParser ;
        ums_str_t params{} ;
        ums_bool_t flags{} ;

        //params
        //wxSpinCtrl
        for(const auto& p : pimpl->sc_params) {
            params[p.first] = std::to_string(p.second->GetValue())  ;
        }

        //wxSlider
        for(const auto& p : pimpl->sl_params) {
            params[p.first] = std::to_string(p.second->GetValue()) ;
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
        for(const auto& p : pimpl->cp_params) {
            const auto col = p.second->GetColour() ;
            params[p.first] = to_hex(col.Red()) + to_hex(col.Green()) + to_hex(col.Blue()) ;
        }

        //wxChoice
        for(const auto& p : pimpl->ch_params) {
            const auto index = p.second->GetSelection() ;
            try {
                params[p.first] = ChoiceTranslater::label2val.at(p.second->GetString(index).ToStdString()) ;
            }
            catch(std::out_of_range& e) {
                Logger::error_stream << Logger::E << e.what() << ": failed save " \
                << p.second->GetString(index) << " (OptionsPanel::save_core)\n" ;
                continue ;
            }
        }

        //opts
        //wxCheckBox
        for(const auto& p : pimpl->opts) {
            flags[p.first] = p.second->GetValue() ;
        }

        save_opts_params(params) ;
        save_opts_bool(flags) ;
    }
}