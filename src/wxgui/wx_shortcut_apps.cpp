#include "wx_shortcut_apps.hpp"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/filepicker.h>
#include <wx/listctrl.h>
#include <wx/button.h>

#include "wx_constant.hpp"
#include "msg_logger.hpp"
#include "ui_translator.hpp"
#include "io_params.hpp"

namespace wxGUI
{
    namespace PathListCol {
        enum : unsigned char {
            NAME,
            PATH,
        } ;
    }

    namespace PathListEvt {
        enum : unsigned int {
            ADD = 10020,
            DEL,
        } ;
    }

    namespace ExappsEvt {
        enum : unsigned int  {
            DEFAULT = 10025,
        } ;
    }

    struct ShortcutAppsPanel::Impl
    {
        wxFilePickerCtrl* sh_path_picker  = nullptr ;
        wxListCtrl* path_list             = nullptr ;
        wxFilePickerCtrl* any_path_picker = nullptr ;
        wxTextCtrl* any_path_label        = nullptr ;

        wxStaticText* sh_label = nullptr ;
        wxButton* add_btn = nullptr ;
        wxButton* del_btn = nullptr ;
        wxButton* def_btn = nullptr ;

        template <typename T>
        void load_path_list(T&& ch) {
            path_list->DeleteAllItems() ;
            for(const auto& obj : ch) {
                try {
                    const auto& name = obj.at("name") ;
                    const auto& value = obj.at("value") ;

                    if(name == "shell") {
                        sh_path_picker->SetPath(value) ;
                        continue ;
                    }
                    auto num = path_list->InsertItem(PathListCol::NAME, name) ;
                    path_list->SetItem(num, PathListCol::PATH, value) ;
                }
                catch(const std::exception& e) {
                    continue ;
                }
            }
        }

        void update_labels() noexcept {
            sh_label->SetLabel(UITrans::trans("notify/preferences/exapps/console")) ;

            wxListItem lt ;
            if(path_list->GetColumn(PathListCol::NAME, lt)) {
                lt.SetText(UITrans::trans("notify/preferences/exapps/name")) ;
                path_list->SetColumn(PathListCol::NAME, lt) ;
            }

            if(path_list->GetColumn(PathListCol::PATH, lt)) {
                lt.SetText(UITrans::trans("notify/preferences/exapps/path")) ;
                path_list->SetColumn(PathListCol::PATH, lt) ;
            }

            add_btn->SetLabel(UITrans::trans("buttons/add")) ;
            del_btn->SetLabel(UITrans::trans("buttons/del")) ;
            def_btn->SetLabel(UITrans::trans("buttons/default")) ;
        }
    } ;

