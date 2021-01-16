#include "wx_bindings.hpp"

#include <fstream>
#include <iomanip>
#include <algorithm>

#include "disable_gcc_warning.hpp"
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/arrstr.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include "enable_gcc_warning.hpp"

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "path.hpp"
#include "msg_logger.hpp"

#include "wx_constant.hpp"
#include "io_params.hpp"
#include "ui_translator.hpp"

namespace wxGUI
{
    namespace BindingsEvt {
        enum : unsigned int {
            DEFAULT = 10200,
            SELECT_FUNC,
            ADD_KEY,
            DEL_KEY,
            ADD_CMD,
            DEL_CMD,
        } ;
    }

    struct BindingsPanel::Impl {
        wxListBox* func_list = nullptr ;

        wxStaticText* id_label = nullptr ;
        wxStaticText* id       = nullptr ;

        wxListBox* keys = nullptr ;
        wxListBox* cmds = nullptr ;

        wxTextCtrl* new_key = nullptr ;
        wxTextCtrl* new_cmd = nullptr ;

        wxBoxSizer* right_sizer      = nullptr ;
        wxStaticBoxSizer* keys_sizer = nullptr ;
        wxStaticBoxSizer* cmds_sizer = nullptr ;

        wxButton* key_add_btn = nullptr ;
        wxButton* key_del_btn = nullptr ;
        wxButton* cmd_add_btn = nullptr ;
        wxButton* cmd_del_btn = nullptr ;
        wxButton* def_btn   = nullptr ;

        nlohmann::json parser{} ;

        void update_shown_details() {
            auto update = [this](const nlohmann::json& obj) {
                id->SetLabelText(obj["name"].get<std::string>()) ;

                keys->Clear() ;
                if(obj["key"].empty()) {
                    right_sizer->Hide(keys_sizer) ;
                }
                else {
                    right_sizer->Show(keys_sizer) ;
                    for(const auto key : obj["key"]) {
                        keys->Append(key.get<std::string>()) ;
                    }
                }

                cmds->Clear() ;
                if(obj["cmd"].empty()) {
                    right_sizer->Hide(cmds_sizer) ;
                }
                else {
                    right_sizer->Show(cmds_sizer) ;
                    for(const auto cmd : obj["cmd"]) {
                        cmds->Append(cmd.get<std::string>()) ;
                    }
                }
            } ;

            const auto index = func_list->GetSelection() ;
            if(index == wxNOT_FOUND) {
                func_list->SetSelection(0) ;
                update(parser.front()) ;
                return ;
            }
            update(parser.at(index)) ;
        }

        void update_func_list() {
            func_list->Clear() ;
            for(const auto& obj : parser) {
                try {
                    func_list->Append(wxString::FromUTF8(obj.at(
                        ioParams::get_vs("ui_lang")
                    ).get<std::string>().c_str())) ;
                }
                catch(const std::exception&) {continue ;}
            }
        }

        void update_labels() noexcept {
            id_label->SetLabel(UITrans::trans("notify/preferences/bindings/id")) ;
            keys_sizer->GetStaticBox()->SetLabel(UITrans::trans("notify/preferences/bindings/keys")) ;
            cmds_sizer->GetStaticBox()->SetLabel(UITrans::trans("notify/preferences/bindings/cmds")) ;

            key_add_btn->SetLabel(UITrans::trans("buttons/add")) ;
            key_del_btn->SetLabel(UITrans::trans("buttons/del")) ;
            cmd_add_btn->SetLabel(UITrans::trans("buttons/add")) ;
            cmd_del_btn->SetLabel(UITrans::trans("buttons/del")) ;
            def_btn->SetLabel(UITrans::trans("buttons/default")) ;
        }

        bool read_json() noexcept {
            //pimpl->parser.clear() ;
            try {
                std::ifstream ifs(Path::BINDINGS()) ;
                ifs >> parser ;
            }
            catch(const std::exception& e) {
                ERROR_PRINT(e.what()) ;
                return false ;
            }
            return true ;
        }

        bool write_json() noexcept {
            try {
                std::ofstream ofs(Path::BINDINGS()) ;
                ofs << std::setw(4) << parser << std::endl ;
            }
            catch(const std::exception& e) {
                ERROR_PRINT(e.what()) ;
                return false ;
            }
            return true ;
        }
    } ;

