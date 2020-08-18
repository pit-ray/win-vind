#include "wx_settings.hpp"

#include <map>

#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/string.h>

#include "msg_logger.hpp"
#include "pref_parser.hpp"
#include "wx_constant.hpp"
#include "ui_translator.hpp"
#include "wx_path.hpp"

namespace wxGUI
{
    namespace SettingsEvt {
        enum : unsigned int {
            DEFAULT = 10100,
        } ;
    }

    //This maps is converter from choiced text by wxChoice to saved text in file.
    namespace ChoiceCvt {
        using namespace UITrans ;
        //the index of these iterator is same as wxChoice's index, so not use unordered_map.
        static const std::map<Language, wxString> uilang {
            {Language::Japanese, trans(Label::Pref_Settings_Common_GUILang_JP)},
            {Language::English,  trans(Label::Pref_Settings_Common_GUILang_US)}
        } ;

        static const std::map<IconStyle, wxString> iconstyle {
            {IconStyle::Dark, trans(Label::Pref_Settings_Common_IconStyle_Dark)},
            {IconStyle::Light, trans(Label::Pref_Settings_Common_IconStyle_Light)}
        } ;

        static const std::map<std::string, wxString> kbtype {
            {"JP.kmp", trans(Label::Pref_Settings_Common_KBType_JP)},
            {"US.kmp", trans(Label::Pref_Settings_Common_KBType_US)},
            {"custom.kmp", trans(Label::Pref_Settings_Common_KBType_Custom)}
        } ;
    }

    struct SettingsPanel::Impl
    {
        wxChoice* ui_language   = nullptr ;
        wxChoice* icon_style    = nullptr ;
        wxChoice* keybrd_type   = nullptr ;
        wxSpinCtrl* resolution  = nullptr ;

        ums_t<wxSpinCtrl*>         sc_params{} ;
        ums_t<wxSpinCtrlDouble*>   scd_params{} ;
        ums_t<wxSlider*>           sl_params{} ;
    } ;

    SettingsPanel::SettingsPanel(wxBookCtrlBase* const p_book_ctrl)
    : wxPanel(p_book_ctrl),
      pimpl(std::make_unique<Impl>())
    {
        using namespace UITrans ;
        p_book_ctrl->AddPage(this, trans(Label::Pref_Settings)) ;

        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto setter_sizer_wrapper = new wxBoxSizer(wxVERTICAL) ;
        {
            auto setter_sizer = new wxBoxSizer(wxHORIZONTAL) ;

            auto add_st = [this, &flags](auto& sizer, auto&& text) {
                sizer->Add(new wxStaticText(this, wxID_ANY, std::forward<decltype(text)>(text)), flags) ;
            } ;

            //In old versionm, this sizer does not exist.
            //However, Commands Settings is very small, so I want to locate it under Common Settings.
            auto left_sizer_wrapper = new wxBoxSizer(wxVERTICAL) ;

            //Add Common Choices
            {
                auto com_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, trans(Label::Pref_Settings_Common)) ;
                auto com_sizer = new wxFlexGridSizer(2) ;

                add_st(com_sizer, trans(Label::Pref_Settings_Common_GUILang)) ;
                wxArrayString lgitems ;
                for(const auto& p : ChoiceCvt::uilang) {
                    lgitems.Add(p.second) ;
                }
                pimpl->ui_language = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, lgitems) ;
                com_sizer->Add(pimpl->ui_language, flags) ;

                add_st(com_sizer, trans(Label::Pref_Settings_Common_IconStyle)) ;
                wxArrayString isitems ;
                for(const auto& p : ChoiceCvt::iconstyle) {
                    isitems.Add(p.second) ;
                }
                pimpl->icon_style = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, isitems) ;
                com_sizer->Add(pimpl->icon_style, flags) ;

