---
layout: page
show_in_menu: false
disable_anchors: true
title: Default Mappings - win-vind
nav: Default Mappings
---

<p align="center">
<img src="{{ site.url }}/imgs/mode_overview.png" width=600>  
<p align="center">Default mode layer overview</p>
</p>

## Insert Mode  

|Bindings|ID|
|:---:|:---:|
|`<Esc-Down>`|**to_resident**|
|`<Esc-Left>`|**to_gui_normal**|
|`<Esc-Right>`|**to_edi_normal**|
|`<F8>`|**to_instant_gui_normal**|

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
|`:`|**to_command**|
|`<Esc-Down>`|**to_resident**|
|`I`, `<Esc-Right>`|**to_edi_normal**|
|`i`|**to_insert**|
|`v`|**to_gui_visual**|


### Mouse

|Bindings|ID|
|:---:|:---:|
|`$`, `<End>`|**jump_cursor_to_right**|
|`0`, `^`, `<Home>`|**jump_cursor_to_left**|
|`<C-b>`|**scroll_up_onepage**|
|`<C-d>`|**scroll_down_halfpage**|
|`<C-e>`, `<C-j>`|**scroll_down**|
|`<C-f>`|**scroll_down_onepage**|
|`<C-u>`|**scroll_up_halfpage**|
|`<C-y>`, `<C-k>`|**scroll_up**|
|`FF`, `Fo`|**easy_click_left**|
|`Fa`|**easy_click_right**|
|`Fh`|**easy_click_hover**|
|`Fm`|**easy_click_mid**|
|`G`|**jump_cursor_to_bottom**|
|`M`|**jump_cursor_to_vcenter**|
|`a`, `A`|**click_right**|
|`f`|**jump_cursor_with_keybrd_layout**|
|`gg`|**jump_cursor_to_top**|
|`gm`|**jump_cursor_to_hcenter**|
|`h`, `<BS>`, `<Left>`|**move_cursor_left**|
|`j`, `+`, `<Down>`|**move_cursor_down**|
|`k`, `-`, `<Up>`|**move_cursor_up**|
|`l`, `<Space>`, `<Right>`|**move_cursor_right**|
|`o`, `O`|**click_left**|
|`t`|**jump_cursor_to_active_window**|
|`zH`|**scroll_left_halfpage**|
|`zL`|**scroll_right_halfpage**|
|`zh`, `<C-h>`|**scroll_left**|
|`zl`, `<C-l>`|**scroll_right**|

### Hotkey

|Bindings|ID|
|:---:|:---:|
|`/`, `?`|**search_pattern**|
|`<C-r>`|**redo**|
|`<gt>`|**goto_next_page**|
|`<lt>`|**goto_prev_page**|
|`<win>`|**open_startmenu**|
|`V`|**select_all**|
|`X`|**hotkey_backspace**|
|`dd`, `D`|**hotkey_cut**|
|`p`, `P`|**hotkey_paste**|
|`u`, `U`|**undo**|
|`x`, `<Del>`|**hotkey_delete**|
|`y`, `yy`, `Y`|**hotkey_copy**|

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
|`<C-w>+`|**increase_window_height**|
|`<C-w>-`|**decrease_window_height**|
|`<C-w><gt>`|**increase_window_width**|
|`<C-w><lt>`|**decrease_window_width**|
|`<C-w>=`|**arrange_windows**|
|`<C-w>H`, `<C-w><Left>`|**snap_current_window_to_left**|
|`<C-w>J`|**snap_current_window_to_bottom**|
|`<C-w>K`|**snap_current_window_to_top**|
|`<C-w>L`, `<C-w><Right>`|**snap_current_window_to_right**|
|`<C-w>R`|**rotate_windows_in_reverse**|
|`<C-w>d`|**minimize_current_window**|
|`<C-w>e`|**window_resizer**|
|`<C-w>h`|**select_left_window**|
|`<C-w>j`|**select_lower_window**|
|`<C-w>k`|**select_upper_window**|
|`<C-w>l`|**select_right_window**|
|`<C-w>n`|**open_new_window**|
|`<C-w>q`, `<C-w>c`|**close_current_window**|
|`<C-w>r`|**rotate_windows**|
|`<C-w>s`|**switch_window**|
|`<C-w>u`|**maximize_current_window**|
|`<C-w>x`|**exchange_window_with_nearest**|