    BindingsPanel::BindingsPanel(wxBookCtrlBase* const p_book_ctrl)
    : PanelCore(p_book_ctrl, "notify/preferences/bindings"),
      pimpl(std::make_unique<Impl>())
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto root_sizer = new wxBoxSizer(wxHORIZONTAL) ;
        pimpl->func_list = new wxListBox(
            this, BindingsEvt::SELECT_FUNC, wxDefaultPosition,
            wxSize(static_cast<int>(WIDTH() * 0.4), HEIGHT()),
            wxArrayString{}, wxLB_SINGLE
        ) ;

        pimpl->read_json() ;
        root_sizer->Add(pimpl->func_list, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, BORDER) ;
        root_sizer->AddStretchSpacer() ;

        pimpl->right_sizer = new wxBoxSizer(wxVERTICAL) ;
        {
            auto id_sizer = new wxBoxSizer(wxHORIZONTAL) ;
            {
                auto fl = flags ;
                fl.Align(wxALIGN_CENTER_VERTICAL) ;
                pimpl->id_label = new wxStaticText(this, wxID_ANY, wxT("Identifier: ")) ;
                id_sizer->Add(pimpl->id_label, std::move(fl)) ;
                pimpl->id = new wxStaticText(this, wxID_ANY, wxT("")) ;
                id_sizer->Add(pimpl->id, flags) ;
            }
            pimpl->right_sizer->Add(id_sizer, flags) ;

            pimpl->keys_sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Keys")) ;
            {
                pimpl->keys = new wxListBox(
                    this, wxID_ANY, wxDefaultPosition,
                    wxSize(static_cast<int>(WIDTH() * 0.5), static_cast<int>(HEIGHT() * 0.125)),
                    wxArrayString{}, wxLB_SINGLE
                ) ;
                pimpl->keys_sizer->Add(pimpl->keys, flags) ;

                auto ctrls_sizer = new wxBoxSizer(wxHORIZONTAL) ;
                auto fl = flags ;
                fl.Align(wxALIGN_CENTER_VERTICAL) ;
                pimpl->new_key = new wxTextCtrl(
                    this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                    wxSize(static_cast<int>(WIDTH() * 0.25), wxDefaultCoord)
                ) ;
                ctrls_sizer->Add(pimpl->new_key, fl) ;
                pimpl->key_add_btn = new wxButton(this, BindingsEvt::ADD_KEY, wxT("Add")) ;
                ctrls_sizer->Add(pimpl->key_add_btn, fl) ;
                pimpl->key_del_btn = new wxButton(this, BindingsEvt::DEL_KEY, wxT("Delete")) ;
                ctrls_sizer->Add(pimpl->key_del_btn, fl) ;
                pimpl->keys_sizer->Add(ctrls_sizer, flags) ;
            }
            pimpl->right_sizer->Add(pimpl->keys_sizer, flags) ;

            pimpl->cmds_sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Commands")) ;
            {
                pimpl->cmds = new wxListBox(
                    this, wxID_ANY, wxDefaultPosition,
                    wxSize(static_cast<int>(WIDTH() * 0.5), static_cast<int>(HEIGHT() * 0.125)),
                    wxArrayString{}, wxLB_SINGLE
                ) ;
                pimpl->cmds_sizer->Add(pimpl->cmds, flags) ;

                auto ctrls_sizer = new wxBoxSizer(wxHORIZONTAL) ;
                auto fl = flags ;
                fl.Align(wxALIGN_CENTER_VERTICAL) ;
                pimpl->new_cmd = new wxTextCtrl(
                    this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                    wxSize(static_cast<int>(WIDTH() * 0.25), wxDefaultCoord)
                ) ;
                ctrls_sizer->Add(pimpl->new_cmd, fl) ;

                pimpl->cmd_add_btn = new wxButton(this, BindingsEvt::ADD_CMD, wxT("Add")) ;
                ctrls_sizer->Add(pimpl->cmd_add_btn, fl) ;
                pimpl->cmd_del_btn = new wxButton(this, BindingsEvt::DEL_CMD, wxT("Delete")) ;
                ctrls_sizer->Add(pimpl->cmd_del_btn, fl) ;
                pimpl->cmds_sizer->Add(ctrls_sizer, flags) ;
            }
            pimpl->right_sizer->Add(pimpl->cmds_sizer, flags) ;

            pimpl->right_sizer->AddStretchSpacer() ;
            auto df = flags;
            pimpl->def_btn = new wxButton(this, BindingsEvt::DEFAULT, wxT("Return to Default")) ;
            pimpl->right_sizer->Add(pimpl->def_btn, std::move(df.Right())) ;
        }
        root_sizer->Add(pimpl->right_sizer, 0, wxEXPAND | wxALL | wxALIGN_CENTRE_HORIZONTAL, BORDER) ;
        SetSizerAndFit(root_sizer) ;

