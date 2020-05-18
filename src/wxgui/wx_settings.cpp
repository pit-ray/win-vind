#include "wx_settings.hpp"

#include <unordered_map>
#include <functional>

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

namespace wxGUI
{
    namespace SettingsEvt {
        enum : unsigned int {
            DEFAULT = 10100,
        } ;
    }

    namespace ChoiceCvt {
        static const std::unordered_map<std::string, std::string> uilang {
            {"Japanese", "config/ui_Japanese.xml"},
            {"English",  "config/ui_English.xml"}
        } ;

        static const std::unordered_map<std::string, std::string> iconstyle {
            {"Dark",  "resources/icon32_dark.ico"},
            {"Light", "resources/icon32_light.ico"}
        } ;

        static const std::unordered_map<std::string, std::string> kbtype {
            {"JP(106/109)", "JP.kmp"},
            {"US(101/102)", "US.kmp"},
            {"custom.kmp",  "custom.kmp"}
        } ;
    }

    struct SettingsPanel::Impl
    {
        wxChoice* ui_language = nullptr ;
        wxChoice* icon_style  = nullptr ;
        wxChoice* keybrd_type = nullptr ;

        ums_t<wxSpinCtrl*>         sc_params{} ;
        ums_t<wxSpinCtrlDouble*>   scd_params{} ;
        ums_t<wxSlider*>           sl_params{} ;
    } ;

    SettingsPanel::SettingsPanel(wxBookCtrlBase* const p_book_ctrl)
    : wxPanel(p_book_ctrl),
      pimpl(std::make_unique<Impl>())
    {
        p_book_ctrl->AddPage(this, wxT("Settings")) ;

        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto setter_sizer_wrapper = new wxBoxSizer(wxVERTICAL) ;
        {
            auto setter_sizer = new wxBoxSizer(wxHORIZONTAL) ;

            auto add_st = [this, &flags](auto& sizer, auto&& text) {
                sizer->Add(new wxStaticText(this, wxID_ANY, std::forward<decltype(text)>(text)), flags) ;
            } ;

            //Common
            {
                auto com_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Common")) ;
                auto com_sizer = new wxFlexGridSizer(2) ;

                add_st(com_sizer, wxT("GUI Language")) ;
                wxArrayString lgitems ;
                for(const auto& p : ChoiceCvt::uilang) {
                    lgitems.Add(p.first) ;
                }
                pimpl->ui_language = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, lgitems) ;
                com_sizer->Add(pimpl->ui_language, flags) ;

                add_st(com_sizer, wxT("System Tray Icon Style")) ;
                wxArrayString isitems ;
                for(const auto& p : ChoiceCvt::iconstyle) {
                    isitems.Add(p.first) ;
                }
                pimpl->icon_style = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, isitems) ;
                com_sizer->Add(pimpl->icon_style, flags) ;

                add_st(com_sizer, wxT("Keyboard Type")) ;
                wxArrayString ktitems ;
                for(const auto& p : ChoiceCvt::kbtype) {
                    ktitems.Add(p.first) ;
                }
                pimpl->keybrd_type = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ktitems) ;
                com_sizer->Add(pimpl->keybrd_type, flags) ;

