#include "wx_options.hpp"

#include <sstream>
#include <string>
#include <map>

#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/slider.h>
#include <wx/clrpicker.h>
#include <wx/colour.h>
#include <wx/choice.h>

#include "wx_constant.hpp"
#include "msg_logger.hpp"
#include "ui_translator.hpp"


namespace wxGUI
{
    namespace OptsEvt {
        enum : unsigned int {
            DEFAULT = 10300,
        } ;
    }

    namespace ChoiceTranslator {
        using namespace UITrans ;
        static const std::map<std::string, wxString> label2val {
            {"UpperLeft",  trans(Label::Pref_Options_DispCmds_Pos_UpperLeft)},
            {"UpperMid",   trans(Label::Pref_Options_DispCmds_Pos_UpperMid)},
            {"UpperRight", trans(Label::Pref_Options_DispCmds_Pos_UpperRight)},
            {"MidLeft",    trans(Label::Pref_Options_DispCmds_Pos_MidLeft)},
            {"Center",     trans(Label::Pref_Options_DispCmds_Pos_Center)},
            {"MidRight",   trans(Label::Pref_Options_DispCmds_Pos_MidRight)},
            {"LowerLeft",  trans(Label::Pref_Options_DispCmds_Pos_LowerLeft)},
            {"LowerMid",   trans(Label::Pref_Options_DispCmds_Pos_LowerMid)},
            {"LowerRight", trans(Label::Pref_Options_DispCmds_Pos_LowerRight)}
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
        using namespace UITrans ;
        p_book_ctrl->AddPage(this, trans(Label::Pref_Options)) ;

        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        //this wrapper is needed to add a default button.
        auto opts_sizer_wrapper = new wxBoxSizer(wxVERTICAL) ;

        auto opts_sizer = new wxBoxSizer(wxHORIZONTAL) ;
        {
            //Option Checks
            {
                auto check_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, trans(Label::Pref_Options_Opts)) ;
                auto check_sizer = new wxBoxSizer(wxVERTICAL) ;

                auto add_cb = [this, &flags, &check_sizer](const auto id) {
                    pimpl->opts[id] = new wxCheckBox(this, wxID_ANY, trans(id)) ;
                    check_sizer->Add(pimpl->opts[std::move(id)], flags) ;
                } ;

                add_cb("autotrack_popup") ;
                add_cb("display_cmd") ;
                add_cb("dedicate_to_window") ;

                check_sizer_wrapper->Add(check_sizer, flags) ;
                opts_sizer->Add(check_sizer_wrapper, flags) ;
                opts_sizer_wrapper->Add(opts_sizer, flags) ;
            }

            //Display Commands Preferences
            {
                auto cmd_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, trans(Label::Pref_Options_DispCmds)) ;
                auto cmd_sizer = new wxFlexGridSizer(2) ;

                //text is longer than 15 characters, so not apply SSO.
                auto add_st = [this, &flags, &cmd_sizer](auto&& text) {
                    cmd_sizer->Add(new wxStaticText(this, wxID_ANY, std::forward<decltype(text)>(text)), flags) ;
                } ;
                auto add_cp = [this, &flags, &cmd_sizer, &add_st](auto id) {
                    add_st(trans(id)) ;
                    pimpl->cp_params[id] = new wxColourPickerCtrl(this, wxID_ANY) ;
                    cmd_sizer->Add(pimpl->cp_params[id], flags) ;
                } ;
                auto add_sc = [this, &flags, &cmd_sizer, &add_st](auto id, const auto min, const auto max, const auto init) {
                    add_st(trans(id)) ;
                    pimpl->sc_params[id] = new wxSpinCtrl(
                        this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                        wxDefaultSize, wxSP_ARROW_KEYS, min, max, init
                    ) ;
                    cmd_sizer->Add(pimpl->sc_params[id], flags) ;
                } ;

                auto add_sl = [this, &flags, &cmd_sizer, &add_st](const auto id, const auto min, const auto max, const auto init) {
                    add_st(trans(id)) ;
                    pimpl->sl_params[id] = new wxSlider(this, wxID_ANY, init, min, max) ;
                    cmd_sizer->Add(pimpl->sl_params[id], flags) ;
                } ;

                add_sc("cmd_font_size", 5, 200, 20) ;
                add_sl("cmd_font_weight", 0, 1000, 600) ;
                add_cp("cmd_font_color") ;
                add_cp("cmd_font_bkcolor") ;
                add_sc("cmd_font_extra", 0, 100, 3) ;

                add_st(trans("cmd_pos")) ;
                wxArrayString cmp_items ;
                for(const auto& p : ChoiceTranslator::label2val) {
                    cmp_items.Add(p.second) ;
                }
                auto& rch = pimpl->ch_params["cmd_pos"] = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, cmp_items) ;
                cmd_sizer->Add(rch, flags) ;

                add_sc("cmd_xmargin", -500, 500, 32) ;
                add_sc("cmd_ymargin", -500, 500, 64) ;

                cmd_sizer_wrapper->Add(cmd_sizer, flags) ;
                opts_sizer->Add(cmd_sizer_wrapper, flags) ;
            }

            opts_sizer_wrapper->AddStretchSpacer() ;

            auto defbtn_flags = flags ;
            defbtn_flags.Align(wxALIGN_RIGHT) ;
            opts_sizer_wrapper->Add(new wxButton(this,
                OptsEvt::DEFAULT, trans(Label::Pref_Return_To_Default)), defbtn_flags) ;

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
            ERROR_STREAM << e.what() << ": " << index << "is invalid index. (OptionsPanel::load_core)\n" ;
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
                //params has loaded params.
                //it converts to choice's string
                p.second->SetSelection(
                    p.second->FindString(
                        ChoiceTranslator::label2val.at(params.at(p.first))
                    )
                ) ;
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
            params[p.first] = std::next(ChoiceTranslator::label2val.begin(), index)->first ;
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