        //left list is selected
        Bind(wxEVT_LISTBOX, [this](auto&) {
            pimpl->update_shown_details() ;
        }, BindingsEvt::SELECT_FUNC) ;

        //Keys Add Button
        Bind(wxEVT_BUTTON, [this](auto&) {
            const auto str = pimpl->new_key->GetLineText(0) ;
            if(str.empty()) return ;

            const auto index = pimpl->func_list->GetSelection() ;
            if(index == wxNOT_FOUND) return ;

            try {
                auto& c = pimpl->parser.at(index).at("key") ;
                if(!c.contains(str)) {
                    pimpl->keys->Append(str) ;
                    c.push_back(std::move(str.ToStdString())) ;
                }
            }
            catch(const std::exception& e) {
                ERROR_PRINT(e.what()) ;
            }

            pimpl->new_key->Clear() ;
        }, BindingsEvt::ADD_KEY) ;

        //Keys Delete Button
        Bind(wxEVT_BUTTON, [this](auto&) {
            const auto index = pimpl->keys->GetSelection() ;
            if(index == wxNOT_FOUND) return ;

            const auto func_index = pimpl->func_list->GetSelection() ;
            if(func_index == wxNOT_FOUND) return ;

            pimpl->keys->Delete(index) ;
            try{pimpl->parser.at(func_index).at("key").erase(index) ;}
            catch(const std::exception& e) {
                ERROR_PRINT(e.what()) ;
            }
        }, BindingsEvt::DEL_KEY) ;

        //Cmds Add Button
        Bind(wxEVT_BUTTON, [this](auto&) {
            const auto str = pimpl->new_cmd->GetLineText(0) ;
            if(str.empty()) return ;

            const auto index = pimpl->func_list->GetSelection() ;
            if(index == wxNOT_FOUND) return ;

            try {
                auto& c = pimpl->parser.at(index).at("cmd") ;
                if(!c.contains(str)) {
                    c.push_back(str.ToStdString()) ;
                    pimpl->cmds->Append(str) ;
                }
            }
            catch(const std::exception& e) {
                ERROR_PRINT(e.what()) ;
            }

            pimpl->new_cmd->Clear() ;
        }, BindingsEvt::ADD_CMD) ;

        //Cmds Delete Button
        Bind(wxEVT_BUTTON, [this](auto&) {
            const auto index = pimpl->cmds->GetSelection() ;
            if(index == wxNOT_FOUND) return ;

            const auto func_index = pimpl->func_list->GetSelection() ;
            if(func_index == wxNOT_FOUND) return ;

            pimpl->cmds->Delete(index) ;
            try {pimpl->parser.at(func_index).at("cmd").erase(index) ;}
            catch(const nlohmann::json::exception&){return ;}
        }, BindingsEvt::DEL_CMD) ;

        //Return to Default Button
        Bind(wxEVT_BUTTON, [this](auto&) {
            //read default json (partial)
            try {
                const auto index = pimpl->func_list->GetSelection() ;
                if(index == wxNOT_FOUND) return ;

                std::ifstream ifs(Path::Default::BINDINGS()) ;
                nlohmann::json p{} ;
                ifs >> p ;

                pimpl->parser.at(index).clear() ;
                pimpl->parser.at(index) = std::move(p.at(index)) ;

                pimpl->update_shown_details() ;
            }
            catch(const std::exception& e) {
                ERROR_PRINT(std::string(e.what()) + "BindingsEvt::DEFAULT)") ;
                return ;
            }
        }, BindingsEvt::DEFAULT) ;
    }
    BindingsPanel::~BindingsPanel() noexcept = default ;
    void BindingsPanel::do_load_config() noexcept {
        /*
        if(!pimpl->read_json()) {
            return ;
        }
        */
    }

    void BindingsPanel::do_save_config() noexcept {
        /*
        if(!pimpl->write_json()) {
            return ;
        }
        */
    }

    void BindingsPanel::translate() noexcept {
        /*
        pimpl->update_func_list() ;

        pimpl->func_list->SetSelection(0) ;
        pimpl->update_shown_details() ;

        pimpl->update_labels() ;
        Layout() ;
        */
    }
}
