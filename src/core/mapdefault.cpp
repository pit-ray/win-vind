#include "mapdefault.hpp"

namespace
{
    const std::vector<std::pair<std::string, std::string>>& gui_normal() {
        static std::vector<std::pair<std::string, std::string>> tmp = {
            {"<esc-left>", "<to_gui_normal>"},
            {"<esc-down>", "<to_resident>"},
            {"v", "<to_gui_visual>"},
            {":", "<to_command>"},
            {"I", "<to_edi_normal>"},
            {"<esc-right>", "<to_edi_normal>"},
            {"i", "<to_insert>"},

            {"V", "<select_all>"},

            {"h", "<move_cursor_left>"},
            {"<bs>", "<move_cursor_left>"},
            {"<left>", "<move_cursor_left>"},
            {"l", "<move_cursor_right>"},
            {"<space>", "<move_cursor_right>"},
            {"<right>", "<move_cursor_right>"},
            {"k", "<move_cursor_up>"},
            {"-", "<move_cursor_up>"},
            {"<up>", "<move_cursor_up>"},
            {"j", "<move_cursor_down>"},
            {"+", "<move_cursor_down>"},
            {"<down>", "<move_cursor_down>"},

            {"0", "<jump_cursor_to_left>"},
            {"^", "<jump_cursor_to_left>"},
            {"<home>", "<jump_cursor_to_left>"},
            {"$", "<jump_cursor_to_right>"},
            {"<end>", "<jump_cursor_to_right>"},
            {"gg", "<jump_cursor_to_top>"},
            {"G", "<jump_cursor_to_bottom>"},
            {"gm", "<jump_cursor_to_hcenter>"},
            {"M", "<jump_cursor_to_vcenter>"},
            {"f", "<jump_cursor_with_keybrd_layout>"},
            {"t", "<jump_cursor_to_active_window>"},

            {"<c-y>", "<scroll_up>"},
            {"<c-k>", "<scroll_up>"},
            {"<c-e>", "<scroll_down>"},
            {"<c-j>", "<scroll_down>"},
            {"<c-u>", "<scroll_up_halfpage>"},
            {"<c-d>", "<scroll_down_halfpage>"},
            {"<c-b>", "<scroll_up_onepage>"},
            {"<c-f>", "<scroll_down_onepage>"},
            {"zh", "<scroll_left>"},
            {"<c-h>", "<scroll_left>"},
            {"zl", "<scroll_right>"},
            {"<c-l>", "<scroll_right>"},
            {"zh", "<scroll_left_halfpage>"},
            {"zl", "<scroll_right_halfpage>"},

            {"o", "<click_left>"},
            {"O", "<click_left>"},
            {"a", "<click_right>"},
            {"A", "<click_right>"},
            {"FF", "<easy_click_left>"},
            {"Fo", "<easy_click_left>"},
            {"Fa", "<easy_click_right>"},
            {"Fm", "<easy_click_mid>"},
            {"Fh", "<easy_click_hover>"},

            {"y", "<hotkey_copy>"},
            {"yy", "<hotkey_copy>"},
            {"Y", "<hotkey_copy>"},
            {"p", "<hotkey_paste>"},
            {"P", "<hotkey_paste>"},
            {"dd", "<hotkey_cut>"},
            {"D", "<hotkey_cut>"},
            {"x", "<hotkey_delete>"},
            {"<Del>", "<hotkey_delete>"},
            {"X", "<hotkey_backspace>"},
            {"<C-r>", "<redo>"},
            {"u", "<undo>"},
            {"U", "<undo>"},

            {"<C-v>h", "<switch_to_left_vdesktop>"},
            {"<C-v>l", "<switch_to_right_vdesktop>"},
            {"<C-v>n", "<create_new_vdesktop>"},
            {"<C-v>q", "<close_current_vdesktop>"},
            {"<C-v>s", "<taskview>"},

            {"gT", "<switch_to_left_tab>"},
            {"gt", "<switch_to_right_tab>"},

            {"/", "<search_pattern>"},
            {"?", "<search_pattern>"},

            {"<C-w>q", "<close_current_window>"},
            {"<C-w>c", "<close_current_window>"},
            {"<C-w>s", "<switch_window>"},
            {"<C-w>e", "<window_resizer>"},
            {"<C-w>h", "<select_left_window>"},
            {"<C-w>l", "<select_right_window>"},
            {"<C-w>k", "<select_upper_window>"},
            {"<C-w>j", "<select_lower_window>"},
            {"<C-w>u", "<maximize_current_window>"},
            {"<C-w>d", "<minimize_current_window>"},
            {"<C-w>H", "<snap_current_window_to_left>"},
            {"<C-w><Left>", "<snap_current_window_to_left>"},
            {"<C-w>L", "<snap_current_window_to_right>"},
            {"<C-w><Right>", "<snap_current_window_to_right>"},
            {"<C-w>K", "<snap_current_window_to_top>"},
            {"<C-w>J", "<snap_current_window_to_bottom>"},
            {"<C-w>=", "<arrange_windows>"},
            {"<C-w>r", "<rotate_windows>"},
            {"<C-w>R", "<rotate_windows_in_reverse>"},
            {"<C-w>x", "<exchange_window_with_nearest>"},
            {"<C-w><gt>", "<increase_window_width>"},
            {"<C-w><lt>", "<decrease_window_width>"},
            {"<C-w>+", "<increase_window_height>"},
            {"<C-w>-", "<decrease_window_height>"},
            {"<C-w>n", "<open_new_window>"},

            {"<gt>", "<goto_next_page>"},
            {"<lt>", "<goto_prev_page>"},

            {"<win>", "<open_startmenu>"},
        } ;
        return tmp ;
    }
}


namespace vind
{
    namespace core
    {
        const std::vector<std::pair<std::string, std::string>>&
        get_default_map(Mode mode) {
            static auto tmp = [] {
                ModeArray<std::vector<std::pair<std::string, std::string>>> tmp_ ;
                tmp_[static_cast<int>(Mode::GUI_NORMAL)] = gui_normal() ;
                return tmp_ ;
            }() ;
            return tmp[static_cast<int>(mode)] ;
        }
    }
}
