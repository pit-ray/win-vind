#include "wx_bindings.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <chrono>

#include "disable_gcc_warning.hpp"
#include <wx/arrstr.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/defs.h>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/srchctrl.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/generic/stattextg.h>
#include "enable_gcc_warning.hpp"

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "io_params.hpp"
#include "msg_logger.hpp"
#include "path.hpp"
#include "ui_translator.hpp"
#include "utility.hpp"
#include "wx_constant.hpp"

#include "common/key_absorber.hpp"
#include "common/mouse_eventer.hpp"
#include "common_bindings/change_mode.hpp"

namespace wxGUI
{
    namespace BindingsEvt {
        enum : unsigned int {
            DEFAULT = 10200,
            SELECT_FUNC,
            ADD_CMD,
            DEL_CMD,
            CHOICE_MODE,
            CHOIDE_LINKED_MODE,
            SEARCH,
            EDIT_WITH_VIM,
        } ;
    }

    static const std::array<std::string, 8> g_modes_label {
        "GUI Normal",
        "GUI Insert",
        "GUI Visual",

        "Edi Normal",
        "Edi Insert",
        "Edi Visual",
        "Edi Visual Line",

        "Command"
    } ;

    static const std::array<std::string, 10> g_modes_key {
        "guin",
        "guii",
        "guiv",

        "edin",
        "edii",
        "ediv",
        "edivl",

        "cmd",

        "mycwn",
        "mycwi"
    } ;

    static const auto g_modes_last_idx = g_modes_label.size() ;

    inline static void write_usage(std::ofstream& ofs) ;
    void write_pretty_all(std::ofstream& ofs, const nlohmann::json& obj) ;
    void write_pretty_one(std::ofstream& ofs, const nlohmann::json& obj) ;

    using namespace std::chrono ;
    constexpr std::chrono::seconds COOL_TIME = 1s ;

    struct BindingsPanel::Impl {
        wxSearchCtrl* search = nullptr ;
        wxListBox* func_list = nullptr ;

        wxStaticText* id_label = nullptr ;
        wxStaticText* id       = nullptr ;

        wxChoice* mode        = nullptr ;
        wxChoice* linked_mode = nullptr ;

        wxStaticText* mode_related_text = nullptr ;

        wxListBox* cmds     = nullptr ;
        wxTextCtrl* new_cmd = nullptr ;

        wxBoxSizer* right_sizer      = nullptr ;
        wxStaticBoxSizer* cmds_sizer = nullptr ;

        wxButton* cmd_add_btn   = nullptr ;
        wxButton* cmd_del_btn   = nullptr ;
        wxButton* edit_with_vim = nullptr ;
        wxButton* def_btn       = nullptr ;

        nlohmann::json parser{} ;

        system_clock::time_point hot_point = system_clock::now() ;

        using ovvec_t = std::vector<wxGenericStaticText*> ;
        ovvec_t mode_overview{ovvec_t(8, nullptr)} ;
        ovvec_t linked_mode_overview{ovvec_t(8, nullptr)} ;

        // mode_links states copying from each modes.
        // Each element has following maps.
        using linkmap_t = std::array<std::size_t, 8> ;
        std::unordered_map<std::string, linkmap_t> mode_links{} ;


        std::size_t get_selected_func_index() {
            const auto index = func_list->GetSelection() ;
            if(index == wxNOT_FOUND) {
                throw RUNTIME_EXCEPT("The function list is not selected.") ;
            }
            const auto func_label = func_list->GetString(index) ;

            for(std::size_t i = 0 ; i < parser.size() ; i ++) {
                const auto label = parser.at(i).at(ioParams::get_vs("ui_lang")).get<std::string>() ;
                if(wxString::FromUTF8(label.c_str()) == func_label) {
                    return i ;
                }
            }
            throw LOGIC_EXCEPT(func_label + " does not have a valid label.") ;
        }

        nlohmann::json& get_selected_func_json() {
            try {
                const auto index = get_selected_func_index() ;
                return parser.at(index) ;
            }
            catch(const std::runtime_error&) {
                static auto c_empty_obj = nlohmann::json() ;
                return c_empty_obj ;
            }
        }

        const std::string get_selected_func_name() {
            auto obj = get_selected_func_json() ;
            if(obj.empty()) return std::string() ;
            return obj["name"].get<std::string>() ;
        }