## GUI Visual Mode
### Mode

|Bindings|ID|
|:---:|:---:|
|`<Esc-Down>`|**to_resident**|
|`<Esc-Left>`|**to_gui_normal**|

### Mouse

|Bindings|ID|
|:---:|:---:|
|`$`, `<End>`|**jump_cursor_to_right**|
|`0`, `^`, `<Home>`|**jump_cursor_to_left**|
|`<C-b>`|**scroll_up_onepage**|
|`<C-d>`|**scroll_down_halfpage**|
|`<C-e>`, `<C-j>`|**scroll_down**|
|`<C-f>`|**scroll_down_onepage**|
|`<C-u>`|**scroll_up_halfpage**|
|`<C-y>`, `<C-k>`|**scroll_up**|
|`G`|**jump_cursor_to_bottom**|
|`M`|**jump_cursor_to_vcenter**|
|`gg`|**jump_cursor_to_top**|
|`gm`|**jump_cursor_to_hcenter**|
|`h`, `<BS>`, `<Left>`|**move_cursor_left**|
|`j`, `+`, `<Down>`|**move_cursor_down**|
|`k`, `-`, `<Up>`|**move_cursor_up**|
|`l`, `<Space>`, `<Right>`|**move_cursor_right**|
|`zH`|**scroll_left_halfpage**|
|`zL`|**scroll_right_halfpage**|
|`zh`, `<C-h>`|**scroll_left**|
|`zl`, `<C-l>`|**scroll_right**|

### Hotkey

|Bindings|ID|
|:---:|:---:|
|`X`|**hotkey_backspace**|
|`dd`, `D`|**hotkey_cut**|
|`p`, `P`|**hotkey_paste**|
|`x`, `<Del>`|**hotkey_delete**|
|`y`, `yy`, `Y`|**hotkey_copy**|


## Editor Normal Mode

### Mode

|Bindings|ID|
|:---:|:---:|
|`:`|**to_command**|
|`<Esc-Down>`|**to_resident**|
|`<Esc-Left>`|**to_gui_normal**|
|`V`|**to_edi_visual_line**|
|`i`|**to_insert**|
|`v`|**to_edi_visual**|

### Mouse

|Bindings|ID|
|:---:|:---:|
|`<C-b>`|**scroll_up_onepage**|
|`<C-d>`|**scroll_down_halfpage**|
|`<C-e>`, `<C-j>`|**scroll_down**|
|`<C-f>`|**scroll_down_onepage**|
|`<C-u>`|**scroll_up_halfpage**|
|`<C-y>`, `<C-k>`|**scroll_up**|
|`zH`|**scroll_left_halfpage**|
|`zL`|**scroll_right_halfpage**|
|`zh`, `<C-h>`|**scroll_left**|
|`zl`, `<C-l>`|**scroll_right**|

### Hotkey

|Bindings|ID|
|:---:|:---:|
|`/`, `?`|**search_pattern**|
|`<C-r>`|**redo**|
|`u`, `U`|**undo**|

### Tab

|Bindings|ID|
|:---:|:---:|
|`gT`|**switch_to_left_tab**|
|`gt`|**switch_to_right_tab**|


### Vim Emulation

**Mode**  

|Bindings|ID|
|:---:|:---:|
|`A`|**to_insert_EOL**|
|`I`, `gI`|**to_insert_BOL**|
|`O`|**to_insert_nlabove**|
|`a`|**to_insert_append**|
|`o`|**to_insert_nlbelow**|

<br>

**Move**  

