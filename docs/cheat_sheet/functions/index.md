---
layout: page
title: Functions - win-vind
nav: Functions
show_in_menu: false
disable_anchors: true
---

### Mode

|ID|Feature|
|:---:|:---|
|`<to_command>`|Command Mode|
|`<to_edi_normal>`|Editor Normal Mode|
|`<to_edi_visual>`|Editor Visual Mode|
|`<to_edi_visual_line>`|Editor Visual-Line Mode|
|`<to_gui_visual>`|GUI Visual Mode|
|`<to_insert>`|Insert Mode|
|`<to_instant_gui_normal>`|Temporarily switches to GUI Normal Mode and performs matching, which can be used as a map-leader.|
|`<to_resident>`|Resident Mode|


### Mouse

|ID|Feature|
|:---:|:---|
|`<click_left>`|Left button of a mouse click|
|`<click_right>`|Right button of a mouse click|
|`<easy_click_hover>`|EasyClick (Hovering)|
|`<easy_click_left>`|EasyClick (Left Click)|
|`<easy_click_mid>`|EasyClick (Mid Click)|
|`<easy_click_right>`|EasyClick (Right Click)|
|`<jump_cursor_to_active_window>`|Jump the mouse cursor to the foreground window|
|`<jump_cursor_to_bottom>`|Jump the mouse cursor to bottom|
|`<jump_cursor_to_hcenter>`|Jump the mouse cursor to horizontal center|
|`<jump_cursor_to_left>`|Jump the Mouse cursor to left|
|`<jump_cursor_to_right>`|Jump the mouse cursor to right|
|`<jump_cursor_to_top>`|Jump the mouse cursor to top|
|`<jump_cursor_to_vcenter>`|Jump the mouse cursor to vertical center|
|`<jump_cursor_with_keybrd_layout>`|Jump the mouse cursor by keyboard mapping|
|`<move_cursor_down>`|Move the Mouse Cursor Down|
|`<move_cursor_left>`|Move the mouse cursor to left|
|`<move_cursor_right>`|Move the mouse cursor to right|
|`<move_cursor_up>`|Move the mouse cursor up|
|`<scroll_down>`|Scroll a mouse wheel down|
|`<scroll_down_halfpage>`|Scroll a mouse wheel down with a half page|
|`<scroll_down_onepage>`|Scroll a mouse wheel down With a page|
|`<scroll_left>`|Scroll a mouse wheel left|
|`<scroll_left_halfpage>`|Scroll a mouse wheel left With a half page|
|`<scroll_right>`|Scroll a mouse wheel right|
|`<scroll_right_halfpage>`|Scroll a mouse wheel right with a half page|
|`<scroll_up>`|Scroll a mouse wheel up|
|`<scroll_up_halfpage>`|Scroll a mouse wheel up with a half page|
|`<scroll_up_onepage>`|Scroll a mouse wheel up with a page|

### Hotkey

|ID|Feature|
|:---:|:---|
|`<backward_ui_navigation>`|Backward UI Navigation|
|`<decide_focused_ui_object>`|Decide a focused UI object|
|`<forward_ui_navigation>`|Forward UI Navigation|
|`<goto_next_page>`|Forward a next page|
|`<goto_prev_page>`|Backward a previous page|
|`<hotkey_backspace>`|BackSpace|
|`<hotkey_copy>`|Copy|
|`<hotkey_cut>`|Cut|
|`<hotkey_delete>`|Delete|
|`<hotkey_paste>`|Paste|
|`<open>`|Open another file|
|`<open_startmenu>`|Open a StartMenu|
|`<redo>`|Redo|
|`<save>`|Save a current file|
|`<search_pattern>`|Search Pattern|
|`<select_all>`|Select all|
|`<start_explorer>`|Start a Explorer|
|`<undo>`|Undo|

### Virtual Desktop

