#include "wx_bind_list.hpp"

#include <wx/scrolwin.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>

#include "msg_logger.hpp"
#include "wx_constant.hpp"
#include "ui_translator.hpp"

namespace wxGUI
{
    namespace BindListEvt {
        enum : unsigned int {
            DEFAULT = 10200,
        } ;
    }

    constexpr auto SCR_HEIGHT = HEIGHT - 225 ;

    struct BindListPanel::Impl
    {
        ums_t<wxTextCtrl*> keybinds{} ;
        ums_t<wxTextCtrl*> cmds{} ;
    } ;

    constexpr auto DEFAULT_TEXT = wxT("Load Failed") ;

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
            auto add = [&sizer, &flags, &map, this](const auto func_name) {
                using namespace UITrans ;
                sizer->Add(new wxStaticText(this, wxID_ANY, trans(func_name)), flags) ;
                map[func_name] = new wxTextCtrl(this, wxID_ANY, DEFAULT_TEXT) ;
                sizer->Add(map[std::move(func_name)], flags) ;
            } ;

            add("change_to_normal") ;
            add("change_to_insert") ;
            add("change_to_visual") ;
            add("change_to_editor") ;
            add("change_to_command") ;
            add("select_all") ;
            add("move_left") ;
            add("move_right") ;
            add("move_up") ;
            add("move_down") ;
            add("jump_to_left") ;
            add("jump_to_right") ;
            add("jump_to_top") ;
            add("jump_to_bottom") ;
            add("jump_to_xcenter") ;
            add("jump_to_ycenter") ;
            add("jump_to_any") ;
            add("jump_to_active_window") ;
            add("scroll_up") ;
            add("scroll_down") ;
            add("scroll_mid_up") ;
            add("scroll_mid_down") ;
            add("scroll_page_up") ;
            add("scroll_page_down") ;
            add("scroll_left") ;
            add("scroll_right") ;
            add("scroll_mid_left") ;
            add("scroll_mid_right") ;
            add("click_left") ;
            add("click_right") ;
            add("cb_copy") ;
            add("cb_paste") ;
            add("cb_cut") ;
            add("cb_delete") ;
            add("sc_redo") ;
            add("sc_undo") ;
            add("switch_vdesktop_to_left") ;
            add("switch_vdesktop_to_right") ;
            add("search_pattern") ;
            add("close_current_window") ;
            add("switch_window") ;
            add("maximize_current_window") ;
            add("minimize_current_window") ;
            add("snap_current_window_to_left") ;
            add("snap_current_window_to_right") ;
            add("open_new_current_window") ;
            add("move_to_next_page") ;
            add("move_to_prev_page") ;

            //Editor
            add("change_to_edi_normal") ;
            add("change_to_edi_insert") ;
            add("change_to_edi_bkinsert") ;
            add("change_to_edi_nlinsert") ;
            add("change_to_edi_visual") ;
            add("change_to_edi_line_visual") ;
            add("edi_move_caret_left") ;
            add("edi_move_caret_right") ;
            add("edi_move_caret_up") ;
            add("edi_move_caret_down") ;
            add("edi_move_caret_Nwords_forward") ;
            add("edi_move_caret_Nwords_backward") ;
            add("edi_move_caret_NWORDS_forward") ;
            add("edi_move_caret_NWORDS_backward") ;
            add("edi_jump_caret_to_BOL") ;
            add("edi_jump_caret_to_EOL") ;
            add("edi_jump_caret_to_nline_default_BOF") ;
            add("edi_jump_caret_to_nline_default_EOF") ;

