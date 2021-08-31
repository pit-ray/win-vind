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

<hr>

## Customize

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
<br>
<br>
