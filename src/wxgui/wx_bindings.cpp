#include "wx_bindings.hpp"

#include <fstream>
#include <iomanip>
#include <algorithm>

#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/arrstr.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>

#include "disable_gcc_warning.hpp"
#include "json.hpp"
#include "enable_gcc_warning.hpp"

#include "path.hpp"
#include "msg_logger.hpp"
#include "wx_path.hpp"
#include "wx_constant.hpp"

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
        wxStaticText* id = nullptr ;
        wxListBox* keys = nullptr ;
        wxListBox* cmds = nullptr ;
        wxTextCtrl* new_key = nullptr ;
        wxTextCtrl* new_cmd = nullptr ;
        nlohmann::json parser{} ;

        wxSizer* right_sizer = nullptr ;
        wxSizer* keys_sizer = nullptr ;
        wxSizer* cmds_sizer = nullptr ;

        void update_shown_details() noexcept {
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
            update(parser[index]) ;
        }

        bool read_json() noexcept {
            //pimpl->parser.clear() ;
            try {
                std::ifstream ifs(Path::BINDINGS_LIST()) ;
                ifs >> parser ;
            }
            catch(const std::exception& e) {
                ERROR_STREAM << e.what() << "(wxGUI::BindingsPanel::load_all)\n" ;
                return false ;
            }
            return true ;
        }
    
        bool write_json() noexcept {
            try {
                std::ofstream ofs(Path::BINDINGS_LIST()) ;
                ofs << std::setw(4) << parser << std::endl ;
            }
            catch(const std::exception& e) {
                ERROR_STREAM << e.what() << "(wxGUI::BindingsPanel::save_all)\n" ;
                return false ;
            }
            return true ;
        }
    } ;

    BindingsPanel::BindingsPanel(wxBookCtrlBase* const p_book_ctrl)
    : wxPanel(p_book_ctrl),
      pimpl(std::make_unique<Impl>())
    {
        p_book_ctrl->AddPage(this, "Bindings") ;

        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        pimpl->read_json() ;
        wxArrayString choices ;
        for(const auto& obj : pimpl->parser) {
            try {choices.Add(wxString::FromUTF8(obj["jp"].get<std::string>().c_str())) ;}
            catch(const nlohmann::json::exception&) {continue ;}
        }

        auto root_sizer = new wxBoxSizer(wxHORIZONTAL) ;
        pimpl->func_list = new wxListBox(
            this, BindingsEvt::SELECT_FUNC, wxDefaultPosition,
            wxSize(static_cast<int>(WIDTH * 0.4), static_cast<int>(HEIGHT * 0.8)),
            choices, wxLB_SINGLE
        ) ;
        root_sizer->Add(pimpl->func_list, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, BORDER) ;
        root_sizer->AddStretchSpacer() ;

        pimpl->right_sizer = new wxBoxSizer(wxVERTICAL) ;
        {
            auto id_sizer = new wxBoxSizer(wxHORIZONTAL) ;
            {
                auto fl = flags ;
                fl.Align(wxALIGN_CENTER_VERTICAL) ;
                id_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Identifier: ")), std::move(fl)) ;
                pimpl->id = new wxStaticText(this, wxID_ANY, wxT("")) ;
                id_sizer->Add(pimpl->id, flags) ;
            }
            pimpl->right_sizer->Add(id_sizer, flags) ;

            pimpl->keys_sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Keys")) ;
            {
                pimpl->keys = new wxListBox(
                    this, wxID_ANY, wxDefaultPosition,
                    wxSize(static_cast<int>(WIDTH * 0.5), static_cast<int>(HEIGHT * 0.15)),
                    wxArrayString{}, wxLB_SINGLE
                ) ;
                pimpl->keys_sizer->Add(pimpl->keys, flags) ;
 
                auto ctrls_sizer = new wxBoxSizer(wxHORIZONTAL) ;
                auto fl = flags ;
                fl.Align(wxALIGN_CENTER_VERTICAL) ;
                pimpl->new_key = new wxTextCtrl(
                    this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                    wxSize(static_cast<int>(WIDTH * 0.25), wxDefaultCoord)
                ) ;
                ctrls_sizer->Add(pimpl->new_key, fl) ;

                ctrls_sizer->Add(new wxButton(this, BindingsEvt::ADD_KEY, wxT("Add")), fl) ;
                ctrls_sizer->Add(new wxButton(this, BindingsEvt::DEL_KEY, wxT("Delete")), fl) ;
                pimpl->keys_sizer->Add(ctrls_sizer, flags) ;
            }
            pimpl->right_sizer->Add(pimpl->keys_sizer, flags) ;

            pimpl->cmds_sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Commands")) ;
            {
                pimpl->cmds = new wxListBox(
                    this, wxID_ANY, wxDefaultPosition,
                    wxSize(static_cast<int>(WIDTH * 0.5), static_cast<int>(HEIGHT * 0.15)),
                    wxArrayString{}, wxLB_SINGLE
                ) ;
                pimpl->cmds_sizer->Add(pimpl->cmds, flags) ;

                auto ctrls_sizer = new wxBoxSizer(wxHORIZONTAL) ;
                auto fl = flags ;
                fl.Align(wxALIGN_CENTER_VERTICAL) ;
                pimpl->new_cmd = new wxTextCtrl(
                    this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                    wxSize(static_cast<int>(WIDTH * 0.25), wxDefaultCoord)
                ) ;
                ctrls_sizer->Add(pimpl->new_cmd, fl) ;

                ctrls_sizer->Add(new wxButton(this, BindingsEvt::ADD_CMD, wxT("Add")), fl) ;
                ctrls_sizer->Add(new wxButton(this, BindingsEvt::DEL_CMD, wxT("Delete")), fl) ;
                pimpl->cmds_sizer->Add(ctrls_sizer, flags) ;
            }
            pimpl->right_sizer->Add(pimpl->cmds_sizer, flags) ;

            pimpl->right_sizer->AddStretchSpacer() ;
            //pimpl->right_sizer->Add(0, 0, 1, wxEXPAND | wxBOTTOM, BORDER) ;
            auto df = flags;
            pimpl->right_sizer->Add(
                new wxButton(this, BindingsEvt::DEFAULT,
                wxT("Return to Default")), std::move(df.Right())
            ) ;
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

            auto& c = pimpl->parser[index]["key"] ;
            if(!c.contains(str)) {
                pimpl->keys->Append(str) ;
                c.push_back(std::move(str.ToStdString())) ;
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
            try{pimpl->parser[func_index]["key"].erase(index) ;}
            catch(const nlohmann::json::exception&){return ;}
        }, BindingsEvt::DEL_KEY) ;

        //Cmds Add Button
        Bind(wxEVT_BUTTON, [this](auto&) {
            const auto str = pimpl->new_cmd->GetLineText(0) ;
            if(str.empty()) return ;

            const auto index = pimpl->func_list->GetSelection() ;
            if(index == wxNOT_FOUND) return ;

            auto& c = pimpl->parser[index]["cmd"] ;
            if(!c.contains(str)) {
                c.push_back(str.ToStdString()) ;
                pimpl->cmds->Append(str) ;
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
            try {pimpl->parser[func_index]["cmd"].erase(index) ;}
            catch(const nlohmann::json::exception&){return ;}
        }, BindingsEvt::DEL_CMD) ;

        //Return to Default Button
        Bind(wxEVT_BUTTON, [this](auto&) {
            //read default json (partial)
            try {
                const auto index = pimpl->func_list->GetSelection() ;
                if(index == wxNOT_FOUND) return ;

                std::ifstream ifs(Path::DEFAULT_BINDINGS) ;
                nlohmann::json p{} ;
                ifs >> p ;

                pimpl->parser[index].clear() ;
                pimpl->parser[index] = std::move(p.at(index)) ;

                pimpl->update_shown_details() ;
            }
            catch(const std::exception& e) {
                ERROR_STREAM << e.what() << "(wxGUI::BindingsPanel, BindingsEvt::DEFAULT)\n" ;
                return ;
            }
        }, BindingsEvt::DEFAULT) ;
    }
    BindingsPanel::~BindingsPanel() noexcept = default ;
    void BindingsPanel::load_all() {
        pimpl->func_list->SetSelection(0) ;
        pimpl->update_shown_details() ;

        if(!pimpl->read_json()) {
            return ;
        }
    }

    void BindingsPanel::save_all() {
        if(!pimpl->write_json()) {
            return ;
        }
    }
}