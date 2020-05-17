#include "wx_prop_dlg.hpp"
#include "wx_system_tray.hpp"
#include "msg_logger.hpp"

#include <wx/panel.h>
#include <wx/bookctrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/scrolwin.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>
#include <wx/slider.h>
#include <wx/filepicker.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/colour.h>

#include <unordered_map>
#include <string>
#include <sstream>

namespace wxGUI
{
    constexpr auto WIDTH  = 800 ;
    constexpr auto HEIGHT = 494 ;
    constexpr auto BORDER = 5 ;
}

namespace wxGUI
{
    template <typename T>
    using ums_t = std::unordered_map<std::string, T> ;

    //Hot Key List with Scroll bar
    class PropDlg::HotkeyScrWindow : public wxScrolledWindow
    {
    public:
        explicit HotkeyScrWindow(wxWindow* parent, wxWindowID id, ums_t<wxTextCtrl*>& map)
        : wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxVSCROLL)
        {
            wxSizerFlags flags ;
            flags.Border(wxALL, BORDER) ;

            auto sizer = new wxFlexGridSizer(2) ;

            //insert map and add object
            auto add = [&sizer, &flags, &map, this](auto func_name, auto label_name) {
                sizer->Add(new wxStaticText(this, wxID_ANY, label_name), flags) ;
                map[func_name] = new wxTextCtrl(this, wxID_ANY) ;
                sizer->Add(map[func_name], flags) ;
            } ;

            {
                add("change_to_normal",             wxT("Change to Normal")) ;
                add("change_to_insert",             wxT("Change to Insert")) ;
                add("change_to_visual",             wxT("Change to Visual")) ;
                add("change_to_editor",             wxT("Change to Editor")) ;
                add("change_to_command",            wxT("Change to Command")) ;
                add("select_all",                   wxT("Select All")) ;
                add("move_left",                    wxT("Move Cursor Left")) ;
                add("move_right",                   wxT("Move Cursor Right")) ;
                add("move_up",                      wxT("Move Cursor Up")) ;
                add("move_down",                    wxT("Move Cursor Down")) ;
                add("jump_to_left",                 wxT("Jump Cursor to Left")) ;
                add("jump_to_right",                wxT("Jump Cursor to Right")) ;
                add("jump_to_top",                  wxT("Jump Cursor to Top")) ;
                add("jump_to_bottom",               wxT("Jump Cursor to Bottom")) ;
                add("jump_to_xcenter",              wxT("Jump Cursor to Horizontal Center")) ;
                add("jump_to_ycenter",              wxT("Jump Cursor to Vertical Center")) ;
                add("jump_to_any",                  wxT("Jump Cursor by Keyboard Mapping")) ;
                add("jump_to_active_window",        wxT("Jump Cursor to Selected Window")) ;
                add("scroll_up",                    wxT("Scroll Up")) ;
                add("scroll_down",                  wxT("Scroll Down")) ;
                add("scroll_mid_up",                wxT("Scroll Half Page Up")) ;
                add("scroll_mid_down",              wxT("Scroll Half Page Down")) ;
                add("scroll_page_up",               wxT("Scroll Page Up")) ;
                add("scroll_page_down",             wxT("Scroll Page Down")) ;
                add("scroll_left",                  wxT("Scroll Left")) ;
                add("scroll_right",                 wxT("Scroll Right")) ;
                add("scroll_mid_left",              wxT("Scroll Half Page Left")) ;
                add("scroll_mid_right",             wxT("Scroll Half Page Right")) ;
                add("click_left",                   wxT("Click Left")) ;
                add("click_right",                  wxT("Click Right")) ;
                add("cb_copy",                      wxT("Copy")) ;
                add("cb_paste",                     wxT("Paste")) ;
                add("cb_cut",                       wxT("Cut")) ;
                add("cb_delete",                    wxT("Delete")) ;
                add("sc_redo",                      wxT("Redo")) ;
                add("sc_undo",                      wxT("Undo")) ;
                add("switch_vdesktop_to_left",      wxT("Switch Virtual Desktop to Left")) ;
                add("switch_vdesktop_to_right",     wxT("Switch Virtual Desktop to Right")) ;
                add("search_pattern",               wxT("Search Pattern")) ;
                add("close_current_window",         wxT("Close Current Window")) ;
                add("switch_window",                wxT("Switch Window")) ;
                add("maximize_current_window",      wxT("Maximize Current Window")) ;
                add("minimize_current_window",      wxT("Minimize Current Window")) ;
                add("snap_current_window_to_left",  wxT("Snap Current Window to Left")) ;
                add("snap_current_window_to_right", wxT("Snap Current Window to Right")) ;
                add("move_to_next_page",            wxT("Move to Next Page")) ;
                add("move_to_prev_page",            wxT("Move to Previous Page")) ;

                //Editor
                add("change_to_edi_normal",         wxT("Change to Editor Normal")) ;
                add("change_to_edi_insert",         wxT("Change to Editor Insert")) ;
                add("change_to_edi_bkinsert",       wxT("Change to Editor BackInsert")) ;
                add("change_to_edi_nlinsert",       wxT("Change to Editor NewLineInsert")) ;
                add("change_to_edi_visual",         wxT("Change to Editor Visual")) ;
                add("edi_move_left",                wxT("Editor Move Left")) ;
                add("edi_move_right",               wxT("Editor Move Right")) ;
                add("edi_move_up",                  wxT("Editor Move Up")) ;
                add("edi_move_down",                wxT("Editor Move Down")) ;
            }

            SetSizer(sizer) ;
            SetScrollRate(0, 5) ;
            SetMinSize(wxSize(350, HEIGHT)) ; //if use auto WIDTH, objects is hidden by scroll-bar.
        }
    } ;

    //Command List with Scroll bar
    class PropDlg::CmdScrWindow : public wxScrolledWindow
    {
    public:
        explicit CmdScrWindow(wxWindow* parent, wxWindowID id, ums_t<wxTextCtrl*>& map)
        : wxScrolledWindow(parent, id)
        {
            wxSizerFlags flags ;
            flags.Border(wxALL, BORDER) ;

            auto sizer = new wxFlexGridSizer(2) ;

            auto add = [&sizer, &flags, &map, this](auto func_name, auto label_name) {
                sizer->Add(new wxStaticText(this, wxID_ANY, label_name), flags) ;
                map[func_name] = new wxTextCtrl(this, wxID_ANY) ;
                sizer->Add(map[func_name], flags) ;
            } ;

            add("save_opened_file",             wxT("Save Opened File")) ;
            add("close_opened_file",            wxT("Close Opened File(Tab)")) ;
            add("close_current_window",         wxT("Close Current Window")) ;
            add("switch_window",                wxT("Switch Window")) ;
            add("maximize_current_window",      wxT("Maximize Current Window")) ;
            add("minimize_current_window",      wxT("Minimize Current Window")) ;
            add("snap_current_window_to_left",  wxT("Snap Current Window to Left")) ;
            add("start_shell",                  wxT("Start Shell")) ;
            add("start_any_app",                wxT("Start Shortcut App")) ;

            SetScrollRate(0, 5) ;
            SetSizer(sizer) ;
            SetMinSize(wxSize(wxDefaultCoord, HEIGHT)) ;
        }
    } ;
}

