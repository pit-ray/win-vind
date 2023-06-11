---
layout: page
title: Functions - win-vind
nav: Functions
show_in_menu: false
disable_anchors: true
---

## Mode

### `<to_command>`
Changes to Command mode.

### `<to_edi_normal>`
Switches to Editor Normal mode.

### `<to_edi_visual>`
Switches to Editor Visual mode.

### `<to_edi_visual_line>`
Switches to Editor Visual-Line mode.

### `<to_gui_visual>`
Switches to GUI Visual mode.

### `<to_insert>`
Enters the Insert mode. In this mode, you can directly input and edit text.

### `<to_instant_gui_normal>`
Temporarily switches to GUI Normal mode and performs matching, which can be used as a map-leader.

### `<to_resident>`
Enters Resident mode.

## Mouse

### `<click_left>`
Left button of a mouse click.

### `<click_right>`
Right button of a mouse click.

### `<click_mid>`
Middle button of a mouse click.

### `<easyclick>`
Move a cursor using hints on the UI objects without clicking.

### `<gridmove>`
Move a cursor using tiled hints laid on the entire screen.

### `<jump_cursor_to_active_window>`
Jump the mouse cursor to the foreground window.

### `<jump_cursor_to_bottom>`
Jump the mouse cursor to the bottom.

### `<jump_cursor_to_hcenter>`
Jump the mouse cursor to the horizontal center.

### `<jump_cursor_to_left>`
Jump the mouse cursor to the left.

### `<jump_cursor_to_right>`
Jump the mouse cursor to the right.

### `<jump_cursor_to_top>`
Jump the mouse cursor to the top.

### `<jump_cursor_to_vcenter>`
Jump the mouse cursor to the vertical center.

### `<jump_cursor_with_keybrd_layout>`
Jump the mouse cursor by keyboard mapping.

### `<move_cursor_down>`
Move the mouse cursor down.

### `<move_cursor_left>`
Move the mouse cursor to the left.

### `<move_cursor_right>`
Move the mouse cursor to the right.

### `<move_cursor_up>`
Move the mouse cursor up.

### `<scroll_down>`
Scroll the mouse wheel down.

### `<scroll_down_halfpage>`
Scroll the mouse wheel down with a half page.

### `<scroll_down_onepage>`
Scroll the mouse wheel down with a page.

### `<scroll_left>`
Scroll the mouse wheel left.

### `<scroll_left_halfpage>`
Scroll the mouse wheel left with a half page.

### `<scroll_right>`
Scroll the mouse wheel right.

### `<scroll_right_halfpage>`
Scroll the mouse wheel right with a half page.

### `<scroll_up>`
Scroll the mouse wheel up.

### `<scroll_up_halfpage>`
Scroll the mouse wheel up with a half page.

### `<scroll_up_onepage>`
Scroll the mouse wheel up with a page.

## Hotkey

### `<backward_ui_navigation>`
Backward UI Navigation.

### `<decide_focused_ui_object>`
Decide a focused UI object.

### `<forward_ui_navigation>`
Forward UI Navigation.

### `<goto_next_page>`
Forward to the next page.

### `<goto_prev_page>`
Go backward to the previous page.

### `<hotkey_backspace>`
Backspace.

### `<hotkey_copy>`
Copy.

### `<hotkey_cut>`
Cut.

### `<hotkey_delete>`
Delete.

### `<hotkey_paste>`
Paste.

### `<open>`
Open another file.

### `<open_startmenu>`
Open the Start Menu.

### `<redo>`
Redo.

### `<save>`
Save the current file.

### `<search_pattern>`
Search Pattern.

### `<select_all>`
Select all.

### `<start_explorer>`
Start Explorer.

### `<undo>`
Undo.

## Virtual Desktop

### `<close_current_vdesktop>`
Close a current virtual desktop.

### `<create_new_vdesktop>`
Create a new virtual desktop.

### `<switch_to_left_vdesktop>`
Switch to a left virtual desktop.

### `<switch_to_right_vdesktop>`
Switch to a right virtual desktop.

