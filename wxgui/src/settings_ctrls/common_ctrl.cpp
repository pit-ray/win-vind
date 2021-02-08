#include "common_ctrl.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/filepicker.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
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

        wxStaticText* gvim_path_label ;
        wxFilePickerCtrl* gvim_path_picker ;

        wxStaticText* font_name_label ;
        wxTextCtrl* font_name ;

        explicit Impl(wxWindow* const self)
        : chm(self),
          scs(),
          lbs(),
          gvim_path_label(nullptr),
          gvim_path_picker(nullptr),
          font_name_label(nullptr),
          font_name(nullptr)
        {}

        void update_labels() noexcept {
            for(auto& l : lbs) {
                l.second->SetLabel(ioParams::get_label(l.first)) ;
            }

            gvim_path_label->SetLabel(ioParams::get_label("gvim_exe_path")) ;
            font_name_label->SetLabel(ioParams::get_label("gui_font_name")) ;
        }

        template <typename T1, typename T2>
        void load_config_common(const T1& get_vi_func, const T2& get_vs_func) noexcept {
            for(auto& p : scs) {
                p.second->SetValue(get_vi_func(p.first)) ;
            }

            gvim_path_picker->SetPath(get_vs_func("gvim_exe_path")) ;

            font_name->ChangeValue(get_vs_func("gui_font_name")) ;
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
        flags.Border(wxALL, BORDER) ;

        auto root_sizer = new wxFlexGridSizer(2) ;

        auto create_ch = [this, root_sizer, &flags](const auto obj_name) {
            pimpl->lbs[obj_name] = new wxStaticText(this, wxID_ANY, obj_name) ;
            root_sizer->Add(pimpl->lbs[obj_name], flags) ;

            auto ptr = pimpl->chm.create(obj_name) ;
            root_sizer->Add(ptr, flags) ;
        } ;
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

        create_ch("ui_lang") ;
        create_ch("icon_style") ;
        create_ch("kb_type") ;
        create_ch("initial_mode") ;
 
        pimpl->gvim_path_label = new wxStaticText(this, wxID_ANY, wxT("gvim_exe_path")) ;
        root_sizer->Add(pimpl->gvim_path_label, flags) ;

        pimpl->gvim_path_picker = new wxFilePickerCtrl(this, wxID_ANY, wxT("gvim"),
                wxFileSelectorPromptStr, wxFileSelectorDefaultWildcardStr, wxDefaultPosition,
                wxSize(static_cast<int>(WIDTH() / 3.5), -1),
                wxFLP_SMALL | wxFLP_USE_TEXTCTRL | wxFLP_FILE_MUST_EXIST) ;
        pimpl->gvim_path_picker->SetBackgroundColour(wxColour(*wxWHITE)) ;
        root_sizer->Add(pimpl->gvim_path_picker, flags) ;

        create_sc("gui_font_size", 2, 20, 9) ;

        pimpl->font_name_label = new wxStaticText(this, wxID_ANY, wxT("GUI Font Name")) ;
        root_sizer->Add(pimpl->font_name_label, flags) ;
        pimpl->font_name = new wxTextCtrl(this, wxID_ANY, wxT("Consolas"),
                wxDefaultPosition, wxSize(static_cast<int>(WIDTH() / 4), -1)) ;
        root_sizer->Add(pimpl->font_name, flags) ;

        SetSizerAndFit(root_sizer) ;
    }
    CommonCtrl::~CommonCtrl() noexcept = default ;

    void CommonCtrl::translate() noexcept {
        pimpl->update_labels() ;
        pimpl->chm.update_config() ;
    }

    void CommonCtrl::do_load_config() noexcept {
        pimpl->load_config_common(ioParams::get_vi, ioParams::get_vs) ;
        pimpl->chm.load_config() ;
    }

    void CommonCtrl::do_load_config_default() noexcept {
        pimpl->load_config_common(ioParams::Default::get_vi, ioParams::Default::get_vs) ;
        pimpl->chm.load_config_default() ;
    }

    void CommonCtrl::do_save_config() noexcept {
        pimpl->chm.save_config() ;
        for(const auto& p : pimpl->scs) {
            ioParams::set(p.first, p.second->GetValue()) ;
        }
        ioParams::set("gvim_exe_path", pimpl->gvim_path_picker->GetPath()) ;
        ioParams::set("gui_font_name", pimpl->font_name->GetLineText(0)) ;
    }

    const wxString CommonCtrl::name() noexcept {
        return UITrans::trans("notify/preferences/settings/common") ;
    }

}
