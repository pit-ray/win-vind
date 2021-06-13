---
layout: default
title: Command Mode
nav_order: 5
descripption: "Command lists of win-vind."
has_children: true
---


## Default Commands

|Command after `:`|ID|Feature|
|:---:|:---:|:---|
|`guinormal`, `gn`|**to_gui_normal**|GUI Normal Mode|
|`resident`|**to_resident**|Resident Mode|
|`guivisual`, `gv`|**to_gui_visual**|GUI Visual Mode|
|`edinormal`, `en`|**to_edi_normal**|Normal Mode (Vim Emulation: <Esc>)|
|`insert`, `i`|**to_insert**|Insert Mode|
|`edivisual`, `ev`|**to_edi_visual**|Visual Mode (Vim Emulation: v)|
|`edivisualline`, `evl`|**to_edi_visual_line**|Visual-Line Mode (Vim Emulation: V)|
|`vdnext`|**switch_to_left_vdesktop**|Switch to a left virtual desktop|
|`vdprev`|**switch_to_right_vdesktop**|Switch to a right virtual desktop|
|`closev`|**close_current_vdesktop**|Close a current virtual desktop|
|`taskview`, `tv`, `vdesktop list`|**taskview**|Task View|
|`tabprevious`|**switch_to_left_tab**|Switch to a left tab|
|`tabnext`|**switch_to_right_tab**|Switch to a right tab|
|`tabnew`|**open_new_tab**|Open a new tab|
|`cl`, `close`|**close_current_window**|Close a current window|
|`sw`, `switch`|**switch_window**|Switch a window|
|`wr`, `resizer`, `winresizer`|**window_resizer**|Window Resizer|
|`max`, `on`, `only`|**maximize_current_window**|Maximize a current window|
|`min`, `hi`, `hide`|**minimize_current_window**|Minimize a current window|
|`lsplit`, `lsp`, `wl`|**snap_current_window_to_left**|Snap a current window to left|
|`rsplit`, `rsp`, `wh`|**snap_current_window_to_right**|Snap a current window to right|
|`tsplit`, `tsp`, `wt`|**snap_current_window_to_top**|Snap a current window to top|
|`bsplit`, `bsp`, `wb`|**snap_current_window_to_bottom**|Snap a current window to bottom|
|`arrange`, `arw`|**arrange_windows**|Arrange windows|
|`rotate`, `rot`|**rotate_windows**|Rotate windows in a current monitor|
|`rerotate`, `rerot`|**rotate_windows_in_reverse**|Rotate windows in a current monitor in reverse|
|`exchange`, `exw`|**exchange_window_with_nearest**|Exchange a window with next one|
|`vertical resize <num>`, `vert res <num>`|**resize_window_width**|Resize the width of a window|
|`vertical resize +<num>`, `vert res +<num>`|**increase_window_width**|Increase the width of a window|
|`vertical resize -<num>`, `vert res -<num>`|**decrease_window_width**|Decrease the width of a window|
|`resize <num>`, `res <num>`|**resize_window_height**|Resize the height of a window|
|`resize +<num>`, `res +<num>`|**increase_window_height**|Increase the height of a window|
|`resize -<num>`, `res -<num>`|**decrease_window_height**|Decrease the height of a window|
|`new`|**open_new_window**|Open a new window|
|`sp`, `split`|**open_new_window_with_hsplit**|Open a new window with horizontal split|
|`vs`, `vsplit`|**open_new_window_with_vsplit**|Open a new window with vertical split|
|`w`|**save**|Save a current file|
|`q`, `q!`, `tabclose`|**close_current_tab**|Close a current tab|
|`sh`, `shell`, `terminal`, `term`|**start_shell**|Start a terminal|
|`ex`, `explorer`|**start_explorer**|Start a Explorer|
|`win`, `start`|**open_startmenu**|Open a start menu|
|`!<any>`|**start_external**|Start a optional application|
|`e<any>`, `edit<any>`, `execute<any>`|**execute**|Open file with default program|
|`setgui`|**show_config_gui**|Show a config window of win-vind|
|`exit`|**exit_config_gui**|Exit win-vind|
|`find`, `open`|**open**|Open a other file|
|`mkdir`, `md`, `mkdir <any>`, `md <any>`|**makedir**|Make a directory|
|`reload`|**reload_current_window**|Reload a current window|
|`target`|**enable_targeting_of_dedicate_to_window**|Enable targeting (Dedicate to One Window)|
|`untarget`|**disable_targeting_of_dedicate_to_window**|Disable targeting (Dedicate to One Window)|
|`-<num>`|**move_caret_up**|Move the caret up (Vim Emulation: k)|
|`+<num>`|**move_caret_down**|Move the caret down (Vim Emulation: j)|
|`<num>`|**jump_caret_to_BOF**|Jump the caret to BOF (Vim Emulation: gg)|
|`ecl`|**easy_click_left**|EasyClick (Left Click)|
|`ecr`|**easy_click_right**|EasyClick (Right Click)|
|`ecm`|**easy_click_mid**|EasyClick (Mid Click)|
|`ech`|**easy_click_hover**|EasyClick (Hovering)|
|`fun`|**forward_ui_navigation**|Forward UI Navigation|
|`bun`|**backward_ui_navigation**|Backward UI Navigation|
|`decide`|**decide_focused_ui_object**|Decide a focused UI object|
|`set<any>`|**system_command_set**|System Command set|
|`{mode-prefix}map<any>`|**system_command_map**|System Command map|
|`{mode-prefix}noremap<any>`|**system_command_noremap**|System Command noremap|
|`{mode-prefix}unmap<any>`|**system_command_unmap**|System Command unmap|
|`{mode-prefix}mapclear<any>`|**system_command_mapclear**|System Command mapclear|
|`command<any>`|**system_command_command**|System Command command|
|`delcommand<any>`|**system_command_delcommand**|System Command delcommand|
|`comclear<any>`|**system_command_comclear**|System Command comclear|
|`source <any>`|**system_command_source**|System Command source|