        void update_mode_overview() {
            const auto idstr = get_selected_func_name() ;
            if(idstr.empty()) {
                for(std::size_t i = 0 ; i < g_modes_last_idx ; i ++) {
                    mode_overview[i]->SetLabelText("") ;
                    linked_mode_overview[i]->SetLabelText("") ;
                }
                return ;
            }


            auto set_markup = [] (
                    wxGenericStaticText* p_gst,
                    const std::string& color,
                    const std::string& text) {
                p_gst->SetLabelMarkup("<tt><span foreground='" + color + "'>" + text + "</span></tt>") ;
            } ;

            for(std::size_t i = 0 ; i < g_modes_last_idx ; i ++) {
                const auto links_idx = mode_links[idstr][i] ;
                if(links_idx != g_modes_last_idx) { //has links
                    set_markup(mode_overview[i], "black", g_modes_key[i]) ;
                    set_markup(linked_mode_overview[i], "#1e96ff", //Blue
                            "--&gt; " + g_modes_key[links_idx]) ;
                }
                else if(!get_selected_func_json()[g_modes_key[i]].empty()) { //has own bindings
                    set_markup(mode_overview[i], "#056905", g_modes_key[i]) ; //Green
                    linked_mode_overview[i]->SetLabelText("") ;
                }
                else { //does not have bindings
                    set_markup(mode_overview[i], "#e65146", g_modes_key[i]) ; //Red
                    linked_mode_overview[i]->SetLabelText("") ;
                }
            }
        }

        void update_static_obj() {
            decltype(auto) obj = get_selected_func_json() ;
            if(obj.empty()) {
                id->SetLabelText("") ;
                return ;
            }

            const auto& idstr = obj["name"].get<std::string>() ;
            id->SetLabelText(idstr) ;

            //detect which a mode a target mode copys.
            linkmap_t linkmap ;
            linkmap.fill(g_modes_last_idx) ;
            bool initial_select_flag = false ;

            for(std::size_t i = 0 ; i < g_modes_last_idx ; i ++) {
                const auto& val = obj[g_modes_key[i]] ;
                if(val.empty()) {
                    continue ;
                }

                const auto& head = val[0].get<std::string>() ;
                const auto bracket1 = head.find_first_of('<') ;
                const auto bracket2 = head.find_last_of('>') ;

                if(bracket1 != std::string::npos && bracket2 != std::string::npos) {
                    try {
                        const auto inner_str = head.substr(bracket1 + 1, bracket2 - 1) ;
                        for(std::size_t j = 0 ; j < g_modes_last_idx ; j ++) {
                            if(inner_str == g_modes_key[j]) {
                                linkmap[i] = j ;
                                break ;
                            }
                        }
                    }
                    catch(const std::out_of_range&) {} //ignore other system keys
                }
                if(!initial_select_flag) {
                    mode->SetSelection(i) ;
                    initial_select_flag = true ;
                }
            }
            mode_links[idstr] = std::move(linkmap) ;
        }

        void update_linked_mode_choice() {
            const auto idstr = get_selected_func_name() ;
            if(idstr.empty()) {
                linked_mode->SetSelection(g_modes_last_idx) ;
                return ;
            }

            const auto mode_idx = mode->GetSelection() ;
            if(mode_idx == wxNOT_FOUND) {
                throw RUNTIME_EXCEPT("Could not get a selection of mode choices.") ;
            }
            try {
                const auto links = mode_links.at(idstr) ;
                try {
                    linked_mode->SetSelection(links.at(mode_idx)) ;
                }
                catch(const std::out_of_range&) {
                    throw RUNTIME_EXCEPT("Invalid mode index is passed.") ;
                }
            }
            catch(const std::out_of_range&) {
                throw RUNTIME_EXCEPT("The link dependencies of a selected function are not registered.") ;
            }
        }

        bool update_bindings_state() {
            const auto linked_idx = linked_mode->GetSelection() ;
            if(linked_idx == wxNOT_FOUND) {
                throw RUNTIME_EXCEPT("Could not get a selection of liked mode choices.") ;
            }

            if(linked_idx == g_modes_last_idx) {
                cmds->Enable() ;
                new_cmd->Enable() ;
                cmd_add_btn->Enable() ;
                cmd_del_btn->Enable() ;
                return true ;
            }
            else {
                cmds->Disable() ;
                new_cmd->Disable() ;
                cmd_add_btn->Disable() ;
                cmd_del_btn->Disable() ;
                return false ;
            }
        }

