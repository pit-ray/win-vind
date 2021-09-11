---
layout: page
title: Functions - win-vind
nav: Functions
show_in_menu: false
disable_anchors: true
parent: Cheat Sheet
description: All supported functions list
---

### Mode

|ID|Feature|
|:---:|:---|
|**to_command**|Command Mode|
|**to_edi_normal**|Editor Normal Mode|
|**to_edi_visual**|Visual Mode|
|**to_edi_visual_line**|Visual-Line Mode|
|**to_gui_visual**|GUI Visual Mode|
|**to_insert**|Insert Mode|
|**to_instant_gui_normal**|Temporarily switches to GUI mode and performs matching, which can be used as a map-leader.|
|**to_resident**|Resident Mode|


### Mouse

|ID|Feature|
|:---:|:---|
|**click_left**|Left button of a mouse click|
|**click_right**|Right button of a mouse click|
|**easy_click_hover**|EasyClick (Hovering)|
|**easy_click_left**|EasyClick (Left Click)|
|**easy_click_mid**|EasyClick (Mid Click)|
|**easy_click_right**|EasyClick (Right Click)|
|**jump_cursor_to_active_window**|Jump the mouse cursor to the foreground window|
|**jump_cursor_to_bottom**|Jump the mouse cursor to bottom|
|**jump_cursor_to_hcenter**|Jump the mouse cursor to horizontal center|
|**jump_cursor_to_left**|Jump the Mouse cursor to left|
|**jump_cursor_to_right**|Jump the mouse cursor to right|
|**jump_cursor_to_top**|Jump the mouse cursor to top|
|**jump_cursor_to_vcenter**|Jump the mouse cursor to vertical center|
|**jump_cursor_with_keybrd_layout**|Jump the mouse cursor by keyboard mapping|
|**move_cursor_down**|Move the Mouse Cursor Down|
|**move_cursor_left**|Move the mouse cursor to left|
|**move_cursor_right**|Move the mouse cursor to right|
|**move_cursor_up**|Move the mouse cursor up|
|**scroll_down**|Scroll a mouse wheel down|
|**scroll_down_halfpage**|Scroll a mouse wheel down with a half page|
|**scroll_down_onepage**|Scroll a mouse wheel down With a page|
|**scroll_left**|Scroll a mouse wheel left|
|**scroll_left_halfpage**|Scroll a mouse wheel left With a half page|
|**scroll_right**|Scroll a mouse wheel right|
|**scroll_right_halfpage**|Scroll a mouse wheel right with a half page|
|**scroll_up**|Scroll a mouse wheel up|
|**scroll_up_halfpage**|Scroll a mouse wheel up with a half page|
|**scroll_up_onepage**|Scroll a mouse wheel up with a page|

### Hotkey

|ID|Feature|
|:---:|:---|
|**backward_ui_navigation**|Backward UI Navigation|
|**decide_focused_ui_object**|Decide a focused UI object|
|**forward_ui_navigation**|Forward UI Navigation|
|**goto_next_page**|Forward a next page|
|**goto_prev_page**|Backward a previous page|
|**hotkey_backspace**|BackSpace|
|**hotkey_copy**|Copy|
|**hotkey_cut**|Cut|
|**hotkey_delete**|Delete|
|**hotkey_paste**|Paste|
|**open**|Open another file|
|**open_startmenu**|Open a StartMenu|
|**open_startmenu**|Open a start menu|
|**redo**|Redo|
|**save**|Save a current file|
|**search_pattern**|Search Pattern|
|**select_all**|Select all|
|**start_explorer**|Start a Explorer|
|**undo**|Undo|

### Virtual Desktop

|ID|Feature|
|:---:|:---|
|**close_current_vdesktop**|Close a current virtual desktop|
|**create_new_vdesktop**|Create a new virtual desktop|
|**switch_to_left_vdesktop**|Switch to a left virtual desktop|
|**switch_to_right_vdesktop**|Switch to a right virtual desktop|
|**taskview**|Task View|

### Tab

|ID|Feature|
|:---:|:---|
|**close_current_tab**|Close a current tab|
|**open_new_tab**|Open a new tab|
|**switch_to_left_tab**|Switch to a left tab|
|**switch_to_right_tab**|Switch to a right tab|


### Window

|ID|Feature|
|:---:|:---|
|**arrange_windows**|Arrange windows with tile style|
|**close_current_window**|Close a current window|
|**decrease_window_height**|Decrease the height of a window|
|**decrease_window_width**|Decrease the width of a window|
|**exchange_window_with_nearest**|Exchange a window with the nearest window|
|**increase_window_height**|Increase the height of a window|
|**increase_window_width**|Increase the width of a window|
|**maximize_current_window**|Maximize a current window|
|**minimize_current_window**|Minimize a current window|
|**open_new_window**|Open a new window|
|**open_new_window_with_hsplit**|Open a new window with horizontal split|
|**open_new_window_with_vsplit**|Open a new window with vertical split|
|**reload_current_window**|Reload a current window|
|**resize_window_height**|Resize the height of a window|
|**resize_window_width**|Resize the width of a window|
|**rotate_windows**|Rotate windows in a current monitor|
|**rotate_windows_in_reverse**|Rotate windows in a current monitor in reverse|
|**select_left_window**|Select a left window|
|**select_lower_window**|Select a lower window|
|**select_right_window**|Select a right window|
|**select_upper_window**|Select an upper window|
|**snap_current_window_to_bottom**|Snap a current window to bottom|
|**snap_current_window_to_left**|Snap a current window to left|
|**snap_current_window_to_right**|Snap a current window to right|
|**snap_current_window_to_top**|Snap a current window to top|
|**switch_window**|Switch a window|
|**window_resizer**|Start window resizer. It respects Vim plugin <a href="https://github.com/simeji/winresizer">simeji/winresizer</a>.|