|ID|Feature|
|:---:|:---|
|`<close_current_vdesktop>`|Close a current virtual desktop|
|`<create_new_vdesktop>`|Create a new virtual desktop|
|`<switch_to_left_vdesktop>`|Switch to a left virtual desktop|
|`<switch_to_right_vdesktop>`|Switch to a right virtual desktop|
|`<taskview>`|Task View|

### Tab

|ID|Feature|
|:---:|:---|
|`<close_current_tab>`|Close a current tab|
|`<open_new_tab>`|Open a new tab|
|`<switch_to_left_tab>`|Switch to a left tab|
|`<switch_to_right_tab>`|Switch to a right tab|


### Window

|ID|Feature|
|:---:|:---|
|`<arrange_windows>`|Arrange windows with tile style|
|`<close_current_window>`|Close a current window|
|`<decrease_window_height>`|Decrease the height of a window|
|`<decrease_window_width>`|Decrease the width of a window|
|`<exchange_window_with_nearest>`|Exchange a window with the nearest window|
|`<increase_window_height>`|Increase the height of a window|
|`<increase_window_width>`|Increase the width of a window|
|`<maximize_current_window>`|Maximize a current window|
|`<minimize_current_window>`|Minimize a current window|
|`<open_new_window>`|Open a new window|
|`<open_new_window_with_hsplit>`|Open a new window with horizontal split|
|`<open_new_window_with_vsplit>`|Open a new window with vertical split|
|`<reload_current_window>`|Reload a current window|
|`<resize_window_height>`|Set the height of a window. You have to pass the pixel value as an argument using the command line.|
|`<resize_window_width>`|Set the width of a window. You have to pass the pixel value as an argument using the command line.|
|`<rotate_windows>`|Rotate windows in a current monitor|
|`<rotate_windows_in_reverse>`|Rotate windows in a current monitor in reverse|
|`<select_left_window>`|Select a left window|
|`<select_lower_window>`|Select a lower window|
|`<select_right_window>`|Select a right window|
|`<select_upper_window>`|Select an upper window|
|`<snap_current_window_to_bottom>`|Snap a current window to bottom|
|`<snap_current_window_to_left>`|Snap a current window to left|
|`<snap_current_window_to_right>`|Snap a current window to right|
|`<snap_current_window_to_top>`|Snap a current window to top|
|`<switch_window>`|Switch a window|
|`<window_resizer>`|Start window resizer. It respects Vim plugin <a href="https://github.com/simeji/winresizer">simeji/winresizer</a>.|

### File

|ID|Feature|
|:---:|:---|
|`<makedir>`|Create a directory. If you call it with a relative path such as `:mkdir foo`, it creates it in the explorer directory. If no explorer is found, it creates it in `~/Desktop/foo`. If you call it with an absolute path like `:mkdir C:/Users/You/Desktop/bar`, it will create a directory along the path recursively.|

### Process

|ID|Feature|
|:---:|:---|
|`<execute>`|Open file with the associated application. This is a wrapper for the famous Windows API, **ShellExecute**, which behaves the same as double-clicking in Explorer. Therefore, you can open any format files and URLs. For example, `:e ~/.vimrc` or `:e https://www.google.com`. If there is no argument, it will open .vindrc loaded at initialization.|
|`<exit>`|Exit win-vind|
|`<start_external>`|Start an external application. This environment variable is dependent on the application specified in the `shell` option. By appending `;` at the end, it keeps the console window without closing immediately. If the explorer is the foreground window, the current directory of a terminal will be that directory.|
|`<start_shell>`|Start a terminal. If the explorer is the foreground window, the current directory of a terminal will be that directory.|

### Option

|ID|Feature|
|:---:|:---|
|`<disable_targeting_of_dedicate_to_window>`|Disable targeting (Dedicate to One Window)|
|`<enable_targeting_of_dedicate_to_window>`|Enable targeting (Dedicate to One Window)|

### System Command