        void update_bindings() {
            cmds->Clear() ;
            update_linked_mode_choice() ;
            if(update_bindings_state()) {
                auto mode_idx = mode->GetSelection() ;
                if(mode_idx == wxNOT_FOUND) {
                    ERROR_PRINT("Mode Choise is not selected.") ;
                    return ;
                }

                decltype(auto) obj = get_selected_func_json() ;
                if(obj.empty()) {
                    return ;
                }
                auto modekey = g_modes_key[mode_idx] ;
                right_sizer->Show(cmds_sizer) ;
                for(const auto cmd : obj[modekey]) {
                    cmds->Append(cmd.get<std::string>()) ;
                }
            }
        }

        //tag database for search
        using tags_t = std::unordered_set<std::string> ;
        std::unordered_map<std::string, tags_t> tagbase{} ;

        void create_tag_database() {
            const auto& ui_langs = ioParams::get_choices("ui_lang") ;

            for(const auto& obj : parser) {
                try {
                    tags_t tags{} ;
                    const auto name = obj.at("name").get<std::string>() ;

                    //from name
                    tags.insert(name) ;

                    //from label
                    for(const auto& lang : ui_langs) {
                        try {
                            const auto& label = obj.at(lang.at("value")) ;
                            tags.insert(Utility::A2a(label)) ;
                        }
                        catch(const std::out_of_range&) {
                            continue ;
                        }
                    }

                    //from modes
                    for(const auto& m : g_modes_key) {
                        try {
                            if(!obj[m].empty()) tags.insert(m) ;
                        }
                        catch(const std::out_of_range&) {
                            continue ;
                        }
                    }

                    /*
                    //from tags
                    try {
                        for(const auto& t : obj.at("tags")) {
                            tags.insert(t.get<std::string>()) ;
                        }
                    }
                    catch(const nlohmann::json::out_of_range&) {}
                    */

                    tagbase[name] = std::move(tags) ;
                }
                catch(const std::out_of_range&)  {
                    continue ;
                }
            }
        }


        void update_func_list(const wxString& search_text) {
            func_list->Clear() ;

            auto append = [this](const nlohmann::json& obj) {
                try {
                    const auto& label = obj.at(ioParams::get_vs("ui_lang")).get<std::string>() ;
                    func_list->Append(wxString::FromUTF8(label.c_str())) ;
                }
                catch(const nlohmann::json::out_of_range&) {
                    return ;
                }
            } ;

            for(const auto& obj : parser) {
                if(search_text.IsEmpty()) { //search without keyword
                    append(obj) ;
                }
                else { //match with the tag database
                    for(const auto& tag : tagbase[obj["name"]]) {

                        //compare as UTF-8
                        if(tag.find(Utility::A2a(search_text.ToUTF8().data())) != std::string::npos) {
                            append(obj) ;
                            break ;
                        }
                    }
                }
            }
            if(!func_list->IsEmpty()) {
                func_list->SetSelection(0) ;
            }
        }

        void update_labels() {
            id_label->SetLabel(UITrans::trans("notify/preferences/bindings/id")) ;
            cmds_sizer->GetStaticBox()->SetLabel(UITrans::trans("notify/preferences/bindings/cmds")) ;

            cmd_add_btn->SetLabel(UITrans::trans("buttons/add")) ;
            cmd_del_btn->SetLabel(UITrans::trans("buttons/del")) ;
            def_btn->SetLabel(UITrans::trans("buttons/default")) ;
        }
    } ;

