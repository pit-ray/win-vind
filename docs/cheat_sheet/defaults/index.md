---
layout: page
show_in_menu: false
disable_anchors: true
title: Defaults - win-vind
nav: Defaults
description: Default bindings lists
---

<p align="center">
<img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/mode_overview.png?raw=true" width=600>  
<p align="center">Default mode layer overview</p>
</p>

## Insert Mode  

|Bindings|ID|
|:---:|:---:|
|`<Esc-Left>`|**to_gui_normal**|
|`<Esc-Right>`|**to_edi_normal**|
|`<F8>`|**to_instant_gui_normal**|
|`<Esc-Down>`|**to_resident**|

## Resident Mode

|Bindings|ID|
|:---:|:---:|
|`<Esc-Left>`|**to_gui_normal**|
|`<Esc-Right>`|**to_edi_normal**|
|`<Esc-Up>`|**to_insert**|


## GUI Normal Mode

### Mode

|Bindings|ID|
|:---:|:---:|
|`v`|**to_gui_visual**|
|`I`, `<Esc-Right>`|**to_edi_normal**|
|`i`|**to_insert**|
|`<Esc-Down>`|**to_resident**|
|`:`|**to_command**|


### Mouse

|Bindings|ID|
|:---:|:---:|
|`h`, `<BS>`, `<Left>`|**move_cursor_left**|
|`l`, `<Space>`, `<Right>`|**move_cursor_right**|
|`k`, `-`, `<Up>`|**move_cursor_up**|
|`j`, `+`, `<Down>`|**move_cursor_down**|
|`0`, `^`, `<Home>`|**jump_cursor_to_left**|
|`$`, `<End>`|**jump_cursor_to_right**|
|`gg`|**jump_cursor_to_top**|
|`G`|**jump_cursor_to_bottom**|
|`gm`|**jump_cursor_to_hcenter**|
|`M`|**jump_cursor_to_vcenter**|
|`f`|**jump_cursor_with_keybrd_layout**|
|`t`|**jump_cursor_to_active_window**|
|`<C-y>`, `<C-k>`|**scroll_up**|
|`<C-e>`, `<C-j>`|**scroll_down**|
|`<C-u>`|**scroll_up_halfpage**|
|`<C-d>`|**scroll_down_halfpage**|
|`<C-b>`|**scroll_up_onepage**|
|`<C-f>`|**scroll_down_onepage**|
|`zh`, `<C-h>`|**scroll_left**|
|`zl`, `<C-l>`|**scroll_right**|
|`zH`|**scroll_left_halfpage**|
|`zL`|**scroll_right_halfpage**|
|`o`, `O`|**click_left**|
|`a`, `A`|**click_right**|
|`FF`, `Fo`|**easy_click_left**|
|`Fa`|**easy_click_right**|
|`Fm`|**easy_click_mid**|
|`Fh`|**easy_click_hover**|

### Hotkey

|Bindings|ID|
|:---:|:---:|
|`V`|**select_all**|
|`y`, `yy`, `Y`|**hotkey_copy**|
|`p`, `P`|**hotkey_paste**|
|`dd`, `D`|**hotkey_cut**|
|`x`, `<Del>`|**hotkey_delete**|
|`X`|**hotkey_backspace**|
|`<C-r>`|**redo**|
|`u`, `U`|**undo**|
|`/`, `?`|**search_pattern**|
|`<gt>`|**goto_next_page**|
|`<lt>`|**goto_prev_page**|
|`<win>`|**open_startmenu**|

### Virtual Desktop

|Bindings|ID|
|:---:|:---:|
|`<C-v>h`|**switch_to_left_vdesktop**|
|`<C-v>l`|**switch_to_right_vdesktop**|
|`<C-v>n`|**create_new_vdesktop**|
|`<C-v>q`|**close_current_vdesktop**|
|`<C-v>s`|**taskview**|

### Tab

|Bindings|ID|
|:---:|:---:|
|`gT`|**switch_to_left_tab**|
|`gt`|**switch_to_right_tab**|


### Window

