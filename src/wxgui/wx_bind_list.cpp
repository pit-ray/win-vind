#include "wx_bind_list.hpp"

#include <wx/scrolwin.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>

#include "msg_logger.hpp"
#include "wx_constant.hpp"

namespace wxGUI
{
    namespace BindListEvt {
        enum : unsigned int {
            DEFAULT = 10200,
        } ;
    }

    constexpr auto SCR_HEIGHT = HEIGHT - 195 ;

    struct BindListPanel::Impl
    {
        ums_t<wxTextCtrl*> keybinds{} ;
        ums_t<wxTextCtrl*> cmds{} ;
    } ;

    //Hot Key List with Scroll bar
    class BindListPanel::HotkeyScrWindow : public wxScrolledWindow
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
            SetMinSize(wxSize(350, SCR_HEIGHT)) ; //if use auto WIDTH, objects is hidden by scroll-bar.
        }
    } ;

    //Command List with Scroll bar
    class BindListPanel::CmdScrWindow : public wxScrolledWindow
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
            SetMinSize(wxSize(315, SCR_HEIGHT)) ;
        }
    } ;

    BindListPanel::BindListPanel(wxBookCtrlBase* const p_book_ctrl)
    : wxPanel(p_book_ctrl),
      pimpl(std::make_unique<Impl>())
    {
        p_book_ctrl->AddPage(this, wxT("Bind List")) ;

        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto bindlist_sizer_wrapper = new wxBoxSizer(wxVERTICAL) ;

        {
            auto bindlist_sizer = new wxBoxSizer(wxHORIZONTAL) ;

            auto kb_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, wxT("HotKeys")) ;
            kb_sizer_wrapper->Add(new HotkeyScrWindow(this, wxID_ANY, pimpl->keybinds), flags) ;
            bindlist_sizer->Add(kb_sizer_wrapper, flags) ;

            auto cmd_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Commands")) ;
            cmd_sizer_wrapper->Add(new CmdScrWindow(this, wxID_ANY, pimpl->cmds), flags) ;
            bindlist_sizer->Add(cmd_sizer_wrapper, flags) ;

            bindlist_sizer_wrapper->Add(bindlist_sizer, flags) ;
        }

        bindlist_sizer_wrapper->AddStretchSpacer() ;

        auto defbtn_flags = flags ;
        defbtn_flags.Align(wxALIGN_RIGHT) ;
        bindlist_sizer_wrapper->Add(new wxButton(this,
            BindListEvt::DEFAULT, wxT("Return to Default")), defbtn_flags) ;

        SetSizerAndFit(bindlist_sizer_wrapper) ;
        load_all() ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            load_default() ;
        }, BindListEvt::DEFAULT) ;
    }

    BindListPanel::~BindListPanel() = default ;


    void BindListPanel::load_core(const PrefParser::ums_ums_str_t bind_list) {
        try {
            const auto hk = bind_list.at("binds") ;
            const auto cmd = bind_list.at("commands") ;

            //UI language and icon-style and keybrd-type is preparing

            auto catch_except = [](auto& e, auto& index) {
                Logger::error_stream << Logger::E << e.what() << ": " \
                << index << "is invalid index. (BindListPanel::load_core)\n" ;
            } ;

            //binds
            //wxTextCtrl
            for(auto& p : pimpl->keybinds) {
                try {
                    p.second->Clear() ;
                    *p.second << hk.at(p.first) ;
                }
                catch(std::out_of_range& e) {
                    catch_except(e, p.first) ;
                    continue ;
                }
            }

            //wxTextCtrl
            for(auto& p : pimpl->cmds) {
                try {
                    p.second->Clear() ;
                    *p.second << cmd.at(p.first) ;
                }
                catch(std::out_of_range& e) {
                    catch_except(e, p.first) ;
                    continue ;
                }
            }
        }
        catch(std::out_of_range& e) {
            Logger::error_stream << Logger::E \
            << e.what() << ": cannot load config (BindListPanel::load_core)\n" ;
        }
    }

    void BindListPanel::load_default() {
        load_core(PrefParser::load_default_binds()) ;
    }

    void BindListPanel::load_all() {
        load_core(PrefParser::load_binds()) ;
    }

    void BindListPanel::save_all() {
        using namespace PrefParser ;
        ums_ums_str_t binds{} ;
        //wxTextCtrl
        for(const auto& p : pimpl->keybinds) {
            binds["binds"][p.first] = p.second->GetLineText(0).ToStdString() ;
        }

        //wxTextCtrl
        for(const auto& p : pimpl->cmds) {
            binds["commands"][p.first] = p.second->GetLineText(0).ToStdString() ;
        }

        save_binds(binds) ;
    }
}