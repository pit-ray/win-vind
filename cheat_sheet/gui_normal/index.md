---
layout: default
title: GUI Normal Mode
nav_order: 5
descripption: "Command lists of win-vind."
parent: Cheat Sheet
---

# GUI Normal Mode
{: .no_toc}  

<hr>

1. TOC1  
{:toc}


## Default

### Mode

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`<Esc-Left>`|**to_gui_normal**|GUI Normal Mode||
|`<c-i>`|**to_resident**|Resident Mode||
|`v`|**to_gui_visual**|GUI Visual Mode||
|`:`|**to_command**|Command Mode||
|`I`, `<Esc-Right>`|**to_edi_normal**|Normal Mode (Vim Emulation: <Esc>)||
|`i`|**to_insert**|Insert Mode||


### Mouse

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`h`, `<BS>`, `<Left>`|**move_cursor_left**|Move the mouse cursor to left||
|`l`, `<Space>`, `<Right>`|**move_cursor_right**|Move the mouse cursor to right||
|`k`, `-`, `<Up>`|**move_cursor_up**|Move the mouse cursor up||
|`j`, `+`, `<Down>`|**move_cursor_down**|Move the Mouse Cursor Down||
|`0`, `^`, `<Home>`|**jump_to_left**|Jump the Mouse cursor to left||
|`$`, `<End>`|**jump_to_right**|Jump the mouse cursor to right||
|`gg`|**jump_to_top**|Jump the mouse cursor to top||
|`G`|**jump_to_bottom**|Jump the mouse cursor to bottom||
|`gm`|**jump_to_hcenter**|Jump the mouse cursor to horizontal center||
|`M`|**jump_to_vcenter**|Jump the mouse cursor to vertical center||
|`f`|**jump_with_keybrd_layout**|Jump the mouse cursor by keyboard mapping||
|`t`|**jump_to_active_window**|Jump the mouse cursor to selected window||
|`<C-y>`, `<C-k>`|**scroll_up**|Scroll a mouse wheel up||
|`<C-e>`, `<C-j>`|**scroll_down**|Scroll a mouse wheel down||
|`<C-u>`|**scroll_up_halfpage**|Scroll a mouse wheel up with a half page||
|`<C-d>`|**scroll_down_halfpage**|Scroll a mouse wheel down with a half page||
|`<C-b>`|**scroll_up_onepage**|Scroll a mouse wheel up with a page||
|`<C-f>`|**scroll_down_onepage**|Scroll a mouse wheel down With a page||
|`zh`, `<C-h>`|**scroll_left**|Scroll a mouse wheel left||
|`zl`, `<C-l>`|**scroll_right**|Scroll a mouse wheel right||
|`zH`|**scroll_left_halfpage**|Scroll a mouse wheel left With a half page||
|`zL`|**scroll_right_halfpage**|Scroll a mouse wheel right with a half page||
|`o`, `O`|**click_left**|Left button of a mouse click||
|`a`, `A`|**click_right**|Right button of a mouse click||
|`FF`, `Fo`|**easy_click_left**|EasyClick (Left Click)||
|`Fa`|**easy_click_right**|EasyClick (Right Click)||
|`Fm`|**easy_click_mid**|EasyClick (Mid Click)||
|`Fh`|**easy_click_hover**|EasyClick (Hovering)||

### Hotkey

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`V`|**select_all**|Select all||
|`y`, `yy`, `Y`|**hotkey_copy**|Standard Copy||
|`p`, `P`|**hotkey_paste**|Standard Paste||
|`dd`, `D`|**hotkey_cut**|Standard Cut||
|`x`, `<Del>`|**hotkey_delete**|Standard Delete||
|`X`|**hotkey_backspace**|Standard BackSpace||
|`<C-r>`|**redo**|Redo||
|`u`, `U`|**undo**|Undo||
|`/`, `?`|**search_pattern**|Search Pattern||
|`<gt>`|**goto_next_page**|Forward a next page||
|`<lt>`|**goto_prev_page**|Backward a previous page||
|`<win>`|**open_startmenu**|Open a start menu||

### Virtual Desktop

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`<C-v>h`|**switch_to_left_vdesktop**|Switch to a left virtual desktop||
|`<C-v>l`|**switch_to_right_vdesktop**|Switch to a right virtual desktop||
|`<C-v>n`, `<C-v>c`|**create_new_vdesktop**|Create a new virtual desktop||
|`<C-v>q`|**close_current_vdesktop**|Close a current virtual desktop||
|`<C-v>s`|**taskview**|Task View||

### Tab

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`gT`|**switch_to_left_tab**|Switch to a left tab||
|`gt`|**switch_to_right_tab**|Switch to a right tab||


### Window

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`<C-w>q`, `<C-w>c`|**close_current_window**|Close a current window||
|`<C-w>s`|**switch_window**|Switch a window||
|`<C-w>e`|**window_resizer**|Window Resizer||
|`<C-w>h`|**select_left_window**|Select a left window||
|`<C-w>l`|**select_right_window**|Select a right window||
|`<C-w>k`|**select_upper_window**|Select an upper window||
|`<C-w>j`|**select_lower_window**|Select a lower window||
|`<C-w>u`|**maximize_current_window**|Maximize a current window||
|`<C-w>d`|**minimize_current_window**|Minimize a current window||
|`<C-w>H`, `<C-w><Left>`|**snap_current_window_to_left**|Snap a current window to left||
|`<C-w>L`, `<C-w><Right>`|**snap_current_window_to_right**|Snap a current window to right||
|`<C-w>K`|**snap_current_window_to_top**|Snap a current window to top||
|`<C-w>J`|**snap_current_window_to_bottom**|Snap a current window to bottom||
|`<C-w>=`|**arrange_windows**|Arrange windows||
|`<C-w>r`|**rotate_windows**|Rotate windows in a current monitor||
|`<C-w>R`|**rotate_windows_in_reverse**|Rotate windows in a current monitor in reverse||
|`<C-w>x`|**exchange_window_with_nearest**|Exchange a window with next one||
|`<C-w><gt>`|**increase_window_width**|Increase the width of a window||
|`<C-w><lt>`|**decrease_window_width**|Decrease the width of a window||
|`<C-w>+`|**increase_window_height**|Increase the height of a window||
|`<C-w>-`|**decrease_window_height**|Decrease the height of a window||
|`<C-w>n`|**open_new_window**|Open a new window||


### Option

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`<LAlt><LAlt>`, `<F2>`|**enable_targeting_of_dedicate_to_window**|Enable targeting (Dedicate to One Window)||
|`<RAlt><RAlt>`, `<F10>`|**disable_targeting_of_dedicate_to_window**|Disable targeting (Dedicate to One Window)||