                com_sizer_wrapper->Add(com_sizer, flags) ;
                setter_sizer->Add(com_sizer_wrapper, flags) ;
            }

            //HotKeys
            {
                auto hk_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, wxT("HotKeys")) ;
                auto hk_sizer = new wxFlexGridSizer(2) ;

                auto add_sc = [this, &flags, &c = pimpl->sc_params, &hk_sizer](auto id) {
                    c[id] = new wxSpinCtrl(this, wxID_ANY) ;
                    hk_sizer->Add(c[id], flags) ;
                } ;

                auto add_sl = [this, &flags, &c = pimpl->sl_params, &hk_sizer](auto id, auto value, auto min, auto max) {
                    c[id] = new wxSlider(this, wxID_ANY, value, min, max) ;
                    hk_sizer->Add(c[id], flags) ;
                } ;

                auto add_ratio = [this, &flags, &c = pimpl->scd_params, &hk_sizer](auto id, auto value) {
                    c[id] = new wxSpinCtrlDouble(
                        this, wxID_ANY,
                        wxEmptyString, wxDefaultPosition, wxDefaultSize,
                        wxSP_ARROW_KEYS, 0.0, 1.0, value, 0.001
                    ) ;
                    hk_sizer->Add(c[id], flags) ;
                } ;

                add_st(hk_sizer, wxT("Screen Position Buffer At Jump Cursor")) ;
                add_sc("screen_pos_buf") ;

                add_st(hk_sizer, wxT("Move Cursor Acceleration")) ;
                add_sl("move_acceleration", 64, 1, 128) ;

                add_st(hk_sizer, wxT("Cursor Weight")) ;
                add_sl("cursor_weight", 512, 1, 1024) ;

                add_st(hk_sizer, wxT("Vertical Scroll Speed")) ;
                add_sl("yscroll_speed", 4, 1, 10) ;

                add_st(hk_sizer, wxT("Horizontal Scroll Speed")) ;
                add_sl("xscroll_speed", 4, 1, 10) ;

                add_st(hk_sizer, wxT("Vertical Page Scroll Ratio")) ;
                add_ratio("yscroll_screen_ratio", 0.125) ;

                add_st(hk_sizer, wxT("Horizontal Page Scroll Ratio")) ;
                add_ratio("xscroll_screen_ratio", 0.125) ;

                hk_sizer_wrapper->Add(hk_sizer, flags) ;
                setter_sizer->Add(hk_sizer_wrapper, flags) ;
            }
            setter_sizer_wrapper->Add(setter_sizer, flags) ;
        }

        setter_sizer_wrapper->AddStretchSpacer() ;

        auto defbtn_flags = flags ;
        defbtn_flags.Align(wxALIGN_RIGHT) ;
        setter_sizer_wrapper->Add(new wxButton(this,
            SettingsEvt::DEFAULT, wxT("Return to Default")), defbtn_flags) ;

        SetSizerAndFit(setter_sizer_wrapper) ;
        load_all() ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            load_default() ;
        }, SettingsEvt::DEFAULT) ;
    }

    SettingsPanel::~SettingsPanel() = default ;

    void SettingsPanel::load_core(
        const PrefParser::ums_str_t params,
        const std::string kb_path,
        const std::string ui_path,
        const std::string ico_path
    ) {
        auto catch_except = [](auto& e, auto& index) {
            Logger::error_stream << Logger::E << e.what() << ": " \
            << index << "is invalid index. (SettingsPanel::load_core)\n" ;
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

        const auto set_choice = [](auto& choice_ptr, auto& pth, auto& cvt) {
            for(const auto& p : cvt) {
                if(p.second != pth) {
                    continue ;
                }
                const auto index = choice_ptr->FindString(p.first) ;
                if(index != wxNOT_FOUND) {
                    choice_ptr->SetSelection(index) ;
                }
            }
        } ;
        set_choice(pimpl->keybrd_type, kb_path, ChoiceCvt::kbtype) ;
        set_choice(pimpl->ui_language, ui_path, ChoiceCvt::uilang) ;
        set_choice(pimpl->icon_style, ico_path, ChoiceCvt::iconstyle) ;
    }

    void SettingsPanel::load_default() {
        using namespace PrefParser ;
        load_core(
            load_default_params(),
            load_default_kbtype(),
            load_default_uilang(),
            load_default_sticon()
        ) ;
    }

    void SettingsPanel::load_all() {
        using namespace PrefParser ;

        load_core(
            load_params(),
            load_kbtype(),
            load_uilang(),
            load_sticon()
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

        const auto get_choice = [](auto& choice_ptr) {
            const auto index = choice_ptr->GetSelection() ;
            return choice_ptr->GetString(index).ToStdString() ;
        } ;

        //wxChoice has the key, so at() dont throw except.
        save_kbtype(ChoiceCvt::kbtype.at(get_choice(pimpl->keybrd_type))) ;
        save_uilang(ChoiceCvt::uilang.at(get_choice(pimpl->ui_language))) ;
        save_sticon(ChoiceCvt::iconstyle.at(get_choice(pimpl->icon_style))) ;

        save_params(params) ;
    }
}