|Bindings|ID|
|:---:|:---:|
|`B`|**move_caret_nonblank_word_backward**|
|`W`|**move_caret_nonblank_word_forward**|
|`b`|**move_caret_word_backward**|
|`h`, `<C-h>`, `<BS>`, `<Left>`|**move_caret_left**|
|`j`, `+`, `gj`, `<C-n>`, `<Down>`, `<Enter>`, `<C-m>`|**move_caret_down**|
|`k`, `-`, `gk`, `<C-p>`, `<Up>`|**move_caret_up**|
|`l`, `<Space>`, `<Right>`|**move_caret_right**|
|`w`|**move_caret_word_forward**|

<br>

**Jump**  

|Bindings|ID|
|:---:|:---:|
|`$`, `<End>`, `g$`|**jump_caret_to_EOL**|
|`0`, `<Home>`, `g0`|**jump_caret_to_BOL**|
|`G`|**jump_caret_to_EOF**|
|`gg`|**jump_caret_to_BOF**|


<br>

**Edit**  

|Bindings|ID|
|:---:|:---:|
|`C`|**change_until_EOL**|
|`D`|**delete_line_until_EOL**|
|`J`|**join_next_line**|
|`P`|**put_before**|
|`R`|**replace_sequence**|
|`X`|**delete_before**|
|`c`|**change_with_motion**|
|`cc`, `S`|**change_line**|
|`d`|**delete_with_motion**|
|`dd`|**delete_line**|
|`p`|**put_after**|
|`r`|**replace_char**|
|`s`|**change_char**|
|`x`, `<Del>`|**delete_after**|
|`y`|**yank_with_motion**|
|`yy`, `Y`|**yank_line**|
|`~`|**switch_char_case**|


## Editor Visual Mode

### Mode

|Bindings|ID|
|:---:|:---:|
|`:`|**to_command**|
|`<Esc-Down>`|**to_resident**|
|`<Esc-Left>`|**to_gui_normal**|
|`<Esc-Right>`|**to_edi_normal**|

### Mouse

|Bindings|ID|
|:---:|:---:|
|`<C-b>`|**scroll_up_onepage**|
|`<C-d>`|**scroll_down_halfpage**|
|`<C-e>`, `<C-j>`|**scroll_down**|
|`<C-f>`|**scroll_down_onepage**|
|`<C-u>`|**scroll_up_halfpage**|
|`<C-y>`, `<C-k>`|**scroll_up**|
|`zH`|**scroll_left_halfpage**|
|`zL`|**scroll_right_halfpage**|
|`zh`, `<C-h>`|**scroll_left**|
|`zl`, `<C-l>`|**scroll_right**|

### Vim Emulation

**Move**  

|Bindings|ID|
|:---:|:---:|
|`B`|**move_caret_nonblank_word_backward**|
|`W`|**move_caret_nonblank_word_forward**|
|`b`|**move_caret_word_backward**|
|`h`, `<C-h>`, `<BS>`, `<Left>`|**move_caret_left**|
|`j`, `+`, `gj`, `<C-n>`, `<Down>`, `<Enter>`, `<C-m>`|**move_caret_down**|
|`k`, `-`, `gk`, `<C-p>`, `<Up>`|**move_caret_up**|
|`l`, `<Space>`, `<Right>`|**move_caret_right**|
|`w`|**move_caret_word_forward**|

<br>

**Jump**  

|Bindings|ID|
|:---:|:---:|
|`$`, `<End>`, `g$`|**jump_caret_to_EOL**|
|`0`, `<Home>`, `g0`|**jump_caret_to_BOL**|
|`G`|**jump_caret_to_EOF**|
|`gg`|**jump_caret_to_BOF**|

<br>

**Edit**  

|Bindings|ID|
|:---:|:---:|
|`c`, `s`, `S`|**change_highlight_text**|
|`d`, `x`, `X`|**delete_highlight_text**|
|`y`|**yank_highlight_text**|


## Command Mode

### Mode

|Command|ID|
|:---:|:---:|
|`::guinormal`, `:gn`|**to_gui_normal**|
|`:edinormal`, `:en`|**to_edi_normal**|
|`:edivisual`, `:ev`|**to_edi_visual**|
|`:edivisualline`, `:evl`|**to_edi_visual_line**|
|`:guivisual`, `:gv`|**to_gui_visual**|
|`:insert`, `:i`|**to_insert**|
|`:resident`|**to_resident**|