|Bindings|ID|
|:---:|:---:|
|`<C-w>q`, `<C-w>c`|**close_current_window**|
|`<C-w>s`|**switch_window**|
|`<C-w>e`|**window_resizer**|
|`<C-w>h`|**select_left_window**|
|`<C-w>l`|**select_right_window**|
|`<C-w>k`|**select_upper_window**|
|`<C-w>j`|**select_lower_window**|
|`<C-w>u`|**maximize_current_window**|
|`<C-w>d`|**minimize_current_window**|
|`<C-w>H`, `<C-w><Left>`|**snap_current_window_to_left**|
|`<C-w>L`, `<C-w><Right>`|**snap_current_window_to_right**|
|`<C-w>K`|**snap_current_window_to_top**|
|`<C-w>J`|**snap_current_window_to_bottom**|
|`<C-w>=`|**arrange_windows**|
|`<C-w>r`|**rotate_windows**|
|`<C-w>R`|**rotate_windows_in_reverse**|
|`<C-w>x`|**exchange_window_with_nearest**|
|`<C-w><gt>`|**increase_window_width**|
|`<C-w><lt>`|**decrease_window_width**|
|`<C-w>+`|**increase_window_height**|
|`<C-w>-`|**decrease_window_height**|
|`<C-w>n`|**open_new_window**|


## GUI Visual Mode
### Mode

|Bindings|ID|
|:---:|:---:|
|`<Esc-Left>`|**to_gui_normal**|
|`<Esc-Down>`|**to_resident**|

### Mouse

|Bindings|ID|
|:---:|:---:|
|`h`, `<BS>`, `<Left>`|**move_cursor_left**|
|`l`, `<Space>`, `<Right>`|**move_cursor_right**|
|`k`, `-`, `<Up>`|**move_cursor_up**|
|`j`, `+`, `<Down>`|**move_cursor_down**|
|`0`, `^`, `<Home>`|**jump_cursor_to_left**|
|`$`, `<End>`|**jump_cursor_to_right**|
|`gg`|**jump_cursor_to_top**|
|`G`|**jump_cursor_to_bottom**|
|`gm`|**jump_cursor_to_hcenter**|
|`M`|**jump_cursor_to_vcenter**|
|`<C-y>`, `<C-k>`|**scroll_up**|
|`<C-e>`, `<C-j>`|**scroll_down**|
|`<C-u>`|**scroll_up_halfpage**|
|`<C-d>`|**scroll_down_halfpage**|
|`<C-b>`|**scroll_up_onepage**|
|`<C-f>`|**scroll_down_onepage**|
|`zh`, `<C-h>`|**scroll_left**|
|`zl`, `<C-l>`|**scroll_right**|
|`zH`|**scroll_left_halfpage**|
|`zL`|**scroll_right_halfpage**|

### Hotkey

|Bindings|ID|
|:---:|:---:|
|`y`, `yy`, `Y`|**hotkey_copy**|
|`p`, `P`|**hotkey_paste**|
|`dd`, `D`|**hotkey_cut**|
|`x`, `<Del>`|**hotkey_delete**|
|`X`|**hotkey_backspace**|


## Editor Normal Mode

### Mode

|Bindings|ID|
|:---:|:---:|
|`<Esc-Left>`|**to_gui_normal**|
|`<Esc-Down>`|**to_resident**|
|`i`|**to_insert**|
|`v`|**to_edi_visual**|
|`V`|**to_edi_visual_line**|
|`:`|**to_command**|

### Mouse

|Bindings|ID|
|:---:|:---:|
|`<C-y>`, `<C-k>`|**scroll_up**|
|`<C-e>`, `<C-j>`|**scroll_down**|
|`<C-u>`|**scroll_up_halfpage**|
|`<C-d>`|**scroll_down_halfpage**|
|`<C-b>`|**scroll_up_onepage**|
|`<C-f>`|**scroll_down_onepage**|
|`zh`, `<C-h>`|**scroll_left**|
|`zl`, `<C-l>`|**scroll_right**|
|`zH`|**scroll_left_halfpage**|
|`zL`|**scroll_right_halfpage**|

### HotKey

|Bindings|ID|
|:---:|:---:|
|`<C-r>`|**redo**|
|`u`, `U`|**undo**|
|`/`, `?`|**search_pattern**|

### Tab

|Bindings|ID|
|:---:|:---:|
|`gT`|**switch_to_left_tab**|
|`gt`|**switch_to_right_tab**|


### Vim Emulation

**Mode**  

|Bindings|ID|
|:---:|:---:|
|`I`, `gI`|**to_insert_BOL**|
|`a`|**to_insert_append**|
|`A`|**to_insert_EOL**|
|`o`|**to_insert_nlbelow**|
|`O`|**to_insert_nlabove**|

