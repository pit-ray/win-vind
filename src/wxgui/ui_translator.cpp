#include "ui_translator.hpp"

#include <unordered_map>
#include <string>

#include "pref_parser.hpp"
#include "wx_constant.hpp"

namespace UITrans
{
    //global static object is not defined as the fastest initialization,
    //so locate static objects used general purposes at local in order to define by called its functions.
    const wxString& trans(const Label index) {
        static const auto ui_language = PrefParser::load_uilang() ;
        static const wxString DEFAULT_LABEL{"CANNOT FOUND LABEL"} ;

        static const std::unordered_map<Label, wxString> lb2jp {
            {Label::SYSTRAY_PREFERENCES, wxT("設定")},
            {Label::SYSTRAY_ABOUT,       wxT("win-vindについて")},
            {Label::SYSTRAY_EXIT,        wxT("終了")},

            {Label::PREF_SETTINGS,                          wxT("詳細設定")},
            {Label::PREF_SETTINGS_COMMON,                   wxT("共通")},
            {Label::PREF_SETTINGS_COMMON_GUILANG,           wxT("GUIの言語")},
            {Label::PREF_SETTINGS_COMMON_GUILANG_JP,        wxT("日本語")},
            {Label::PREF_SETTINGS_COMMON_GUILANG_US,        wxT("英語")},
            {Label::PREF_SETTINGS_COMMON_ICONSTYLE,         wxT("アイコンのスタイル")},
            {Label::PREF_SETTINGS_COMMON_ICONSTYLE_DARK,    wxT("ダーク")},
            {Label::PREF_SETTINGS_COMMON_ICONSTYLE_LIGHT,   wxT("ライト")},
            {Label::PREF_SETTINGS_COMMON_KBTYPE,            wxT("キーボード配列")},
            {Label::PREF_SETTINGS_COMMON_KBTYPE_US,         wxT("英語配列(101/102)")},
            {Label::PREF_SETTINGS_COMMON_KBTYPE_JP,         wxT("日本語配列(106/109)")},
            {Label::PREF_SETTINGS_COMMON_KBTYPE_CUSTOM,     wxT("カスタム")},
            {Label::PREF_SETTINGS_HOTKEYS,                  wxT("ホットキー")},
            {Label::PREF_SETTINGS_COMMANDS,                 wxT("コマンド")},

            {Label::PREF_BINDLIST,          wxT("バインドリスト")},
            {Label::PREF_BINDLIST_HOTKEYS,  wxT("ホットキー")},
            {Label::PREF_BINDLIST_COMMANDS, wxT("コマンド")},

            {Label::PREF_SHORTCUTAPPS,          wxT("アプリのショートカット")},
            {Label::PREF_SHORTCUTAPPS_SHELL,    wxT("コンソール (shellコマンド)")},
            {Label::PREF_SHORTCUTAPPS_NAME,     wxT("名前")},
            {Label::PREF_SHORTCUTAPPS_PATH,     wxT("ファイルの場所")},
            {Label::PREF_SHORTCUTAPPS_ADD,      wxT("追加")},
            {Label::PREF_SHORTCUTAPPS_DEL,      wxT("削除")},

            {Label::PREF_OPTIONS,          wxT("オプション")},
            {Label::PREF_OPTIONS_OPTS,     wxT("オプションの切り替え")},
            {Label::PREF_OPTIONS_DISPCMDS, wxT("コマンドの画面表示")},

            {Label::PREF_OPTIONS_DISPCMDS_POS_UPPERLEFT,    wxT("左上")},
            {Label::PREF_OPTIONS_DISPCMDS_POS_UPPERMID,     wxT("上中央")},
            {Label::PREF_OPTIONS_DISPCMDS_POS_UPPERRIGHT,   wxT("右上")},
            {Label::PREF_OPTIONS_DISPCMDS_POS_MIDLEFT,      wxT("左中央")},
            {Label::PREF_OPTIONS_DISPCMDS_POS_CENTER,       wxT("中央")},
            {Label::PREF_OPTIONS_DISPCMDS_POS_MIDRIGHT,     wxT("右中央")},
            {Label::PREF_OPTIONS_DISPCMDS_POS_LOWERLEFT,    wxT("左下")},
            {Label::PREF_OPTIONS_DISPCMDS_POS_LOWERMID,     wxT("下中央")},
            {Label::PREF_OPTIONS_DISPCMDS_POS_LOWERRIGHT,   wxT("右下")},

            {Label::PREF_RETURN_TO_DEFAULT, wxT("初期設定に戻す")},

            {Label::PREF_OK,     wxT("OK")},
            {Label::PREF_CANCEL, wxT("キャンセル")},
            {Label::PREF_APPLY,  wxT("適用")}
        } ;

        static const std::unordered_map<Label, wxString> lb2us {
            {Label::SYSTRAY_PREFERENCES, "Preferences"},
            {Label::SYSTRAY_ABOUT,       "About"},
            {Label::SYSTRAY_EXIT,        "Exit"},

            {Label::PREF_SETTINGS,                          "Settings"},
            {Label::PREF_SETTINGS_COMMON,                   "Common"},
            {Label::PREF_SETTINGS_COMMON_GUILANG,           "GUI Language"},
            {Label::PREF_SETTINGS_COMMON_GUILANG_JP,        "Japanese"},
            {Label::PREF_SETTINGS_COMMON_GUILANG_US,        "English"},
            {Label::PREF_SETTINGS_COMMON_ICONSTYLE,         "Icon Style"},
            {Label::PREF_SETTINGS_COMMON_ICONSTYLE_DARK,    "Dark"},
            {Label::PREF_SETTINGS_COMMON_ICONSTYLE_LIGHT,   "Light"},
            {Label::PREF_SETTINGS_COMMON_KBTYPE,            "Keyboard Type"},
            {Label::PREF_SETTINGS_COMMON_KBTYPE_US,         "US(101/102)"},
            {Label::PREF_SETTINGS_COMMON_KBTYPE_JP,         "JP(106/109)"},
            {Label::PREF_SETTINGS_COMMON_KBTYPE_CUSTOM,     "Custom"},
            {Label::PREF_SETTINGS_HOTKEYS,                  "Hot Keys"},
            {Label::PREF_SETTINGS_COMMANDS,                 "Commands"},

            {Label::PREF_BINDLIST,          "Bind Lists"},
            {Label::PREF_BINDLIST_HOTKEYS,  "Hot Keys"},
            {Label::PREF_BINDLIST_COMMANDS, "Commands"},

            {Label::PREF_SHORTCUTAPPS,          "Shortcut Apps"},
            {Label::PREF_SHORTCUTAPPS_SHELL,    "console (command: 'shell')"},
            {Label::PREF_SHORTCUTAPPS_NAME,     "Name"},
            {Label::PREF_SHORTCUTAPPS_PATH,     "Path"},
            {Label::PREF_SHORTCUTAPPS_ADD,      "Add"},
            {Label::PREF_SHORTCUTAPPS_DEL,      "Delete"},

            {Label::PREF_OPTIONS,          "Options"},
            {Label::PREF_OPTIONS_OPTS,     "Options"},
            {Label::PREF_OPTIONS_DISPCMDS, "Display Commands"},

            {Label::PREF_OPTIONS_DISPCMDS_POS_UPPERLEFT,    "UpperLeft"},
            {Label::PREF_OPTIONS_DISPCMDS_POS_UPPERMID,     "UpperMid"},
            {Label::PREF_OPTIONS_DISPCMDS_POS_UPPERRIGHT,   "UpperRight"},
            {Label::PREF_OPTIONS_DISPCMDS_POS_MIDLEFT,      "MidLeft"},
            {Label::PREF_OPTIONS_DISPCMDS_POS_CENTER,       "Center"},
            {Label::PREF_OPTIONS_DISPCMDS_POS_MIDRIGHT,     "MidRight"},
            {Label::PREF_OPTIONS_DISPCMDS_POS_LOWERLEFT,    "LowerLeft"},
            {Label::PREF_OPTIONS_DISPCMDS_POS_LOWERMID,     "LowerMid"},
            {Label::PREF_OPTIONS_DISPCMDS_POS_LOWERRIGHT,   "LowerRight"},

            {Label::PREF_RETURN_TO_DEFAULT, "Return to Default"},

            {Label::PREF_OK,     "OK"},
            {Label::PREF_CANCEL, "Cancel"},
            {Label::PREF_APPLY,  "Apply"}
        } ;

        try {
            if(ui_language == wxGUI::Language::Japanese) {
                return lb2jp.at(index) ;
            }

            return lb2us.at(index) ;
        }
        catch(std::out_of_range& e) {
            return DEFAULT_LABEL ;
        }
    }