    BindingsPanel::BindingsPanel(wxBookCtrlBase* const p_book_ctrl)
    : PanelCore(p_book_ctrl, "notify/preferences/bindings"),
      pimpl(std::make_unique<Impl>())
    {
        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto root_sizer = new wxBoxSizer(wxHORIZONTAL) ;
        {
            const auto c_left_width = static_cast<int>(WIDTH() * 0.5) ;
            auto left_sizer = new wxBoxSizer(wxVERTICAL) ;
            pimpl->search = new wxSearchCtrl(
                    this, BindingsEvt::SEARCH, wxEmptyString, wxDefaultPosition,
                    wxSize(c_left_width, -1)) ;
            pimpl->search->ShowCancelButton(false) ;
            pimpl->search->ShowSearchButton(false) ;
            left_sizer->Add(pimpl->search, flags) ;

            pimpl->func_list = new wxListBox(
                this, BindingsEvt::SELECT_FUNC, wxDefaultPosition,
                wxSize(c_left_width, HEIGHT()), wxArrayString{}, wxLB_SINGLE
            ) ;
            left_sizer->Add(pimpl->func_list, flags) ;

            root_sizer->Add(left_sizer, flags) ;
        }
 
        root_sizer->AddStretchSpacer() ;
        {
            pimpl->right_sizer = new wxBoxSizer(wxVERTICAL) ;
            const auto c_right_width = static_cast<int>(WIDTH() * 0.4) ;
            {
               auto id_sizer = new wxBoxSizer(wxHORIZONTAL) ;
                auto fl = flags ;
                fl.Align(wxALIGN_CENTER_VERTICAL) ;
                pimpl->id_label = new wxStaticText(this, wxID_ANY, wxT("Identifier: ")) ;
                id_sizer->Add(pimpl->id_label, std::move(fl)) ;
                pimpl->id = new wxStaticText(this, wxID_ANY, wxT("undefined")) ;
                id_sizer->Add(pimpl->id, flags) ;
                pimpl->right_sizer->Add(id_sizer, flags) ;

                auto mov_sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Mode Overview")) ;
                for(std::size_t i = 0 ; i < pimpl->mode_overview.size() ; i += 2) {
                    auto mov_item_sizer = new wxBoxSizer(wxHORIZONTAL) ;

                    for(std::size_t j = 0 ; j < 2 ; j ++) {
                        if(j != 0) {
                            mov_item_sizer->Add(new wxGenericStaticText(
                                        this, wxID_ANY, wxT(""),
                                        wxDefaultPosition, wxSize(50, -1)
                                        ), 0, wxLEFT | wxALIGN_LEFT, BORDER) ;
                        }
                        pimpl->mode_overview[i + j] = new wxGenericStaticText(
                                this, wxID_ANY, wxT(""),
                                wxDefaultPosition, wxSize(50, -1)) ;
                        mov_item_sizer->Add(pimpl->mode_overview[i + j], 0, wxUP | wxBOTTOM | wxLEFT | wxALIGN_LEFT, BORDER) ;

                        pimpl->linked_mode_overview[i + j] = new wxGenericStaticText(
                                this, wxID_ANY, wxT(""),
                                wxDefaultPosition, wxSize(50, -1)) ;
                        mov_item_sizer->Add(pimpl->linked_mode_overview[i + j], 0, wxUP | wxBOTTOM | wxRIGHT| wxALIGN_LEFT, BORDER) ;
                    }
                    mov_sizer->Add(mov_item_sizer, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_HORIZONTAL, 5) ;
                }
                mov_sizer->SetMinSize(wxSize(c_right_width - 20, -1)) ;
                pimpl->right_sizer->Add(mov_sizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, BORDER) ;
            }
            {
                auto mode_sizer = new wxBoxSizer(wxHORIZONTAL) ;
                wxArrayString modes ;

                for(auto& l : g_modes_label) {
                    modes.Add(wxString(l)) ;
                }
                pimpl->mode = new wxChoice(
                        this, BindingsEvt::CHOICE_MODE, wxDefaultPosition,
                        wxSize(-1, -1), modes) ;
                pimpl->mode->SetSelection(0) ;
                mode_sizer->Add(pimpl->mode, flags) ;

                pimpl->mode_related_text = new wxStaticText(this, wxID_ANY, wxT("copy from")) ;
                mode_sizer->Add(pimpl->mode_related_text, 0, wxALL | wxALIGN_CENTER_VERTICAL, BORDER) ;

                modes.Insert(wxT("None"), g_modes_last_idx) ;
                pimpl->linked_mode = new wxChoice(
                        this, BindingsEvt::CHOIDE_LINKED_MODE, wxDefaultPosition,
                        wxSize(-1, -1), modes) ;
                pimpl->linked_mode->SetSelection(g_modes_last_idx) ;
                mode_sizer->Add(pimpl->linked_mode, flags) ;

                pimpl->right_sizer->Add(mode_sizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, BORDER) ;
            }
            {
                pimpl->cmds_sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Commands")) ;
                pimpl->cmds = new wxListBox(
                    this, wxID_ANY, wxDefaultPosition,
                    wxSize(c_right_width, HEIGHT() / 8),
                    wxArrayString{}, wxLB_SINGLE
                ) ;
                pimpl->cmds_sizer->Add(pimpl->cmds, 0, wxALL, 0) ;

                auto ctrls_sizer = new wxBoxSizer(wxHORIZONTAL) ;
                auto fl = flags ;
                fl.Align(wxALIGN_CENTER_VERTICAL) ;
                pimpl->new_cmd = new wxTextCtrl(
                    this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                    wxSize(c_right_width / 2, wxDefaultCoord)
                ) ;
                ctrls_sizer->Add(pimpl->new_cmd, fl) ;

                pimpl->cmd_add_btn = new wxButton(
                        this, BindingsEvt::ADD_CMD, wxT("Add"),
                        wxDefaultPosition, wxSize(-1, -1)) ;
                ctrls_sizer->Add(pimpl->cmd_add_btn, fl) ;

                pimpl->cmd_del_btn = new wxButton(
                        this, BindingsEvt::DEL_CMD, wxT("Delete"),
                        wxDefaultPosition, wxSize(-1, -1)) ;
                ctrls_sizer->Add(pimpl->cmd_del_btn, fl) ;
                pimpl->cmds_sizer->Add(ctrls_sizer, flags) ;
                pimpl->right_sizer->Add(pimpl->cmds_sizer, flags) ;
            }

            pimpl->right_sizer->AddStretchSpacer() ;

            {
                auto bottom_sizer = new wxBoxSizer(wxHORIZONTAL) ;
                pimpl->edit_with_vim = new wxButton(
                        this, BindingsEvt::EDIT_WITH_VIM, wxT("Edit With Vim"),
                        wxDefaultPosition, wxSize(c_right_width / 2, -1)) ;
                bottom_sizer->Add(pimpl->edit_with_vim, flags) ;

                bottom_sizer->AddStretchSpacer() ;
                pimpl->def_btn = new wxButton(this, BindingsEvt::DEFAULT, wxT("Return to Default"),
                        wxDefaultPosition, wxSize(-1, -1)) ;
                bottom_sizer->Add(pimpl->def_btn, flags) ;

                pimpl->right_sizer->Add(bottom_sizer, 0, wxALL | wxEXPAND, BORDER) ;
            }

            root_sizer->Add(pimpl->right_sizer, 0, wxEXPAND | wxALL, BORDER) ;
        }
        SetSizerAndFit(root_sizer) ;

        Bind(wxEVT_TEXT, [this](auto& e) {
            pimpl->update_func_list(e.GetString()) ;
            pimpl->update_static_obj() ;
            pimpl->update_bindings() ;
            pimpl->update_mode_overview() ;
        }, BindingsEvt::SEARCH) ;

        //left list is selected
        Bind(wxEVT_LISTBOX, [this](auto&) {
            pimpl->update_static_obj() ;
            pimpl->update_bindings() ;
            pimpl->update_mode_overview() ;
        }, BindingsEvt::SELECT_FUNC) ;

        Bind(wxEVT_CHOICE, [this](auto&) {
            pimpl->update_bindings() ;
        }, BindingsEvt::CHOICE_MODE) ;

        Bind(wxEVT_CHOICE, [this](auto&) {
            const auto idstr = pimpl->get_selected_func_name() ;
            if(idstr.empty()) {
                return ;
            }

            const auto target_idx = pimpl->mode->GetSelection() ;
            if(target_idx == wxNOT_FOUND) {
                throw RUNTIME_EXCEPT("Could not get a selection of mode choices.") ;
            }

            const auto linked_idx = pimpl->linked_mode->GetSelection() ;
            if(linked_idx == wxNOT_FOUND) {
                throw RUNTIME_EXCEPT("Could not get a selection of liked mode choices.") ;
            }

            try {
                pimpl->mode_links.at(idstr).at(target_idx) = linked_idx ;
            }
            catch(const std::out_of_range&) {
                throw RUNTIME_EXCEPT("The link dependencies of a selected function are not registered.") ;
            }

            pimpl->update_bindings_state() ;
            pimpl->update_mode_overview() ;
        }, BindingsEvt::CHOIDE_LINKED_MODE) ;

        //Cmds Add Button
        Bind(wxEVT_BUTTON, [this](auto&) {
            const auto str = pimpl->new_cmd->GetLineText(0) ;
            if(str.empty()) return ;

            try {
                const auto mode_idx = pimpl->mode->GetSelection() ;
                if(mode_idx != wxNOT_FOUND) {
                    auto& c = pimpl->get_selected_func_json().at(g_modes_key[mode_idx]) ;
                    if(!c.contains(str)) {
                        if(pimpl->cmds->IsEmpty()) {
                            c.clear() ;
                        }
                        c.push_back(str.ToStdString()) ;
                        pimpl->cmds->Append(str) ;
                    }
                }
            }
            catch(const std::exception& e) {
                ERROR_PRINT(e.what()) ;
            }

            pimpl->new_cmd->Clear() ;
            pimpl->update_mode_overview() ;
        }, BindingsEvt::ADD_CMD) ;

        //Cmds Delete Button
        Bind(wxEVT_BUTTON, [this](auto&) {
            const auto index = pimpl->cmds->GetSelection() ;
            if(index == wxNOT_FOUND) return ;

            pimpl->cmds->Delete(index) ;
            const auto mode_idx = pimpl->mode->GetSelection() ;
            if(mode_idx != wxNOT_FOUND) {
                try {
                    pimpl->get_selected_func_json().at(g_modes_key[mode_idx]).erase(index) ;
                }
                catch(const nlohmann::json::exception&){return ;}
            }
            pimpl->update_mode_overview() ;
        }, BindingsEvt::DEL_CMD) ;

        //Return to Default Button
        Bind(wxEVT_BUTTON, [this](auto&) {
            //read default json (partial)
            try {
                std::ifstream ifs(Path::to_u8path(Path::Default::BINDINGS())) ;
                nlohmann::json p{} ;
                ifs >> p ;

                const auto index = pimpl->get_selected_func_index() ;
                pimpl->parser.at(index).clear() ;
                pimpl->parser.at(index) = std::move(p.at(index)) ;

                pimpl->update_static_obj() ;
                pimpl->update_bindings() ;

                pimpl->update_mode_overview() ;
            }
            catch(const std::exception& e) {
                ERROR_PRINT(std::string(e.what()) + "BindingsEvt::DEFAULT)") ;
                return ;
            }
        }, BindingsEvt::DEFAULT) ;

        //Edit with Vim
        Bind(wxEVT_BUTTON, [this](auto&) {
            //duplicate clicks are prohibited
            if(system_clock::now() - pimpl->hot_point < COOL_TIME) {
                return ;
            }

            const auto gvim_exe = ioParams::get_vs("gvim_exe_path") ;
            static const auto temp_dir = Path::ROOT_PATH() + "temp\\" ;

            if(!Utility::is_existed_dir(temp_dir)) {
                Utility::create_directory(temp_dir) ;
            }

            auto& target_json = pimpl->get_selected_func_json() ;
            if(target_json.empty()) {
                return ;
            }

            const auto& temp_path = temp_dir + "edit_with_vim.json" ;
            std::ofstream ofs(Path::to_u8path(temp_path)) ;
            ofs << "[\n" ;
            write_pretty_one(ofs, target_json) ;
            write_usage(ofs) ;
            ofs.flush() ;

            HANDLE hproc ;
            auto create = [&hproc, &temp_path] (const std::string exe) {
                hproc = Utility::create_process(".", exe, "\"" + temp_path + "\"") ;
            } ;

            pimpl->edit_with_vim->Disable() ;

            const auto use_bindings = ioParams::get_vb("enable_specific_bindings_in_mygui") ;
            if(use_bindings) {
                MyConfigWindowInsert::sprocess(true, 1, nullptr, nullptr) ;
            }
            else {
                Change2Insert::sprocess(true, 1, nullptr, nullptr) ;
            }

            //release a message to push [Edit with Vim] button.
            KeyAbsorber::close_all_ports_with_refresh() ;

            try {
                create(gvim_exe) ;
            }
            catch(const std::runtime_error&) {
                create("notepad") ; //If failed a launch of gvim.exe, alternatively starts with notepad.exe.
            }

            //wait until finishing a created child process.
            if(WaitForSingleObject(hproc, INFINITE) != WAIT_OBJECT_0) {
                ERROR_PRINT("Failed the process of gVim in a child process.") ;
                return ;
            }

            KeyAbsorber::close_all_ports_with_refresh() ;

            if(use_bindings) {
                MyConfigWindowNormal::sprocess(true, 1, nullptr, nullptr) ;
            }

            nlohmann::json new_json ;
            std::ifstream ifs(Path::to_u8path(temp_path)) ;
            ifs >> new_json ;
            target_json = new_json[0] ; //overwrite the inner json object

            pimpl->update_static_obj() ;
            pimpl->update_bindings() ;
            pimpl->update_mode_overview() ;

            pimpl->hot_point = system_clock::now() ;
            pimpl->edit_with_vim->Enable() ;
        }, BindingsEvt::EDIT_WITH_VIM) ;
    }
    BindingsPanel::~BindingsPanel() noexcept = default ;
    void BindingsPanel::do_load_config() {
        std::ifstream ifs(Path::to_u8path(Path::BINDINGS())) ;
        ifs >> pimpl->parser ;
    }