            SetSizer(sizer) ;
            SetScrollRate(0, 5) ;
            SetMinSize(wxSize(350, SCR_HEIGHT)) ; //if use auto WIDTH, objects is hidden by scroll-bar.
        }
    } ;

    class BindListPanel::CmdScrWindow : public wxScrolledWindow
    {
    public:
        explicit CmdScrWindow(wxWindow* parent, wxWindowID id, ums_t<wxTextCtrl*>& map)
        : wxScrolledWindow(parent, id)
        {
            wxSizerFlags flags ;
            flags.Border(wxALL, BORDER) ;

            auto sizer = new wxFlexGridSizer(2) ;

            auto add = [&sizer, &flags, &map, this](const auto func_name) {
                using namespace UITrans ;
                sizer->Add(new wxStaticText(this, wxID_ANY, trans(func_name)), flags) ;
                map[func_name] = new wxTextCtrl(this, wxID_ANY, DEFAULT_TEXT) ;
                sizer->Add(map[std::move(func_name)], flags) ;
            } ;

            add("save_opened_file") ;
            add("close_opened_file") ;
            add("close_current_window") ;
            add("switch_window") ;
            add("maximize_current_window") ;
            add("minimize_current_window") ;
            add("snap_current_window_to_left") ;
            add("snap_current_window_to_right") ;
            add("open_new_current_window") ;
            add("reload_current_window") ;
            add("start_shell") ;
            add("start_any_app") ;
            add("show_config_window") ;
            add("exit_config_window") ;
            add("open_other_file") ;
            add("make_dir") ;

            SetScrollRate(0, 5) ;
            SetSizer(sizer) ;
            SetMinSize(wxSize(315, SCR_HEIGHT)) ;
        }
    } ;

    BindListPanel::BindListPanel(wxBookCtrlBase* const p_book_ctrl)
    : wxPanel(p_book_ctrl),
      pimpl(std::make_unique<Impl>())
    {
        using namespace UITrans ;
        p_book_ctrl->AddPage(this, trans(Label::PREF_BINDLIST)) ;

        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        //this wrapper is needed to add default button.
        auto bindlist_sizer_wrapper = new wxBoxSizer(wxVERTICAL) ;
        {
            auto bindlist_sizer = new wxBoxSizer(wxHORIZONTAL) ;

            auto kb_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, trans(Label::PREF_BINDLIST_HOTKEYS)) ;
            kb_sizer_wrapper->Add(new HotkeyScrWindow(this, wxID_ANY, pimpl->keybinds), flags) ;
            bindlist_sizer->Add(kb_sizer_wrapper, flags) ;

            auto cmd_sizer_wrapper = new wxStaticBoxSizer(wxVERTICAL, this, trans(Label::PREF_BINDLIST_COMMANDS)) ;
            cmd_sizer_wrapper->Add(new CmdScrWindow(this, wxID_ANY, pimpl->cmds), flags) ;
            bindlist_sizer->Add(cmd_sizer_wrapper, flags) ;

            bindlist_sizer_wrapper->Add(bindlist_sizer, flags) ;
        }

        bindlist_sizer_wrapper->AddStretchSpacer() ;

        auto defbtn_flags = flags ;
        defbtn_flags.Align(wxALIGN_RIGHT) ;
        bindlist_sizer_wrapper->Add(new wxButton(this,
            BindListEvt::DEFAULT, trans(Label::PREF_RETURN_TO_DEFAULT)), defbtn_flags) ;

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

            auto catch_except = [](auto& e, auto& index) {
                ERROR_STREAM << e.what() << ": " << index << " is invalid index. (BindListPanel::load_core)\n" ;
            } ;

            //wxTextCtrl
            for(auto& p : pimpl->keybinds) {
                try {
                    p.second->Clear() ;
                    *p.second << hk.at(p.first) ;
                }
                catch(std::out_of_range& e) {
                    catch_except(e, p.first) ;
                    *p.second << DEFAULT_TEXT ;
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
                    *p.second << DEFAULT_TEXT ;
                    continue ;
                }
            }
        }
        catch(std::out_of_range& e) {
            ERROR_STREAM << e.what() << ": cannot load config (BindListPanel::load_core)\n" ;
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