### `<taskview>`
Task View.

## Tab

### `<close_current_tab>`
Close a current tab.

### `<open_new_tab>`
Open a new tab.

### `<switch_to_left_tab>`
Switch to a left tab.

### `<switch_to_right_tab>`
Switch to a right tab.

## Window

### `<arrange_windows>`
Arrange windows with tile style.

### `<close_current_window>`
Close the current window.

### `<decrease_window_height>`
Decrease the height of a window.

### `<decrease_window_width>`
Decrease the width of a window.

### `<exchange_window_with_nearest>`
Exchange a window with the nearest window.

### `<increase_window_height>`
Increase the height of a window.

### `<increase_window_width>`
Increase the width of a window.

### `<maximize_current_window>`
Maximize the current window.

### `<minimize_current_window>`
Minimize the current window.

### `<open_new_window>`
Open a new window.

### `<open_new_window_with_hsplit>`
Open a new window with a horizontal split.

### `<open_new_window_with_vsplit>`
Open a new window with a vertical split.

### `<reload_current_window>`
Reload the current window.

### `<resize_window_height>`
Set the height of a window. You have to pass the pixel value as an argument using the command line.

### `<resize_window_width>`
Set the width of a window. You have to pass the pixel value as an argument using the command line.

### `<rotate_windows>`
Rotate windows in the current monitor.

### `<rotate_windows_in_reverse>`
Rotate windows in the current monitor in reverse.

### `<select_left_window>`
Select the left window.

### `<select_lower_window>`
Select the lower window.

### `<select_right_window>`
Select the right window.

### `<select_upper_window>`
Select the upper window.

### `<snap_current_window_to_bottom>`
Snap the current window to the bottom.

### `<snap_current_window_to_left>`
Snap the current window to the left.

### `<snap_current_window_to_right>`
Snap the current window to the right.

### `<snap_current_window_to_top>`
Snap the current window to the top.

### `<switch_window>`
Switch a window.