<br>

**Move**  

|Bindings|ID|
|:---:|:---:|
|`h`, `<C-h>`, `<BS>`, `<Left>`|**move_caret_left**|
|`l`, `<Space>`, `<Right>`|**move_caret_right**|
|`k`, `-`, `gk`, `<C-p>`, `<Up>`|**move_caret_up**|
|`j`, `+`, `gj`, `<C-n>`, `<Down>`, `<Enter>`, `<C-m>`|**move_caret_down**|
|`w`|**move_caret_word_forward**|
|`b`|**move_caret_word_backward**|
|`W`|**move_caret_nonblank_word_forward**|
|`B`|**move_caret_nonblank_word_backward**|

<br>

**Jump**  

|Bindings|ID|
|:---:|:---:|
|`0`, `<Home>`, `g0`|**jump_caret_to_BOL**|
|`$`, `<End>`, `g$`|**jump_caret_to_EOL**|
|`gg`|**jump_caret_to_BOF**|
|`G`|**jump_caret_to_EOF**|


<br>

**Edit**  

|Bindings|ID|
|:---:|:---:|
|`yy`, `Y`|**yank_line**|
|`y`|**yank_with_motion**|
|`p`|**put_after**|
|`P`|**put_before**|
|`dd`|**delete_line**|
|`D`|**delete_line_until_EOL**|
|`x`, `<Del>`|**delete_after**|
|`X`|**delete_before**|
|`J`|**join_next_line**|
|`r`|**replace_char**|
|`R`|**replace_sequence**|
|`~`|**switch_char_case**|
|`d`|**delete_with_motion**|
|`c`|**change_with_motion**|
|`cc`, `S`|**change_line**|
|`s`|**change_char**|
|`C`|**change_until_EOL**|


## Editor Visual Mode

### Mode

|Bindings|ID|
|:---:|:---:|
|`<Esc-Left>`|**to_gui_normal**|
|`<Esc-Down>`|**to_resident**|
|`:`|**to_command**|
|`<Esc-Right>`|**to_edi_normal**|

### Mouse

|Bindings|ID|
|:---:|:---:|
|`<C-y>`, `<C-k>`|**scroll_up**|
|`<C-e>`, `<C-j>`|**scroll_down**|
|`<C-u>`|**scroll_up_halfpage**|
|`<C-d>`|**scroll_down_halfpage**|
|`<C-b>`|**scroll_up_onepage**|
|`<C-f>`|**scroll_down_onepage**|
|`zh`, `<C-h>`|**scroll_left**|
|`zl`, `<C-l>`|**scroll_right**|
|`zH`|**scroll_left_halfpage**|
|`zL`|**scroll_right_halfpage**|

### Vim Emulation

**Move**  

|Bindings|ID|
|:---:|:---:|
|`h`, `<C-h>`, `<BS>`, `<Left>`|**move_caret_left**|
|`l`, `<Space>`, `<Right>`|**move_caret_right**|
|`k`, `-`, `gk`, `<C-p>`, `<Up>`|**move_caret_up**|
|`j`, `+`, `gj`, `<C-n>`, `<Down>`, `<Enter>`, `<C-m>`|**move_caret_down**|
|`w`|**move_caret_word_forward**|
|`b`|**move_caret_word_backward**|
|`W`|**move_caret_nonblank_word_forward**|
|`B`|**move_caret_nonblank_word_backward**|

<br>

**Jump**  

|Bindings|ID|
|:---:|:---:|
|`0`, `<Home>`, `g0`|**jump_caret_to_BOL**|
|`$`, `<End>`, `g$`|**jump_caret_to_EOL**|
|`gg`|**jump_caret_to_BOF**|
|`G`|**jump_caret_to_EOF**|

<br>

**Edit**  

|Bindings|ID|
|:---:|:---:|
|`y`|**yank_highlight_text**|
|`d`, `x`, `X`|**delete_highlight_text**|
|`c`, `s`, `S`|**change_highlight_text**|


## Command Mode

### Mode