    void BindingsPanel::do_save_config() {
        for(auto& func : pimpl->parser) {
            for(std::size_t i = 0 ; i < g_modes_last_idx ; i ++) {
                try {
                    const auto linked_idx = pimpl->mode_links.at(func.at("name"))[i] ;
                    if(linked_idx != g_modes_last_idx) {
                        func.at(g_modes_key[i]) = {"<" + g_modes_key[linked_idx] + ">"} ;
                    }
                }
                catch(const std::out_of_range&) {
                    continue ;
                }
            }
        }

        std::ofstream ofs(Path::to_u8path(Path::BINDINGS())) ;
        write_pretty_all(ofs, pimpl->parser) ;
    }

    void BindingsPanel::translate() {
        pimpl->mode_links.clear() ;

        pimpl->update_func_list("") ;
        pimpl->create_tag_database() ;
        pimpl->search->Clear() ;

        pimpl->update_static_obj() ;
        pimpl->update_bindings() ;

        pimpl->update_labels() ;

        pimpl->update_mode_overview() ;
        Layout() ;
    }

    void write_pretty_all(std::ofstream& ofs, const nlohmann::json& obj) {
        ofs << "[\n" ;
        for(auto func = obj.cbegin() ; func != obj.cend() ; func ++) {
            if(func != obj.cbegin()) {
                ofs << ",\n" ;
            }
            write_pretty_one(ofs, *func) ;
        }
        ofs << "\n]" ;
    }