### `<window_resizer>`
Start window resizer. It respects Vim plugin [simeji/winresizer](https://github.com/simeji/winresizer).

## File
### `<makedir>`
Create a directory. If you call it with a relative path such as `:mkdir foo`, it creates it in the explorer directory. If no explorer is found, it creates it in `~/Desktop/foo`. If you call it with an absolute path like `:mkdir C:/Users/You/Desktop/bar`, it will create a directory along the path recursively.

## Process

### `<execute>`
Open file with the associated application. This is a wrapper for the famous Windows API, **ShellExecute**, which behaves the same as double-clicking in Explorer. Therefore, you can open any format files and URLs. For example, `:e ~/.vimrc` or `:e https://www.google.com`. If there is no argument, it will open .vindrc loaded at initialization.

### `<exit>`
Exit win-vind.

### `<start_external>`
Start an external application. This environment variable is dependent on the application specified in the `shell` option. By appending `;` at the end, it keeps the console window without closing immediately. If the explorer is the foreground window, the current directory of a terminal will be that directory.

### `<start_shell>`
Start a terminal. If the explorer is the foreground window, the current directory of a terminal will be that directory.

## Option

### `<disable_targeting_of_dedicate_to_window>`
Disable targeting (Dedicate to One Window).

### `<enable_targeting_of_dedicate_to_window>`
Enable targeting (Dedicate to One Window).

## System Command

### `<set>`
System Command set.

### `<source>`
System Command source.

### `<map>`
System Command map.

### `<noremap>`
System Command noremap.

### `<unmap>`
System Command unmap.

### `<mapclear>`
System Command mapclear.

### `<command>`
System Command command.

### `<delcommand>`
System Command delcommand.

### `<comclear>`
System Command comclear.

## Vim Emulation

### `<to_insert_BOL>`
**Vim Emulation:** `I`  
Insert to begin of line.

### `<to_insert_EOL>`
**Vim Emulation:** `A`  
Append end of line.

### `<to_insert_append>`
**Vim Emulation:** `a`  
Append after a caret.

### `<to_insert_nlabove>`
**Vim Emulation:** `O`  
Begin new line above a caret.

### `<to_insert_nlbelow>`
**Vim Emulation:** `o`  
Begin new line below a caret.

### `<move_caret_left>`
**Vim Emulation:** `h`  
Move the caret to left.

### `<move_caret_down>`
**Vim Emulation:** `j`  
Move the caret down.

### `<move_caret_up>`
**Vim Emulation:** `k`  
Move the caret up.

### `<move_caret_right>`
**Vim Emulation:** `l`  
Move the caret to right.

### `<move_fwd_word>`
**Vim Emulation:** `w`  
Move words forward for normal mode.

### `<move_fwd_word_simple>`
**Vim Emulation:** `w`  
Move words forward fast.

### `<move_bck_word>`
**Vim Emulation:** `b`  
Move words backward for normal mode.

### `<move_bck_word_simple>`
**Vim Emulation:** `b`  
Move words backward fast.

### `<move_fwd_bigword>`
**Vim Emulation:** `W`  
Move WORDS forward.

### `<move_bck_bigword>`
**Vim Emulation:** `B`  
Move WORDS backward.

### `<move_end_word>`
**Vim Emulation:** `e`  
Forward to the end of words.

### `<move_end_bigword>`
**Vim Emulation:** `E`  
Forward to the end of WORDS.

### `<move_bckend_word>`
**Vim Emulation:** `ge`  
Backward to the end of words.

### `<move_bckend_bigword>`
**Vim Emulation:** `gE`  
Backward to the end of WORDS.

### `<jump_caret_to_BOF>`
**Vim Emulation:** `gg`  
Jump the caret to BOF.

### `<jump_caret_to_BOL>`
**Vim Emulation:** `0`  
Jump the caret to begin of line.

### `<jump_caret_to_EOF>`
**Vim Emulation:** `G`  
Jump the caret to EOF.

### `<jump_caret_to_EOL>`
**Vim Emulation:** `$`  
Jump the caret to end of line.

### `<change_char>`
**Vim Emulation:** `s`  
Change Characters.

### `<change_highlight_text>`
**Vim Emulation:** `c`, `s`, `S`  
Change highlighted texts.

### `<change_line>`
**Vim Emulation:** `cc`, `S`  
Change Lines.

### `<change_until_EOL>`
**Vim Emulation:** `C`  
Change until EOL.

### `<change_with_motion>`
**Vim Emulation:** `c{motion}`  
Change texts with motion.

### `<delete_after>`
**Vim Emulation:** `x`  
Delete chars after the caret.

### `<delete_before>`
**Vim Emulation:** `X`  
Delete chars before the caret.

### `<delete_highlight_text>`
**Vim Emulation:** `d`, `x`, `X`  
Delete highlighted texts.

### `<delete_line>`
**Vim Emulation:** `dd`  
Delete lines.

### `<delete_line_until_EOL>`
**Vim Emulation:** `D`  
Delete texts until end of line.

### `<delete_with_motion>`
**Vim Emulation:** `d{motion}`  
Delete texts with motion.

### `<join_next_line>`
**Vim Emulation:** `J`  
Join a next line.

### `<put_after>`
**Vim Emulation:** `p`  
Put texts after the caret.

### `<put_before>`
**Vim Emulation:** `P`  
Put texts before the caret.

### `<repeat_last_change>`
**Vim Emulation:** `.`  
Repeat last simple change.

### `<replace_char>`
**Vim Emulation:** `r`  
Replace a char.

### `<replace_sequence>`
**Vim Emulation:** `R`  
Replace Mode.

### `<switch_char_case>`
**Vim Emulation:** `~`  
Switch char case.

### `<yank_highlight_text>`
**Vim Emulation:** `y`  
Yank highlighted texts.

### `<yank_line>`
**Vim Emulation:** `yy`, `Y`  
Yank lines.

### `<yank_with_motion>`
**Vim Emulation:** `y{motion}`  
Yank lines with motion.

<br>
<br>
<br>
<br>