    const wxString& trans(const std::string id) {
        static const auto ui_language = PrefParser::load_uilang() ;
        static const wxString DEFAULT_LABEL{"CANNOT FOUND LABEL"} ;

        static const std::unordered_map<std::string, wxString> id2jp {
            {"change_to_normal",             wxT("ノーマルモード")},
            {"change_to_insert",             wxT("インサートモード")},
            {"change_to_visual",             wxT("ヴィジュアルモード")},
            {"change_to_editor",             wxT("エディタモード")},
            {"change_to_command",            wxT("コマンドモード")},
            {"select_all",                   wxT("全選択")},
            {"move_left",                    wxT("左にカーソル移動")},
            {"move_right",                   wxT("右にカーソル移動")},
            {"move_up",                      wxT("上にカーソル移動")},
            {"move_down",                    wxT("下にカーソル移動")},
            {"jump_to_left",                 wxT("左端へカーソル移動")},
            {"jump_to_right",                wxT("右端へカーソル移動")},
            {"jump_to_top",                  wxT("上端へカーソル移動")},
            {"jump_to_bottom",               wxT("下端へカーソル移動")},
            {"jump_to_xcenter",              wxT("横中央へカーソル移動")},
            {"jump_to_ycenter",              wxT("縦中央へカーソル移動")},
            {"jump_to_any",                  wxT("キー配置でカーソル移動")},
            {"jump_to_active_window",        wxT("選択ウィンドウへカーソル移動")},
            {"scroll_up",                    wxT("上スクロール")},
            {"scroll_down",                  wxT("下スクロール")},
            {"scroll_mid_up",                wxT("上スクロール(ページ半分)")},
            {"scroll_mid_down",              wxT("下スクロール(ページ半分)")},
            {"scroll_page_up",               wxT("上スクロール(1ページ分)")},
            {"scroll_page_down",             wxT("下スクロール(1ページ分)")},
            {"scroll_left",                  wxT("左スクロール")},
            {"scroll_right",                 wxT("右スクロール")},
            {"scroll_mid_left",              wxT("左スクロール(ページ半分)")},
            {"scroll_mid_right",             wxT("右スクロール(ページ半分)")},
            {"click_left",                   wxT("左クリック")},
            {"click_right",                  wxT("右クリック")},
            {"cb_copy",                      wxT("コピー")},
            {"cb_paste",                     wxT("貼り付け")},
            {"cb_cut",                       wxT("切り取り")},
            {"cb_delete",                    wxT("Delete")},
            {"sc_redo",                      wxT("やり直す")},
            {"sc_undo",                      wxT("元に戻す")},
            {"switch_vdesktop_to_left",      wxT("左の仮想デスクトップへ移動")},
            {"switch_vdesktop_to_right",     wxT("右の仮想デスクトップへ移動")},
            {"search_pattern",               wxT("文字検索")},
            {"close_current_window",         wxT("ウィンドウを閉じる")},
            {"switch_window",                wxT("ウィンドウを切り替える")},
            {"maximize_current_window",      wxT("ウィンドウ最大化")},
            {"minimize_current_window",      wxT("ウィンドウ最小化")},
            {"snap_current_window_to_left",  wxT("ウィンドウを画面の左半分にする")},
            {"snap_current_window_to_right", wxT("ウィンドウを画面の右半分にする")},
            {"move_to_next_page",            wxT("次のページへ進む")},
            {"move_to_prev_page",            wxT("前のページに戻る")},

            //Editor
            {"change_to_edi_normal",                wxT("[E] ノーマルモード")},
            {"change_to_edi_insert",                wxT("[E] インサートモード")},
            {"change_to_edi_bkinsert",              wxT("[E] バックインサートモード")},
            {"change_to_edi_nlinsert",              wxT("[E] 改行インサートモード")},
            {"change_to_edi_visual",                wxT("[E] ビジュアルモード")},
            {"change_to_edi_line_visual",           wxT("[E] ビジュアルモード(行単位)")},
            {"edi_move_caret_left",                 wxT("[E] 左へキャレット移動")},
            {"edi_move_caret_right",                wxT("[E] 右へキャレット移動")},
            {"edi_move_caret_up",                   wxT("[E] 上へキャレット移動")},
            {"edi_move_caret_down",                 wxT("[E] 下へキャレット移動")},
            {"edi_move_caret_Nwords_forward",       wxT("[E] N単語分先に進む")},
            {"edi_move_caret_Nwords_backward",      wxT("[E] N単語分前に戻る")},
            {"edi_move_caret_NWORDS_forward",       wxT("[E] N単語分先に進む(空白)")},
            {"edi_move_caret_NWORDS_backward",      wxT("[E] N単語分前に戻る(空白)")},
            {"edi_jump_caret_to_BOL",               wxT("[E] 行頭へキャレット移動")},
            {"edi_jump_caret_to_EOL",               wxT("[E] 行末へキャレット移動")},
            {"edi_jump_caret_to_nline_default_BOF", wxT("[E] N行目にキャレット移動 (BOF)")},
            {"edi_jump_caret_to_nline_default_EOF", wxT("[E] N行目にキャレット移動 (EOF)")},

            {"save_opened_file",             wxT("ファイルを保存")},
            {"close_opened_file",            wxT("ドキュメント(タブ)を閉じる")},
            {"close_current_window",         wxT("ウィンドウを閉じる")},
            {"switch_window",                wxT("ウィンドウを切り替える")},
            {"maximize_current_window",      wxT("ウィンドウ最大化")},
            {"minimize_current_window",      wxT("ウィンドウ最小化")},
            {"snap_current_window_to_left",  wxT("ウィンドウを画面の左半分にする")},
            {"snap_current_window_to_right", wxT("ウィンドウを画面の右半分にする")},
            {"start_shell",                  wxT("コマンドプロンプトを起動")},
            {"start_any_app",                wxT("任意のアプリを実行")},
            {"show_config_window",           wxT("設定ウィンドウを開く")},
            {"exit_config_window",           wxT("アプリケーションを終了する")},
            {"open_other_file",              wxT("ファイルを開く")},
            {"make_dir",                     wxT("ディレクトリを作成する")},
            {"open_new_current_window",      wxT("新しいウィンドウを開く")},
            {"reload_current_window",        wxT("ウィンドウを再読み込み")},

            {"display_cmd",     wxT("コマンドの画面表示")},
            {"autotrack_popup", wxT("ポップアップ自動追従")},

            {"yscroll_speed",           wxT("縦スクロールのスピード")},
            {"xscroll_speed",           wxT("横スクロールのスピード")},
            {"cmd_max_char",            wxT("最大文字数")},
            {"cmd_max_history_num",     wxT("コマンド履歴数")},
            {"cursor_acceleration",     wxT("カーソルの加速度")},
            {"cursor_weight",           wxT("カーソルの重さ")},
            {"cursor_max_velocity",     wxT("カーソルの最大速度")},
            {"screen_pos_buf",          wxT("カーソルジャンプ時の画面端余白")},
            {"xscroll_screen_ratio",    wxT("縦ページスクロール比")},
            {"yscroll_screen_ratio",    wxT("横ページスクロール比")},

            {"cmd_font_size",       wxT("フォントサイズ")},
            {"cmd_font_weight",     wxT("フォントの太さ")},
            {"cmd_font_color",      wxT("フォントの色")},
            {"cmd_font_bkcolor",    wxT("背景の色")},
            {"cmd_font_extra",      wxT("文字同士の隙間")},
            {"cmd_pos",             wxT("位置")},
            {"cmd_ymargin",         wxT("縦の余白")},
            {"cmd_xmargin",         wxT("横の余白")}
        } ;

        static const std::unordered_map<std::string, wxString> id2us {
            {"change_to_normal",             "Change to Normal"},
            {"change_to_insert",             "Change to Insert"},
            {"change_to_visual",             "Change to Visual"},
            {"change_to_editor",             "Change to Editor"},
            {"change_to_command",            "Change to Command"},
            {"select_all",                   "Select All"},
            {"move_left",                    "Move Cursor Left"},
            {"move_right",                   "Move Cursor Right"},
            {"move_up",                      "Move Cursor Up"},
            {"move_down",                    "Move Cursor Down"},
            {"jump_to_left",                 "Jump Cursor to Left"},
            {"jump_to_right",                "Jump Cursor to Right"},
            {"jump_to_top",                  "Jump Cursor to Top"},
            {"jump_to_bottom",               "Jump Cursor to Bottom"},
            {"jump_to_xcenter",              "Jump Cursor to Horizontal Center"},
            {"jump_to_ycenter",              "Jump Cursor to Vertical Center"},
            {"jump_to_any",                  "Jump Cursor by Keyboard Mapping"},
            {"jump_to_active_window",        "Jump Cursor to Selected Window"},
            {"scroll_up",                    "Scroll Up"},
            {"scroll_down",                  "Scroll Down"},
            {"scroll_mid_up",                "Scroll Half Page Up"},
            {"scroll_mid_down",              "Scroll Half Page Down"},
            {"scroll_page_up",               "Scroll Page Up"},
            {"scroll_page_down",             "Scroll Page Down"},
            {"scroll_left",                  "Scroll Left"},
            {"scroll_right",                 "Scroll Right"},
            {"scroll_mid_left",              "Scroll Half Page Left"},
            {"scroll_mid_right",             "Scroll Half Page Right"},
            {"click_left",                   "Click Left"},
            {"click_right",                  "Click Right"},
            {"cb_copy",                      "Copy"},
            {"cb_paste",                     "Paste"},
            {"cb_cut",                       "Cut"},
            {"cb_delete",                    "Delete"},
            {"sc_redo",                      "Redo"},
            {"sc_undo",                      "Undo"},
            {"switch_vdesktop_to_left",      "Switch to Left Virtual Desktop"},
            {"switch_vdesktop_to_right",     "Switch to Right Virtual Desktop"},
            {"search_pattern",               "Search Pattern"},
            {"close_current_window",         "Close Current Window"},
            {"switch_window",                "Switch Window"},
            {"maximize_current_window",      "Maximize Current Window"},
            {"minimize_current_window",      "Minimize Current Window"},
            {"snap_current_window_to_left",  "Snap Current Window to Left"},
            {"snap_current_window_to_right", "Snap Current Window to Right"},
            {"move_to_next_page",            "Move to Next Page"},
            {"move_to_prev_page",            "Move to Previous Page"},

            //Editor
            {"change_to_edi_normal",                "[E] Change to Editor Normal"},
            {"change_to_edi_insert",                "[E] Change to Editor Insert"},
            {"change_to_edi_bkinsert",              "[E] Change to Editor BackInsert"},
            {"change_to_edi_nlinsert",              "[E] Change to Editor NewLineInsert"},
            {"change_to_edi_visual",                "[E] Change to Editor Visual"},
            {"change_to_edi_line_visual",           "[E] Change to Editor Visual (Line)"},
            {"edi_move_caret_left",                 "[E] Move Caret Left"},
            {"edi_move_caret_right",                "[E] Move Caret Right"},
            {"edi_move_caret_up",                   "[E] Move Caret Up"},
            {"edi_move_caret_down",                 "[E] Move Caret Down"},
            {"edi_move_caret_Nwords_forward",       "[E] N-words forward"},
            {"edi_move_caret_Nwords_backward",      "[E] N-words backward"},
            {"edi_move_caret_NWORDS_forward",       "[E] N-WORDS forward"},
            {"edi_move_caret_NWORDS_backward",      "[E] N-WORDS backward"},
            {"edi_jump_caret_to_BOL",               "[E] Jump Caret to BOL"},
            {"edi_jump_caret_to_EOL",               "[E] Jump Caret to EOL"},
            {"edi_jump_caret_to_nline_default_BOF", "[E] Jump Caret to N-Line (BOF)"},
            {"edi_jump_caret_to_nline_default_EOF", "[E] Jump Caret to N-Line (EOF)"},

            {"save_opened_file",             "Save Opened File"},
            {"close_opened_file",            "Close Opened File(Tab)"},
            {"close_current_window",         "Close Current Window"},
            {"switch_window",                "Switch Window"},
            {"maximize_current_window",      "Maximize Current Window"},
            {"minimize_current_window",      "Minimize Current Window"},
            {"snap_current_window_to_left",  "Snap Current Window to Left"},
            {"start_shell",                  "Start Shell"},
            {"start_any_app",                "Start Shortcut App"},
            {"show_config_window",           "Show My Config Window"},
            {"exit_config_window",           "Exit This Application"},
            {"open_other_file",              "Open Other File"},
            {"make_dir",                     "Make Directory"},
            {"open_new_current_window",      "Open New Window"},
            {"reload_current_window",        "Reload Current Window"},

            {"display_cmd",     "Display Commands"},
            {"autotrack_popup", "Autotrack Popup"},

            {"yscroll_speed",           "Vertical Scroll Speed"},
            {"xscroll_speed",           "Horizontal Scroll Speed"},
            {"cmd_max_char",            "Max Char"},
            {"cmd_max_history_num",     "Max Command History Num"},
            {"cursor_acceleration",     "Cursor Acceleration"},
            {"cursor_weight",           "Cursor Weight"},
            {"cursor_max_velocity",     "Cursor Velocity"},
            {"screen_pos_buf",          "Screen Position Buffer at Jump Cursor"},
            {"xscroll_screen_ratio",    "Vertical Page Scroll Ratio"},
            {"yscroll_screen_ratio",    "Horizontal Page Scroll Ratio"},

            {"cmd_font_size",       "Font Size"},
            {"cmd_font_weight",     "Font Weight"},
            {"cmd_font_color",      "Font Color"},
            {"cmd_font_bkcolor",    "Font Backgrount Color"},
            {"cmd_font_extra",      "Font Extra"},
            {"cmd_pos",             "Position"},
            {"cmd_ymargin",         "Vertical Margin"},
            {"cmd_xmargin",         "Horizontal Margin"}
        } ;

        try {
            if(ui_language == wxGUI::Language::Japanese) {
                return id2jp.at(id) ;
            }

            return id2us.at(id) ;
        }
        catch(std::out_of_range& e) {
            return DEFAULT_LABEL ;
        }
    }
}