|ID|Feature|
|:---:|:---|
|`<comclear>`|System Command comclear|
|`<command>`|System Command command|
|`<delcommand>`|System Command delcommand|
|`<map>`|System Command map|
|`<mapclear>`|System Command mapclear|
|`<noremap>`|System Command noremap|
|`<set>`|System Command set|
|`<source>`|System Command source|
|`<unmap>`|System Command unmap|


### Vim Emulation

<br>

**Mode**  

|ID|Feature|Emulation|
|:---:|:---|:---:|
|`<to_insert_BOL>`|Insert to begin of line.|`I`|
|`<to_insert_EOL>`|Append end of line.|`A`|
|`<to_insert_append>`|Append after a caret.|`a`|
|`<to_insert_nlabove>`|Begin new line above a caret.|`O`|
|`<to_insert_nlbelow>`|Begin new line below a caret.|`o`|

<br>

**Move**  

|ID|Feature|Emulation|
|:---:|:---|:---:|
|`<move_caret_left>`|Move the caret to left.|`h`|
|`<move_caret_down>`|Move the caret down.|`j`|
|`<move_caret_up>`|Move the caret up|`k`|
|`<move_caret_right>`|Move the caret to right.|`l`|
|`<move_fwd_word>`|words forward for normal mode.|`w`|
|`<move_fwd_word_simple>`|words forward fast.|`w`|
|`<move_bck_word>`|words backward for normal mode.|`b`|
|`<move_bck_word_simple>`|words backward fast.|`b`|
|`<move_fwd_bigword>`|WORDS forward.|`W`|
|`<move_bck_bigword>`|WORDS backward.|`B`|
|`<move_end_word>`|Forward to the end of words.|`e`|
|`<move_end_bigword>`|Forward to the end of WORDS.|`E`|
|`<move_bckend_word>`|Backward to the end of words.|`ge`|
|`<move_bckend_bigword>`|Backward to the end of WORDS.|`gE`|


<br>

**Jump**  

|ID|Feature|Emulation|
|:---:|:---|:---:|
|`<jump_caret_to_BOF>`|Jump the caret to BOF|`gg`|
|`<jump_caret_to_BOL>`|Jump the caret to begin of line|`0`|
|`<jump_caret_to_EOF>`|Jump the caret to EOF|`G`|
|`<jump_caret_to_EOL>`|Jump the caret to end of line|`$`|

<br>

**Edit**  

|ID|Feature|Emulation|
|:---:|:---|:---:|
|`<change_char>`|Change Characters|`s`|
|`<change_highlight_text>`|Change highlighted texts|`c`, `s`, `S`|
|`<change_line>`|Change Lines|`cc`, `S`|
|`<change_until_EOL>`|Change until EOL|`C`|
|`<change_with_motion>`|Change texts with motion|`c{motion}`|
|`<delete_after>`|Delete chars after the caret|`x`|
|`<delete_before>`|Delete chars before the caret|`X`|
|`<delete_highlight_text>`|Delete highlighted texts|`d`, `x`, `X`|
|`<delete_line>`|Delete lines|`dd`|
|`<delete_line_until_EOL>`|Delete texts until end of line|`D`|
|`<delete_with_motion>`|Delete texts with motion|`d{motion}`|
|`<join_next_line>`|Join a next line|`J`|
|`<put_after>`|Put texts after the caret|`p`|
|`<put_before>`|Put texts before the caret|`P`|
|`<repeat_last_change>`|Repeat last simple change|`.`|
|`<replace_char>`|Replace a char|`r`|
|`<replace_sequence>`|Replace Mode|`R`|
|`<switch_char_case>`|Switch char case|`~`|
|`<yank_highlight_text>`|Yank highlighted texts|`y`|
|`<yank_line>`|Yank lines|`yy`, `Y`|
|`<yank_with_motion>`|Yank lines with motion|`y{motion}`|


<br>
<br>
<br>
<br>