namespace wxGUI
{
    constexpr auto TITLE = wxT("win-vind Preferences") ;
    constexpr auto ICON_PATH = wxT("resources/icon32.ico") ;

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
            DEFAULT,
        } ;
    }

    namespace SettingsEvt {
        enum : unsigned int {
            DEFAULT = 10100,
        } ;
    }

    namespace BindListEvt {
        enum : unsigned int {
            DEFAULT = 10200,
        } ;
    }

    struct PropDlg::Impl {
        wxChoice* ui_language = nullptr ;
        wxChoice* icon_style  = nullptr ;
        wxChoice* keybrd_type = nullptr ;

        ums_t<wxTextCtrl*> keybinds{} ;
        ums_t<wxTextCtrl*> cmds{} ;

        ums_t<wxTextCtrl*>         tc_params{} ;
        ums_t<wxSpinCtrl*>         sc_params{} ;
        ums_t<wxSpinCtrlDouble*>   scd_params{} ;
        ums_t<wxSlider*>           sl_params{} ;
        ums_t<wxColourPickerCtrl*> cp_params{} ;
        ums_t<wxChoice*>           ch_params{} ;

        wxFilePickerCtrl* sh_path_picker  = nullptr ;

        wxListCtrl* path_list = nullptr ;
        wxFilePickerCtrl* any_path_picker = nullptr ;
        wxTextCtrl* any_path_label = nullptr ;

        ums_t<wxCheckBox*> opts{} ;
    } ;

    PropDlg::PropDlg()
    : wxPropertySheetDialog(nullptr, wxID_ANY, TITLE),
      pimpl(std::make_unique<Impl>()),
      ptbi(std::make_unique<SystemTray>(ICON_PATH, wxT("win-vind"), this))
    {
        SetIcon(wxIcon(ICON_PATH, wxBITMAP_TYPE_ICO)) ;

        CreateButtons(wxOK | wxCANCEL | wxAPPLY) ;

        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        //Settings------------------------------------------
        {
            auto setter_panel = new wxPanel(GetBookCtrl()) ;
            GetBookCtrl()->AddPage(setter_panel, wxT("Settings")) ;
            auto setter_sizer_wrapper = new wxBoxSizer(wxVERTICAL) ;
            {
                auto setter_sizer = new wxBoxSizer(wxHORIZONTAL) ;

                auto add_st = [&setter_panel, &flags](auto& sizer, auto&& text) {
                    sizer->Add(new wxStaticText(setter_panel, wxID_ANY, std::forward<decltype(text)>(text)), flags) ;
                } ;

                //Common
                {
                    auto com_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, setter_panel, wxT("Common")) ;
                    auto com_sizer = new wxFlexGridSizer(2) ;

                    add_st(com_sizer, wxT("Language")) ;
                    wxArrayString lgitems ;
                    lgitems.Add(wxT("Japanese")) ;
                    lgitems.Add(wxT("English")) ;
                    pimpl->ui_language = new wxChoice(setter_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, lgitems) ;
                    com_sizer->Add(pimpl->ui_language, flags) ;

                    add_st(com_sizer, wxT("Icon Style")) ;
                    wxArrayString isitems ;
                    isitems.Add(wxT("Dark")) ;
                    isitems.Add(wxT("Light")) ;
                    pimpl->icon_style = new wxChoice(setter_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, isitems) ;
                    com_sizer->Add(pimpl->icon_style, flags) ;

                    add_st(com_sizer, wxT("Keyboard Type")) ;
                    wxArrayString ktitems ;
                    ktitems.Add(wxT("JP(106/109)")) ;
                    ktitems.Add(wxT("US(101/102)")) ;
                    pimpl->keybrd_type = new wxChoice(setter_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, ktitems) ;
                    com_sizer->Add(pimpl->keybrd_type, flags) ;

                    com_sizer_wrapper->Add(com_sizer, flags) ;
                    setter_sizer->Add(com_sizer_wrapper, flags) ;
                }

                //HotKeys
                {
                    auto hk_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, setter_panel, wxT("HotKeys")) ;
                    auto hk_sizer = new wxFlexGridSizer(2) ;

                    auto add_sc = [&setter_panel, &flags, &c = pimpl->sc_params, &hk_sizer](auto id) {
                        c[id] = new wxSpinCtrl(setter_panel, wxID_ANY) ;
                        hk_sizer->Add(c[id], flags) ;
                    } ;

                    auto add_sl = [&setter_panel, &flags, &c = pimpl->sl_params, &hk_sizer](auto id, auto value, auto min, auto max) {
                        c[id] = new wxSlider(setter_panel, wxID_ANY, value, min, max) ;
                        hk_sizer->Add(c[id], flags) ;
                    } ;

                    auto add_ratio = [&setter_panel, &flags, &c = pimpl->scd_params, &hk_sizer](auto id, auto value) {
                        c[id] = new wxSpinCtrlDouble(
                            setter_panel, wxID_ANY,
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
            setter_sizer_wrapper->Add(new wxButton(setter_panel,
                SettingsEvt::DEFAULT, wxT("Return to Default")), defbtn_flags) ;

            setter_panel->SetSizerAndFit(setter_sizer_wrapper) ;
        }

        //Bind List-----------------------------------------
        {
            auto bindlist_panel = new wxPanel(GetBookCtrl()) ;
            GetBookCtrl()->AddPage(bindlist_panel, wxT("Bind List")) ;

            auto bindlist_sizer = new wxBoxSizer(wxHORIZONTAL) ;

            auto kb_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, bindlist_panel, wxT("HotKeys")) ;
            kb_sizer_wrapper->Add(new HotkeyScrWindow(bindlist_panel, wxID_ANY, pimpl->keybinds), flags) ;
            bindlist_sizer->Add(kb_sizer_wrapper, flags) ;

            auto cmd_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, bindlist_panel, wxT("Commands")) ;
            cmd_sizer_wrapper->Add(new CmdScrWindow(bindlist_panel, wxID_ANY, pimpl->cmds), flags) ;
            bindlist_sizer->Add(cmd_sizer_wrapper, flags) ;

            bindlist_panel->SetSizerAndFit(bindlist_sizer) ;
        }

        //Shortcut Apps-------------------------------------
        {
            auto exapps_panel = new wxPanel(GetBookCtrl()) ;
            GetBookCtrl()->AddPage(exapps_panel, wxT("Shortcut Apps")) ;

            auto exapps_sizer = new wxBoxSizer(wxVERTICAL) ;

            auto add_st = [&exapps_panel, &flags](auto& sizer, auto&& label) {
                sizer->Add(new wxStaticText(exapps_panel, wxID_ANY, std::forward<decltype(label)>(label)), flags) ;
            } ;

            {
                auto sh_sizer = new wxFlexGridSizer(2) ;
                add_st(sh_sizer, wxT("console (command: 'shell')")) ;
                pimpl->sh_path_picker = new wxFilePickerCtrl(exapps_panel, wxID_ANY, wxT("cmd"),
                    wxFileSelectorPromptStr, wxFileSelectorDefaultWildcardStr, wxDefaultPosition,
                    wxSize(WIDTH - 200, wxDefaultCoord)) ;
                sh_sizer->Add(pimpl->sh_path_picker, flags) ;
                exapps_sizer->Add(sh_sizer, flags) ;
            }

            {
                pimpl->path_list = new wxListCtrl(exapps_panel, wxID_ANY, wxDefaultPosition, wxSize(WIDTH, 280), wxLC_REPORT) ;

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
                pimpl->any_path_label = new wxTextCtrl(exapps_panel, wxID_ANY) ;
                ctrls_sizer->Add(pimpl->any_path_label, flags) ;

                pimpl->any_path_picker = new wxFilePickerCtrl(exapps_panel, wxID_ANY, wxT("C\\"),
                    wxFileSelectorPromptStr, wxFileSelectorDefaultWildcardStr, wxDefaultPosition,
                    wxSize(static_cast<int>(WIDTH * 0.5), wxDefaultCoord)) ;
                ctrls_sizer->Add(pimpl->any_path_picker, flags) ;

                ctrls_sizer->Add(new wxButton(exapps_panel, PathListEvt::ADD, wxT("Add")), flags) ;
                ctrls_sizer->Add(new wxButton(exapps_panel, PathListEvt::DEL, wxT("Delete")), flags) ;

                exapps_sizer->Add(ctrls_sizer, flags) ;
            }

            exapps_panel->SetSizerAndFit(exapps_sizer) ;
        }

        //Options-------------------------------------------
        {
            auto opts_panel = new wxPanel(GetBookCtrl()) ;
            GetBookCtrl()->AddPage(opts_panel, wxT("Options")) ;
            auto opts_sizer = new wxBoxSizer(wxHORIZONTAL) ;
            {
                //Option Checks
                {
                    auto check_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, opts_panel, wxT("Options")) ;
                    auto check_sizer = new wxBoxSizer(wxVERTICAL) ;

                    auto add_cb = [&opts_panel, &flags, &check_sizer, &c = pimpl->opts](auto id, auto label) {
                        c[id] = new wxCheckBox(opts_panel, wxID_ANY, label) ;
                        check_sizer->Add(c[id], flags) ;
                    } ;

                    add_cb("autotrack_popup", wxT("Autotrack Popup")) ;
                    add_cb("display_cmd", wxT("Display Commands")) ;

                    check_sizer_wrapper->Add(check_sizer, flags) ;

                    opts_sizer->Add(check_sizer_wrapper, flags) ;
                }

                //Display Commands
                {
                    auto cmd_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, opts_panel, wxT("Display Commands")) ;
                    auto cmd_sizer = new wxFlexGridSizer(2) ;

                    auto add_cp = [&opts_panel, &flags, &cmd_sizer, &c = pimpl->cp_params](auto id) {
                        c[id] = new wxColourPickerCtrl(opts_panel, wxID_ANY) ;
                        cmd_sizer->Add(c[id], flags) ;
                    } ;
                    auto add_st = [&opts_panel, &flags, &cmd_sizer](auto&& text) {
                        cmd_sizer->Add(new wxStaticText(opts_panel, wxID_ANY, std::forward<decltype(text)>(text)), flags) ;
                    } ;
                    auto add_sc = [&opts_panel, &flags, &cmd_sizer, &c = pimpl->sc_params](auto id) {
                        c[id] = new wxSpinCtrl(opts_panel, wxID_ANY) ;
                        cmd_sizer->Add(c[id], flags) ;
                    } ;

                    add_st(wxT("Font Size")) ;
                    add_sc("cmd_font_size") ;

                    add_st(wxT("Font Weight")) ;
                    auto& rcfw = pimpl->sl_params["cmd_font_weight"] = new wxSlider(opts_panel, wxID_ANY, 600, 0, 1000) ;
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
                    auto& rch = pimpl->ch_params["cmd_pos"] = new wxChoice(opts_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, cmp_items) ;
                    cmd_sizer->Add(rch, flags) ;

                    add_st(wxT("Position X Margin")) ;
                    add_sc("cmd_xmargin") ;

                    add_st(wxT("Position Y Margin")) ;
                    add_sc("cmd_ymargin") ;

                    cmd_sizer_wrapper->Add(cmd_sizer, flags) ;
                    opts_sizer->Add(cmd_sizer_wrapper, flags) ;
                }

                opts_panel->SetSizerAndFit(opts_sizer) ;
            }
        }

        LayoutDialog() ;
        Centre() ;
        SetSize(wxSize(WIDTH, HEIGHT)) ;

        //set event handler ----------------------
        Bind(wxEVT_BUTTON, [this](auto&) {
            //add a new item
            auto label = pimpl->any_path_label->GetLineText(0) ;
            auto path = pimpl->any_path_picker->GetPath() ;

            if(!label.empty() && !path.empty()) {
                auto& lc = pimpl->path_list ;

                auto num = lc->GetItemCount() ;

                num = lc->InsertItem(PathListCol::NAME, label) ;
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

        Bind(wxEVT_BUTTON, [this](auto&) {
            using namespace PrefParser ;
            load_all(load_binds(), load_default_params(), load_default_opts()) ;
        }, SettingsEvt::DEFAULT) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            save_all() ;
        }, wxID_APPLY) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            save_all() ;
            Show(false) ;
        }, wxID_OK) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            Show(false) ;
        }, wxID_CANCEL) ;

        Bind(wxEVT_CLOSE_WINDOW, [this](auto&) {
            Show(false) ;
        }) ;


        using namespace PrefParser ;
        //save_binds(load_default_binds()) ;
        load_all(load_binds(), load_params(), load_opts()) ;
    }

    void PropDlg::load_all(
        const PrefParser::ums_ums_str_t binds,
        const PrefParser::ums_str_t params,
        const PrefParser::ums_bool_t opts) {

        auto hex2wxColour = [](auto hex) {
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

        try {
            using namespace PrefParser ;

            const auto hk = binds.at("binds") ;
            const auto cmd = binds.at("commands") ;

            //UI language and icon-style and keybrd-type is preparing

            auto catch_except = [](auto& e, auto& index) {
                Logger::error_stream << Logger::E << e.what() << ": " << index << "is invalid index. (PropDlg::load_all)\n" ;
            } ;

            //binds
            //wxTextCtrl
            for(auto& p : pimpl->keybinds) {
                try {*p.second << hk.at(p.first) ;}
                catch(std::out_of_range& e) {
                    catch_except(e, p.first) ;
                    continue ;
                }
            }

            //wxTextCtrl
            for(auto& p : pimpl->cmds) {
                try {*p.second << cmd.at(p.first) ;}
                catch(std::out_of_range& e) {
                    catch_except(e, p.first) ;
                    continue ;
                }
            }

            //params
            //wxTextCtrl
            for(auto& p : pimpl->tc_params) {
                try {*p.second << params.at(p.first) ;}
                catch(std::out_of_range& e) {
                    catch_except(e, p.first) ;
                    continue ;
                }
            }

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
                    const auto index = p.second->FindString(params.at(p.first)) ;
                    p.second->SetSelection(index) ;
                }
                catch(std::out_of_range& e) {
                    catch_except(e, p.first) ;
                    continue ;
                }
            }

            //opts
            //wxCheckBox
            for(auto& p : pimpl->opts) {
                try {p.second->SetValue(opts.at(p.first)) ;}
                catch(std::out_of_range& e) {
                    catch_except(e, p.first) ;
                    continue ;
                }
            }
        }
        catch(std::out_of_range& e) {
            Logger::error_stream << Logger::E \
            << e.what() << ": cannot load config (PropDlg::load_all)\n" ;
        }
    }

    void PropDlg::save_all() {
        auto to_hex = [](unsigned char val) {
            std::stringstream ss ;

            ss << std::hex << static_cast<int>(val) ;

            if(val == 0) {
                return "0" + ss.str() ;
            }

            return ss.str() ;
        } ;

        using namespace PrefParser ;
        ums_ums_str_t binds{} ;
        ums_str_t params{} ;
        ums_bool_t opts{} ;

        //UI language and icon-style and keybrd-type is preparing

        //binds
        //wxTextCtrl
        for(const auto& p : pimpl->keybinds) {
            binds["binds"][p.first] = p.second->GetLineText(0).ToStdString() ;
        }

        //wxTextCtrl
        for(const auto& p : pimpl->cmds) {
            binds["commands"][p.first] = p.second->GetLineText(0).ToStdString() ;
        }

        //params
        //wxTextCtrl
        for(const auto& p : pimpl->tc_params) {
            params[p.first] = p.second->GetLineText(0).ToStdString() ;
        }

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

        //wxColourPickerCtrl
        for(const auto& p : pimpl->cp_params) {
            const auto col = p.second->GetColour() ;
            params[p.first] = to_hex(col.Red()) + to_hex(col.Green()) + to_hex(col.Blue()) ;
        }

        //wxChoice
        for(const auto& p : pimpl->ch_params) {
            const auto index = p.second->GetSelection() ;
            params[p.first] = p.second->GetString(index) ;
        }

        //opts
        //wxCheckBox
        for(const auto& p : pimpl->opts) {
            opts[p.first] = p.second->GetValue() ;
        }

        save_binds(binds) ;
        save_params(params) ;
        save_opts(opts) ;
    }

    PropDlg::~PropDlg() = default ;
}