                add_st(com_sizer, trans(Label::Pref_Settings_Common_KBType)) ;
                wxArrayString ktitems ;
                for(const auto& p : ChoiceCvt::kbtype) {
                    ktitems.Add(p.second) ;
                }
                pimpl->keybrd_type = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ktitems) ;
                com_sizer->Add(pimpl->keybrd_type, flags) ;

                add_st(com_sizer, trans(Label::Pref_Settings_Common_Resolution)) ;
                pimpl->resolution = new wxSpinCtrl(
                    this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                    wxDefaultSize, wxSP_ARROW_KEYS, 1, 12, 6
                ) ;
                com_sizer->Add(pimpl->resolution, flags) ;

                com_sizer_wrapper->Add(com_sizer, flags) ;
                //setter_sizer->Add(com_sizer_wrapper, flags) ;
                left_sizer_wrapper->Add(com_sizer_wrapper, flags) ;
            }

            //Add Commands Settings
            {
                auto cmd_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, trans(Label::Pref_Settings_Commands));
                auto cmd_sizer = new wxFlexGridSizer(2) ;

                auto add_sc = [this, &flags, &cmd_sizer, &add_st](const auto id, const auto min, const auto max, const auto init) {
                    add_st(cmd_sizer, trans(id)) ;
                    pimpl->sc_params[id] = new wxSpinCtrl(
                        this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                        wxDefaultSize, wxSP_ARROW_KEYS, min, max, init
                    ) ;
                    cmd_sizer->Add(pimpl->sc_params[id], flags) ;
                } ;

                add_sc("cmd_max_char", 5, 1024, 32) ;
                add_sc("cmd_max_history_num", 1, 1024, 10) ;

                cmd_sizer_wrapper->Add(cmd_sizer, flags) ;
                //setter_sizer->Add(cmd_sizer_wrapper, flags) ;
                left_sizer_wrapper->Add(cmd_sizer_wrapper, flags) ;
            }
            setter_sizer->Add(left_sizer_wrapper, flags) ;

            //Add HotKeys Settings
            {
                auto hk_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, trans(Label::Pref_Settings_Hotkeys)) ;
                auto hk_sizer = new wxFlexGridSizer(2) ;

                auto add_sc = [this, &flags, &hk_sizer, &add_st](const auto id, const auto min, const auto max, const auto init) {
                    add_st(hk_sizer, trans(id)) ;
                    pimpl->sc_params[id] = new wxSpinCtrl(
                        this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                        wxDefaultSize, wxSP_ARROW_KEYS, min, max, init
                    ) ;
                    hk_sizer->Add(pimpl->sc_params[id], flags) ;
                } ;

                auto add_sl = [this, &flags, &hk_sizer, &add_st](const auto id, const auto min, const auto max, const auto init) {
                    add_st(hk_sizer, trans(id)) ;
                    pimpl->sl_params[id] = new wxSlider(this, wxID_ANY, init, min, max) ;
                    hk_sizer->Add(pimpl->sl_params[id], flags) ;
                } ;

                auto add_ratio = [this, &flags, &hk_sizer, &add_st](const auto id, const auto value) {
                    add_st(hk_sizer, trans(id)) ;
                    pimpl->scd_params[id] = new wxSpinCtrlDouble(
                        this, wxID_ANY,
                        wxEmptyString, wxDefaultPosition, wxDefaultSize,
                        wxSP_ARROW_KEYS, 0.0, 1.0, value, 0.001
                    ) ;
                    hk_sizer->Add(pimpl->scd_params[id], flags) ;
                } ;

                add_sc("screen_pos_buf", 0, 128, 10) ;
                add_sl("cursor_acceleration", 1, 128, 64) ;
                add_sl("cursor_weight", 1, 1024, 512) ;
                add_sl("cursor_max_velocity", 1, 20, 10) ;
                add_sl("yscroll_speed", 1, 10, 4) ;
                add_sl("xscroll_speed", 1, 10, 4) ;
                add_ratio("yscroll_screen_ratio", 0.125) ;
                add_ratio("xscroll_screen_ratio", 0.125) ;
                hk_sizer_wrapper->Add(hk_sizer, flags) ;
                setter_sizer->Add(hk_sizer_wrapper, flags) ;
            }

            setter_sizer_wrapper->Add(setter_sizer, flags) ;
        }

        setter_sizer_wrapper->AddStretchSpacer() ;

        //Add Return to Default Button
        auto defbtn_flags = flags ;
        defbtn_flags.Align(wxALIGN_RIGHT) ;
        setter_sizer_wrapper->Add(new wxButton(this,
            SettingsEvt::DEFAULT, trans(Label::Pref_Return_To_Default)), defbtn_flags) ;

        SetSizerAndFit(setter_sizer_wrapper) ;
        load_all() ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            load_default() ;
        }, SettingsEvt::DEFAULT) ;
    }

    SettingsPanel::~SettingsPanel() noexcept = default ;

    void SettingsPanel::load_core(
        const PrefParser::ums_str_t params,
        const std::string kb_path,
        const Language ui_idx,
        const IconStyle ico_idx,
        const unsigned char res_ppi_idx
    ) {
        auto catch_except = [](auto& e, auto& index) {
            ERROR_STREAM << e.what() << ": " << index << "is invalid index. (SettingsPanel::load_core)\n" ;
        } ;

        //wxSpinCtrl
        for(auto& p : pimpl->sc_params) {
            try {p.second->SetValue(params.at(p.first)) ;}
            catch(std::out_of_range& e) {
                catch_except(e, p.first) ;
                continue ;
            }
        }

        //wxSpinCtrlDouble
        for(auto& p : pimpl->scd_params) {
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

        //Path may be very long, so will not allpy SSO.
        const auto set_choice = [](const auto choice_ptr, const auto& loaded_data, const auto& cvt) {
            //wxChoice is based on ChoiceCvt, so at() does not throw exception.
            choice_ptr->SetSelection(choice_ptr->FindString(cvt.at(loaded_data))) ;
        } ;
        set_choice(pimpl->keybrd_type, kb_path, ChoiceCvt::kbtype) ;
        set_choice(pimpl->ui_language, ui_idx, ChoiceCvt::uilang) ;
        set_choice(pimpl->icon_style, ico_idx, ChoiceCvt::iconstyle) ;

        //Property Dialog's resolution PPI magnification
        pimpl->resolution->SetValue(res_ppi_idx) ;
    }

    void SettingsPanel::load_default() {
        using namespace PrefParser ;
        load_core(
            load_default_params(),
            load_default_kbtype(),
            load_default_uilang(),
            load_default_sticon(),
            load_default_propdlg_res()
        ) ;
    }

    void SettingsPanel::load_all() {
        using namespace PrefParser ;

        load_core(
            load_params(),
            load_kbtype(),
            load_uilang(),
            load_sticon(),
            load_propdlg_res()
        ) ;
    }

    void SettingsPanel::save_all() {
        using namespace PrefParser ;
        ums_str_t params{} ;

        //wxSpinCtrl
        for(const auto& p : pimpl->sc_params) {
            params[p.first] = std::to_string(p.second->GetValue())  ;
        }

        //wxSpinCtrlDouble
        for(const auto& p : pimpl->scd_params) {
            params[p.first] = std::to_string(p.second->GetValue())  ;
        }

        //wxSlider
        for(const auto& p : pimpl->sl_params) {
            params[p.first] = std::to_string(p.second->GetValue()) ;
        }

        const auto get_choice = [](const auto choice_ptr, const auto& cvt) {
            const auto index = choice_ptr->GetSelection() ;
            return std::next(cvt.begin(), index)->first ;
        } ;

        save_kbtype(get_choice(pimpl->keybrd_type, ChoiceCvt::kbtype)) ;
        save_uilang(get_choice(pimpl->ui_language, ChoiceCvt::uilang)) ;
        save_sticon(get_choice(pimpl->icon_style,  ChoiceCvt::iconstyle)) ;

        //Property Dialog's resolution PPI magnification
        save_propdlg_res(static_cast<unsigned char>(pimpl->resolution->GetValue())) ;

        save_params(params) ;
    }
}