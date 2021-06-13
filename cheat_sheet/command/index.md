---
layout: default
title: Command Mode
nav_order: 7
descripption: "Command Mode Cheat Sheet of win-vind."
parent: Cheat Sheet
---

# Command Mode
{: .no_toc}  

<hr>

1. TOC1  
{:toc}

## Default

### Mode

|Command|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`::guinormal`, `:gn`|**to_gui_normal**|GUI Normal Mode||
|`:resident`|**to_resident**|Resident Mode||
|`:guivisual`, `:gv`|**to_gui_visual**|GUI Visual Mode||
|`:edinormal`, `:en`|**to_edi_normal**|Editor Normal Mode||
|`:insert`, `:i`|**to_insert**|Insert Mode||
|`:edivisual`, `:ev`|**to_edi_visual**|Editor Visual Mode||
|`:edivisualline`, `:evl`|**to_edi_visual_line**|Editor Visual Line Mode||

### Virtual Desktop

|Command|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`:vdnext`|**switch_to_left_vdesktop**|Switch to a left virtual desktop||
|`:vdprev`|**switch_to_right_vdesktop**|Switch to a right virtual desktop||
|`:closev`|**close_current_vdesktop**|Close a current virtual desktop||
|`:taskview`|**taskview**|Task View||

### Tab

|Command|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`:tabprevious`|**switch_to_left_tab**|Switch to a left tab||
|`:tabnext`|**switch_to_right_tab**|Switch to a right tab||
|`:tabnew`|**open_new_tab**|Open a new tab||
|`:q`, `:q!`, `:tabclose`|**close_current_tab**|Close a current tab||

### Window

|Command|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`:cl`, `:close`|**close_current_window**|Close a current window||
|`:sw`, `:switch`|**switch_window**|Switch a window||
|`:wr`, `:resizer`, `:winresizer`|**window_resizer**|Window Resizer||
|`:max`, `:on`, `:only`|**maximize_current_window**|Maximize a current window||
|`:min`, `:hi`, `:hide`|**minimize_current_window**|Minimize a current window||
|`:lsplit`, `:lsp`, `:wl`|**snap_current_window_to_left**|Snap a current window to left||
|`:rsplit`, `:rsp`, `:wh`|**snap_current_window_to_right**|Snap a current window to right||
|`:tsplit`, `:tsp`, `:wt`|**snap_current_window_to_top**|Snap a current window to top||
|`:bsplit`, `:bsp`, `:wb`|**snap_current_window_to_bottom**|Snap a current window to bottom||
|`:arrange`, `:arw`|**arrange_windows**|Arrange windows||
|`:rotate`, `:rot`|**rotate_windows**|Rotate windows in a current monitor||
|`:rerotate`, `:rerot`|**rotate_windows_in_reverse**|Rotate windows in a current monitor in reverse||
|`:exchange`, `:exw`|**exchange_window_with_nearest**|Exchange a window with next one||
|`:vertical resize <num>`, `:vert res <num>`|**resize_window_width**|Resize the width of a window||
|`:vertical resize +<num>`, `:vert res +<num>`|**increase_window_width**|Increase the width of a window||
|`:vertical resize -<num>`, `:vert res -<num>`|**decrease_window_width**|Decrease the width of a window||
|`:resize <num>`, `:res <num>`|**resize_window_height**|Resize the height of a window||
|`:resize +<num>`, `:res +<num>`|**increase_window_height**|Increase the height of a window||
|`:resize -<num>`, `:res -<num>`|**decrease_window_height**|Decrease the height of a window||
|`:new`|**open_new_window**|Open a new window||
|`:sp`, `:split`|**open_new_window_with_hsplit**|Open a new window with horizontal split||
|`:vs`, `:vsplit`|**open_new_window_with_vsplit**|Open a new window with vertical split||
|`:reload`|**reload_current_window**|Reload a current window||

### Mouse

|Command|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`:ecl`|**easy_click_left**|EasyClick (Left Click)||
|`:ecr`|**easy_click_right**|EasyClick (Right Click)||
|`:ecm`|**easy_click_mid**|EasyClick (Mid Click)||
|`:ech`|**easy_click_hover**|EasyClick (Hovering)||

### File

|Command|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`:mkdir<any>`, `:md<any>`|**makedir**|Make a directory||

### Vim Emulation

|Command|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`:-<num>`|**move_caret_up**|Move the caret up (Vim Emulation: k)||
|`:+<num>`|**move_caret_down**|Move the caret down (Vim Emulation: j)||
|`:<num>`|**jump_caret_to_BOF**|Jump the caret to BOF (Vim Emulation: gg)||

### Process

|Command|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`:sh`, `:shell`, `:terminal`, `:term`|**start_shell**|Start a terminal||
|`:!<any>`|**start_external**|Start a optional application||
|`:e<any>`, `:edit<any>`, `:execute<any>`|**execute**|Open file with default program||
|`:exit`|**exit_config_gui**|Exit win-vind||

### Hotkey

|Command|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`:bun`|**backward_ui_navigation**|Backward UI Navigation||
|`:decide`|**decide_focused_ui_object**|Decide a focused UI object||
|`:ex`, `:explorer`|**start_explorer**|Start a Explorer||
|`:find`, `:open`|**open**|Open a other file||
|`:fun`|**forward_ui_navigation**|Forward UI Navigation||
|`:w`|**save**|Save a current file||
|`:win`, `:start`|**open_startmenu**|Open a start menu||

### Option

|Command|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`:target`|**enable_targeting_of_dedicate_to_window**|Enable targeting (Dedicate to One Window)||
|`:untarget`|**disable_targeting_of_dedicate_to_window**|Disable targeting (Dedicate to One Window)||

### System Command

|Command|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`:set<any>`|**system_command_set**|System Command set||
|`:source<any>`|**system_command_source**|System Command source||
|`:{mode-prefix}map<any>`|**system_command_map**|System Command map||
|`:{mode-prefix}mapclear<any>`|**system_command_mapclear**|System Command mapclear||
|`:{mode-prefix}noremap<any>`|**system_command_noremap**|System Command noremap||
|`:{mode-prefix}unmap<any>`|**system_command_unmap**|System Command unmap||
|`:comclear<any>`|**system_command_comclear**|System Command comclear||
|`:command<any>`|**system_command_command**|System Command command||
|`:delcommand<any>`|**system_command_delcommand**|System Command delcommand||

<hr>

## Customization

Write `.vindrc` in the following way. The avaliable commands are `command`, `delcommand`, and `comclear`.

**Sample**
```vim
" ~/.win-vind/.vindrc

command foo easy_click_left " Call easy_click_left with :foo
delcommand close            " Delete :close command
comclear                    " Remove all commands
```

<br>
<br>
