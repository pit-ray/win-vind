#include "wx_shortcut_apps.hpp"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/filepicker.h>
#include <wx/listctrl.h>
#include <wx/button.h>

#include "wx_constant.hpp"
#include "msg_logger.hpp"

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

        wxListCtrl* path_list = nullptr ;
        wxFilePickerCtrl* any_path_picker = nullptr ;
        wxTextCtrl* any_path_label = nullptr ;
    } ;

    ShortcutAppsPanel::ShortcutAppsPanel(wxBookCtrlBase* const p_book_ctrl)
    : wxPanel(p_book_ctrl),
      pimpl(std::make_unique<Impl>())
    {
        p_book_ctrl->AddPage(this, wxT("Shortcut Apps")) ;

        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto exapps_sizer = new wxBoxSizer(wxVERTICAL) ;

        auto add_st = [this, &flags](auto& sizer, auto&& label) {
            sizer->Add(new wxStaticText(this, wxID_ANY, std::forward<decltype(label)>(label)), flags) ;
        } ;

        {
            auto sh_sizer = new wxFlexGridSizer(2) ;
            add_st(sh_sizer, wxT("console (command: 'shell')")) ;
            pimpl->sh_path_picker = new wxFilePickerCtrl(this, wxID_ANY, wxT("cmd"),
                wxFileSelectorPromptStr, wxFileSelectorDefaultWildcardStr, wxDefaultPosition,
                wxSize(WIDTH - 200, wxDefaultCoord)) ;
            sh_sizer->Add(pimpl->sh_path_picker, flags) ;
            exapps_sizer->Add(sh_sizer, flags) ;
        }

        {
            pimpl->path_list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(WIDTH, 225), wxLC_REPORT) ;

            constexpr auto name_width = 150 ;

            wxListItem col0 ;
            col0.SetId(PathListCol::NAME) ;
            col0.SetText(wxT("Name")) ;
            col0.SetWidth(name_width) ;
            pimpl->path_list->InsertColumn(PathListCol::NAME, col0) ;

            wxListItem col1 ;
            col1.SetId(PathListCol::PATH) ;
            col1.SetText(wxT("Path")) ;
            col1.SetWidth(WIDTH - name_width - 100) ;
            pimpl->path_list->InsertColumn(PathListCol::PATH, col1) ;

            //Add Initial Column ---------


            //----------------------------

            exapps_sizer->Add(pimpl->path_list, flags) ;

            auto ctrls_sizer = new wxBoxSizer(wxHORIZONTAL) ;
            pimpl->any_path_label = new wxTextCtrl(this, wxID_ANY) ;
            ctrls_sizer->Add(pimpl->any_path_label, flags) ;

            pimpl->any_path_picker = new wxFilePickerCtrl(this, wxID_ANY, wxT("C\\"),
                wxFileSelectorPromptStr, wxFileSelectorDefaultWildcardStr, wxDefaultPosition,
                wxSize(static_cast<int>(WIDTH * 0.5), wxDefaultCoord)) ;
            ctrls_sizer->Add(pimpl->any_path_picker, flags) ;

            ctrls_sizer->Add(new wxButton(this, PathListEvt::ADD, wxT("Add")), flags) ;
            ctrls_sizer->Add(new wxButton(this, PathListEvt::DEL, wxT("Delete")), flags) ;

            exapps_sizer->Add(ctrls_sizer, flags) ;
        }

        exapps_sizer->AddStretchSpacer() ;

        auto defbtn_flags = flags ;
        defbtn_flags.Align(wxALIGN_RIGHT) ;
        exapps_sizer->Add(new wxButton(this,
            ExappsEvt::DEFAULT, wxT("Return to Default")), defbtn_flags) ;

        SetSizerAndFit(exapps_sizer) ;

        load_all() ;

        //set event handler ----------------------
        Bind(wxEVT_BUTTON, [this](auto&) {
            load_default() ;
        }) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            //add a new item
            auto label = pimpl->any_path_label->GetLineText(0) ;
            auto path = pimpl->any_path_picker->GetPath() ;

            if(!label.empty() && !path.empty()) {
                auto& lc = pimpl->path_list ;

                auto num = lc->InsertItem(PathListCol::NAME, label) ;
                lc->SetItem(num, PathListCol::PATH, path) ;
            }

            pimpl->any_path_label->Clear() ;
            pimpl->any_path_picker->SetPath(wxT("C:\\")) ;
        }, PathListEvt::ADD) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            //delete a item
            auto& lc = pimpl->path_list ;

            for(int i = 0 ; i < lc->GetItemCount() ; i ++) {
                if(lc->GetItemState(i, wxLIST_STATE_SELECTED) == wxLIST_STATE_SELECTED) {
                    lc->DeleteItem(i) ;
                    break ;
                }
            }
        }, PathListEvt::DEL) ;
    }

    ShortcutAppsPanel::~ShortcutAppsPanel() = default ;


    void ShortcutAppsPanel::load_core(const PrefParser::ums_str_t paths) {
        pimpl->path_list->DeleteAllItems() ;

        for(const auto& p : paths) {
            if(p.first == "shell") {
                pimpl->sh_path_picker->SetPath(p.second) ;
                continue ;
            }

            auto num = pimpl->path_list->InsertItem(PathListCol::NAME, p.first) ;
            pimpl->path_list->SetItem(num, PathListCol::PATH, p.second) ;
        }
    }

    void ShortcutAppsPanel::load_default() {
        load_core(PrefParser::load_default_exapps()) ;
    }

    void ShortcutAppsPanel::load_all() {
        load_core(PrefParser::load_exapps()) ;
    }

    void ShortcutAppsPanel::save_all() {
        PrefParser::ums_str_t paths{} ;

        //shell
        paths["shell"] = pimpl->sh_path_picker->GetPath().ToStdString() ;

        for(long i = 0 ; i < pimpl->path_list->GetItemCount() ; i ++) {
            const auto n = pimpl->path_list->GetItemText(i, PathListCol::NAME).ToStdString() ;
            const auto p  = pimpl->path_list->GetItemText(i, PathListCol::PATH).ToStdString() ;

            paths[n] = p ;
        }

        //any
        PrefParser::save_exapps(paths) ;
    }
}