### Virtual Desktop

|Command|ID|
|:---:|:---:|
|`:closev`|**close_current_vdesktop**|
|`:taskview`|**taskview**|
|`:vdnext`|**switch_to_right_vdesktop**|
|`:vdprev`|**switch_to_left_vdesktop**|

### Tab

|Command|ID|
|:---:|:---:|
|`:q`, `:q!`, `:tabclose`|**close_current_tab**|
|`:tabnew`|**open_new_tab**|
|`:tabnext`|**switch_to_right_tab**|
|`:tabprevious`|**switch_to_left_tab**|

### Window

|Command|ID|
|:---:|:---:|
|`:arrange`|**arrange_windows**|
|`:bsplit`, `:bsp`|**snap_current_window_to_bottom**|
|`:cl`, `:close`|**close_current_window**|
|`:exchange`|**exchange_window_with_nearest**|
|`:lsplit`, `:lsp`|**snap_current_window_to_left**|
|`:max`, `:on`, `:only`|**maximize_current_window**|
|`:min`, `:hi`, `:hide`|**minimize_current_window**|
|`:new`|**open_new_window**|
|`:reload`|**reload_current_window**|
|`:rerotate`, `:rerot`|**rotate_windows_in_reverse**|
|`:resize +<num>`, `:res +<num>`|**increase_window_height**|
|`:resize -<num>`, `:res -<num>`|**decrease_window_height**|
|`:resize <num>`, `:res <num>`|**resize_window_height**|
|`:resizer`, `:winresizer`|**window_resizer**|
|`:rotate`, `:rot`|**rotate_windows**|
|`:rsplit`, `:rsp`|**snap_current_window_to_right**|
|`:sp`, `:split`|**open_new_window_with_hsplit**|
|`:sw`, `:switch`|**switch_window**|
|`:tsplit`, `:tsp`|**snap_current_window_to_top**|
|`:vertical resize +<num>`, `:vert res +<num>`|**increase_window_width**|
|`:vertical resize -<num>`, `:vert res -<num>`|**decrease_window_width**|
|`:vertical resize <num>`, `:vert res <num>`|**resize_window_width**|
|`:vs`, `:vsplit`|**open_new_window_with_vsplit**|

### File

|Command|ID|
|:---:|:---:|
|`:mkdir<any>`, `:md<any>`|**makedir**|

### Process

|Command|ID|
|:---:|:---:|
|`:!<any>`|**start_external**|
|`:e <any>`, `:edit <any>`, `:execute <any>`|**execute**|
|`:exit`|**exit_config_gui**|
|`:sh`, `:shell`, `:terminal`, `:term`|**start_shell**|

### Hotkey

|Command|ID|
|:---:|:---:|
|`:backward`|**backward_ui_navigation**|
|`:decide`|**decide_focused_ui_object**|
|`:ex`, `:explorer`|**start_explorer**|
|`:find`, `:open`|**open**|
|`:forward`|**forward_ui_navigation**|
|`:w`|**save**|
|`:win`, `:start`|**open_startmenu**|

### Option

|Command|ID|
|:---:|:---:|
|`:target`|**enable_targeting_of_dedicate_to_window**|
|`:untarget`|**disable_targeting_of_dedicate_to_window**|

### System Command

|Command|ID|
|:---:|:---:|
|`:comclear`|**system_command_comclear**|
|`:command<any>`|**system_command_command**|
|`:delcommand<any>`|**system_command_delcommand**|
|`:set<any>`|**system_command_set**|
|`:source<any>`|**system_command_source**|
|`:{mode-prefix}map<any>`|**system_command_map**|
|`:{mode-prefix}mapclear`|**system_command_mapclear**|
|`:{mode-prefix}noremap<any>`|**system_command_noremap**|
|`:{mode-prefix}unmap<any>`|**system_command_unmap**|

### Vim Emulation

|Command|ID|
|:---:|:---:|
|`:+<num>`|**move_caret_down**|
|`:-<num>`|**move_caret_up**|
|`:<num>`|**jump_caret_to_BOF**|


<br>
<br>
<br>
<br>