    void write_pretty_one(std::ofstream& ofs, const nlohmann::json& obj) {
        const auto& ui_langs = ioParams::get_choices("ui_lang") ;

        ofs << "    {\n" ;
        ofs << "        \"name\": " << obj["name"].dump() << ",\n" ;

        for(const auto& modestr : g_modes_key) {
            ofs << "        \"" << modestr << "\": " ;

            try {
                ofs << obj.at(modestr).dump() ;
            }
            catch(const std::out_of_range&) {
                ofs << "[]" ;
            }
            ofs << ",\n" ;
        }

        for(auto lang = ui_langs.cbegin() ;
                lang != ui_langs.cend() ; lang ++) {

            if(lang != ui_langs.cbegin()) {
                ofs << ",\n" ;
            }
            try {
                ofs << "        \"" << (*lang).at("value") << "\": " ;
                try {
                    ofs << obj.at((*lang).at("value")) ;
                }
                catch(const std::out_of_range&) {}
            }
            catch(const std::out_of_range&) {
                continue ;
            }
        }

        ofs << "\n    }" ;
    }

    inline static void write_usage(std::ofstream& ofs) {
        ofs << ",\n" ;

        std::ifstream ifs(Path::to_u8path(Path::MODULE_ROOT_PATH() + "resources\\usage.json")) ;
        nlohmann::json usage ;
        ifs >> usage ;

        ofs << std::setw(4) << usage ;
        ofs << "]" ;
    }
}