|Command|ID|
|:---:|:---:|
|`::guinormal`, `:gn`|**to_gui_normal**|
|`:resident`|**to_resident**|
|`:guivisual`, `:gv`|**to_gui_visual**|
|`:edinormal`, `:en`|**to_edi_normal**|
|`:insert`, `:i`|**to_insert**|
|`:edivisual`, `:ev`|**to_edi_visual**|
|`:edivisualline`, `:evl`|**to_edi_visual_line**|

### Virtual Desktop

|Command|ID|
|:---:|:---:|
|`:vdprev`|**switch_to_left_vdesktop**|
|`:vdnext`|**switch_to_right_vdesktop**|
|`:closev`|**close_current_vdesktop**|
|`:taskview`|**taskview**|

### Tab

|Command|ID|
|:---:|:---:|
|`:tabprevious`|**switch_to_left_tab**|
|`:tabnext`|**switch_to_right_tab**|
|`:tabnew`|**open_new_tab**|
|`:q`, `:q!`, `:tabclose`|**close_current_tab**|

### Window

|Command|ID|
|:---:|:---:|
|`:cl`, `:close`|**close_current_window**|
|`:sw`, `:switch`|**switch_window**|
|`:resizer`, `:winresizer`|**window_resizer**|
|`:max`, `:on`, `:only`|**maximize_current_window**|
|`:min`, `:hi`, `:hide`|**minimize_current_window**|
|`:lsplit`, `:lsp`|**snap_current_window_to_left**|
|`:rsplit`, `:rsp`|**snap_current_window_to_right**|
|`:tsplit`, `:tsp`|**snap_current_window_to_top**|
|`:bsplit`, `:bsp`|**snap_current_window_to_bottom**|
|`:arrange`|**arrange_windows**|
|`:rotate`, `:rot`|**rotate_windows**|
|`:rerotate`, `:rerot`|**rotate_windows_in_reverse**|
|`:exchange`|**exchange_window_with_nearest**|
|`:vertical resize <num>`, `:vert res <num>`|**resize_window_width**|
|`:vertical resize +<num>`, `:vert res +<num>`|**increase_window_width**|
|`:vertical resize -<num>`, `:vert res -<num>`|**decrease_window_width**|
|`:resize <num>`, `:res <num>`|**resize_window_height**|
|`:resize +<num>`, `:res +<num>`|**increase_window_height**|
|`:resize -<num>`, `:res -<num>`|**decrease_window_height**|
|`:new`|**open_new_window**|
|`:sp`, `:split`|**open_new_window_with_hsplit**|
|`:vs`, `:vsplit`|**open_new_window_with_vsplit**|
|`:reload`|**reload_current_window**|

### File

|Command|ID|
|:---:|:---:|
|`:mkdir<any>`, `:md<any>`|**makedir**|

### Vim Emulation

|Command|ID|
|:---:|:---:|
|`:-<num>`|**move_caret_up**|
|`:+<num>`|**move_caret_down**|
|`:<num>`|**jump_caret_to_BOF**|

### Process

|Command|ID|
|:---:|:---:|
|`:sh`, `:shell`, `:terminal`, `:term`|**start_shell**|
|`:!<any>`|**start_external**|
|`:e <any>`, `:edit <any>`, `:execute <any>`|**execute**|
|`:exit`|**exit_config_gui**|

### Hotkey

|Command|ID|
|:---:|:---:|
|`:win`, `:start`|**open_startmenu**|
|`:ex`, `:explorer`|**start_explorer**|
|`:find`, `:open`|**open**|
|`:w`|**save**|
|`:forward`|**forward_ui_navigation**|
|`:backward`|**backward_ui_navigation**|
|`:decide`|**decide_focused_ui_object**|

### Option

|Command|ID|
|:---:|:---:|
|`:target`|**enable_targeting_of_dedicate_to_window**|
|`:untarget`|**disable_targeting_of_dedicate_to_window**|

### System Command

|Command|ID|
|:---:|:---:|
|`:set<any>`|**system_command_set**|
|`:source<any>`|**system_command_source**|
|`:{mode-prefix}map<any>`|**system_command_map**|
|`:{mode-prefix}mapclear`|**system_command_mapclear**|
|`:{mode-prefix}noremap<any>`|**system_command_noremap**|
|`:{mode-prefix}unmap<any>`|**system_command_unmap**|
|`:comclear`|**system_command_comclear**|
|`:command<any>`|**system_command_command**|
|`:delcommand<any>`|**system_command_delcommand**|


<br>
<br>
<br>
<br>