    ShortcutAppsPanel::ShortcutAppsPanel(wxBookCtrlBase* const p_book_ctrl)
    : PanelCore(p_book_ctrl, "notify/preferences/exapps"),
      pimpl(std::make_unique<Impl>())
    {
        using namespace UITrans ;
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;
        flags.Align(wxALIGN_CENTER_VERTICAL) ;

        auto exapps_sizer = new wxBoxSizer(wxVERTICAL) ;

        constexpr auto flp_flags = wxFLP_SMALL | wxFLP_USE_TEXTCTRL | wxFLP_FILE_MUST_EXIST ;

        {
            auto sh_sizer = new wxFlexGridSizer(2) ;
            pimpl->sh_label = new wxStaticText(this, wxID_ANY, wxT("Console")) ;
            sh_sizer->Add(pimpl->sh_label, flags) ;

            pimpl->sh_path_picker = new wxFilePickerCtrl(this, wxID_ANY, wxT("cmd"),
                wxFileSelectorPromptStr, wxFileSelectorDefaultWildcardStr, wxDefaultPosition,
                wxSize(static_cast<int>(WIDTH() * 0.75), wxDefaultCoord), flp_flags) ;
            sh_sizer->Add(pimpl->sh_path_picker, flags) ;
            exapps_sizer->Add(sh_sizer, flags) ;
        }

        {
            pimpl->path_list = new wxListCtrl(
                this, wxID_ANY, wxDefaultPosition,
                wxSize(WIDTH(), static_cast<int>(HEIGHT() * 0.4)),
                wxLC_REPORT
            ) ;

            static const auto name_width = static_cast<int>(WIDTH() * 0.187) ;
            static const auto path_width = WIDTH() - name_width - 100 ;
            wxListItem col0 ;
            col0.SetId(PathListCol::NAME) ;
            col0.SetText(trans("notify/preferences/exapps/name")) ;
            col0.SetWidth(name_width) ;
            pimpl->path_list->InsertColumn(PathListCol::NAME, col0) ;

            wxListItem col1 ;
            col1.SetId(PathListCol::PATH) ;
            col1.SetText(trans("notify/preferences/exapps/path")) ;
            col1.SetWidth(path_width) ;
            pimpl->path_list->InsertColumn(PathListCol::PATH, col1) ;

            exapps_sizer->Add(pimpl->path_list, flags) ;

            //input sections
            auto ctrls_sizer = new wxBoxSizer(wxHORIZONTAL) ;
            pimpl->any_path_label = new wxTextCtrl(this, wxID_ANY) ;
            ctrls_sizer->Add(pimpl->any_path_label, flags) ;

            pimpl->any_path_picker = new wxFilePickerCtrl(this, wxID_ANY, wxT("C\\"),
                wxFileSelectorPromptStr, wxFileSelectorDefaultWildcardStr, wxDefaultPosition,
                wxSize(static_cast<int>(WIDTH() * 0.5), wxDefaultCoord), flp_flags) ;
            ctrls_sizer->Add(pimpl->any_path_picker, flags) ;

            pimpl->add_btn = new wxButton(this, PathListEvt::ADD, trans("buttons/add")) ;
            ctrls_sizer->Add(pimpl->add_btn, flags) ;
            pimpl->del_btn = new wxButton(this, PathListEvt::DEL, trans("buttons/del")) ;
            ctrls_sizer->Add(pimpl->del_btn, flags) ;

            exapps_sizer->Add(ctrls_sizer, flags) ;
        }

        exapps_sizer->AddStretchSpacer() ;
        auto def_sizer = new wxBoxSizer(wxHORIZONTAL) ;
        def_sizer->AddStretchSpacer() ;
        pimpl->def_btn = new wxButton(this, ExappsEvt::DEFAULT, trans("buttons/default")) ;
        def_sizer->Add(pimpl->def_btn, 0, wxEXPAND | wxALL | wxALIGN_RIGHT, BORDER) ;
        exapps_sizer->Add(def_sizer, 0, wxEXPAND | wxALL | wxALIGN_BOTTOM, BORDER) ;

        SetSizerAndFit(exapps_sizer) ;
        load_config() ;

        //set event handler ----------------------
        Bind(wxEVT_BUTTON, [this](auto&) {
            pimpl->load_path_list(ioParams::Default::get_choices("exapps")) ;
        }) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            //add a new item
            auto label = pimpl->any_path_label->GetLineText(0) ;
            auto path = pimpl->any_path_picker->GetPath() ;

            if(!label.empty() && !path.empty()) {
                auto num = pimpl->path_list->InsertItem(PathListCol::NAME, label) ;
                pimpl->path_list->SetItem(num, PathListCol::PATH, path) ;
            }

            //return to default state
            pimpl->any_path_label->Clear() ;
            pimpl->any_path_picker->SetPath(wxT("C:\\")) ;
        }, PathListEvt::ADD) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            //delete a item
            for(int i = 0 ; i < pimpl->path_list->GetItemCount() ; i ++) {
                if(pimpl->path_list->GetItemState(i, wxLIST_STATE_SELECTED) == wxLIST_STATE_SELECTED) {
                    pimpl->path_list->DeleteItem(i) ;
                    break ;
                }
            }
        }, PathListEvt::DEL) ;
    }

    ShortcutAppsPanel::~ShortcutAppsPanel() noexcept = default ;


    void ShortcutAppsPanel::do_load_config() noexcept {
        pimpl->load_path_list(ioParams::get_choices("exapps")) ;
    }

    void ShortcutAppsPanel::do_save_config() noexcept {
        ioParams::choices_t ch {
            {
                {"name", "shell"},
                {"value", pimpl->sh_path_picker->GetPath().ToStdString()}
            }
        } ;

        for(long i = 0 ; i < pimpl->path_list->GetItemCount() ; i ++) {
            const auto n = pimpl->path_list->GetItemText(i, PathListCol::NAME).ToStdString() ;
            const auto p  = pimpl->path_list->GetItemText(i, PathListCol::PATH).ToStdString() ;
            ioParams::item_t a{{"name", n}, {"value", p}} ;
            ch.emplace_back(std::move(a)) ;
        }

        ioParams::set("exapps", ch) ;
    }

    void ShortcutAppsPanel::translate() noexcept {
        pimpl->update_labels() ;
        Layout() ;
    }
}