### File

|ID|Note|
|:---:|:---|
|**makedir**|Create a directory. If you call it with a relative path such as `:mkdir foo`, it creates it in the explorer directory. If no explorer is found, it creates it in `~/Desktop/foo`. If you call it with an absolute path like `:mkdir C:/Users/You/Desktop/bar`, it will create a directory along the path, but not recursively.|

### Process

|ID|Note|
|:---:|:---|
|**execute**|Open file with the associated application. It is wrapper of **ShellExecute**. Therefore, you can open any format files and URLs. For example, `:e ~/.vimrc` or `:e https://www.google.com`.|
|**exit_config_gui**|Exit win-vind|
|**start_external**|Start an external application. This environment variable is dependent on the application specified in the shell option. By appending `;` at the end, it keeps the console window without closing immediately. If the explorer is the foreground window, the current directory of a terminal will be that directory.|
|**start_shell**|Start a terminal. If the explorer is the foreground window, the current directory of a terminal will be that directory.|

### Option

|ID|Note|
|:---:|:---|
|**disable_targeting_of_dedicate_to_window**|Disable targeting (Dedicate to One Window)|
|**enable_targeting_of_dedicate_to_window**|Enable targeting (Dedicate to One Window)|

If **Dedicate to One window** enables, you can select one window with Enable Targeting function. In this case, it makes the mode automatically switch to Editor Normal Mode on the targeting window. When the foreground window change to another, it makes the mode switch to Insert Mode. The targeting becomes disable with Disable Targeting function. In other words, this feature transforms some normal editors to fake Vim. The computing cost is so small.

### Vim Emulation

**Mode**  

|ID|Feature|
|:---:|:---|
|**to_insert_BOL**|Insert to begin of line|
|**to_insert_EOL**|Append end of line|
|**to_insert_append**|Append after a caret|
|**to_insert_nlabove**|Begin new line above a caret|
|**to_insert_nlbelow**|Begin new line below a caret|

**Move**  

|ID|Feature|
|:---:|:---|
|**move_caret_down**|Move the caret down|
|**move_caret_left**|Move the caret to left|
|**move_caret_nonblank_word_backward**|Backward with WORDS|
|**move_caret_nonblank_word_forward**|Forward with WORDS|
|**move_caret_right**|Move the caret to right|
|**move_caret_up**|Move the caret up|
|**move_caret_word_backward**|Backward with words|
|**move_caret_word_forward**|Forward with words|

**Jump**  

|ID|Feature|
|:---:|:---|
|**jump_caret_to_BOF**|Jump the caret to BOF|
|**jump_caret_to_BOL**|Jump the caret to begin of line|
|**jump_caret_to_EOF**|Jump the caret to EOF|
|**jump_caret_to_EOL**|Jump the caret to end of line|

**Edit**  

|ID|Feature|
|:---:|:---|
|**change_char**|Change Characters|
|**change_highlight_text**|Change highlighted texts|
|**change_line**|Change Lines|
|**change_until_EOL**|Change until EOL|
|**change_with_motion**|Change texts with motion|
|**delete_after**|Delete chars after the caret|
|**delete_before**|Delete chars before the caret|
|**delete_highlight_text**|Delete highlighted texts|
|**delete_line**|Delete lines|
|**delete_line_until_EOL**|Delete texts until end of line|
|**delete_with_motion**|Delete texts with motion|
|**join_next_line**|Join a next line|
|**put_after**|Put texts after the caret|
|**put_before**|Put texts before the caret|
|**repeat_last_change**|Repeat last simple change (Dot command)|
|**replace_char**|Replace a char|
|**replace_sequence**|Replace Mode|
|**switch_char_case**|Switch char case|
|**yank_highlight_text**|Yank highlighted texts|
|**yank_line**|Yank lines|
|**yank_with_motion**|Yank lines with motion|


### System Command

|ID|Note|
|:---:|:---|
|**system_command_comclear**|System Command comclear|
|**system_command_command**|System Command command|
|**system_command_delcommand**|System Command delcommand|
|**system_command_map**|System Command map|
|**system_command_mapclear**|System Command mapclear|
|**system_command_noremap**|System Command noremap|
|**system_command_set**|System Command set|
|**system_command_source**|System Command source|
|**system_command_unmap**|System Command unmap|

<br>
<br>
<br>
<br>
