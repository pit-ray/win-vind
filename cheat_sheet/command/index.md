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

|Command|ID|Note|
|:---:|:---:|:---|
|`::guinormal`, `:gn`|**to_gui_normal**|GUI Normal Mode|
|`:resident`|**to_resident**|Resident Mode|
|`:guivisual`, `:gv`|**to_gui_visual**|GUI Visual Mode|
|`:edinormal`, `:en`|**to_edi_normal**|Editor Normal Mode|
|`:insert`, `:i`|**to_insert**|Insert Mode|
|`:edivisual`, `:ev`|**to_edi_visual**|Editor Visual Mode|
|`:edivisualline`, `:evl`|**to_edi_visual_line**|Editor Visual Line Mode|

### Virtual Desktop

|Command|ID|Note|
|:---:|:---:|:---|
|`:vdnext`|**switch_to_left_vdesktop**|Switch to a left virtual desktop|
|`:vdprev`|**switch_to_right_vdesktop**|Switch to a right virtual desktop|
|`:closev`|**close_current_vdesktop**|Close a current virtual desktop|
|`:taskview`|**taskview**|Task View|

### Tab

|Command|ID|Note|
|:---:|:---:|:---|
|`:tabprevious`|**switch_to_left_tab**|Switch to a left tab|
|`:tabnext`|**switch_to_right_tab**|Switch to a right tab|
|`:tabnew`|**open_new_tab**|Open a new tab|
|`:q`, `:q!`, `:tabclose`|**close_current_tab**|Close a current tab|

### Window

|Command|ID|Note|
|:---:|:---:|:---|
|`:cl`, `:close`|**close_current_window**|Close a current window|
|`:sw`, `:switch`|**switch_window**|Switch a window|
|`:resizer`, `:winresizer`|**window_resizer**|Start window resizer. It respects Vim plugin (<a href="https://github.com/simeji/winresizer">simeji/winresizer</a>.|
|`:max`, `:on`, `:only`|**maximize_current_window**|Maximize a current window|
|`:min`, `:hi`, `:hide`|**minimize_current_window**|Minimize a current window|
|`:lsplit`, `:lsp`, `:wl`|**snap_current_window_to_left**|Snap a current window to left|
|`:rsplit`, `:rsp`, `:wh`|**snap_current_window_to_right**|Snap a current window to right|
|`:tsplit`, `:tsp`, `:wt`|**snap_current_window_to_top**|Snap a current window to top|
|`:bsplit`, `:bsp`, `:wb`|**snap_current_window_to_bottom**|Snap a current window to bottom|
|`:arrange`|**arrange_windows**|Arrange windows with tile style|
|`:rotate`, `:rot`|**rotate_windows**|Rotate windows in a current monitor|
|`:rerotate`, `:rerot`|**rotate_windows_in_reverse**|Rotate windows in a current monitor in reverse|
|`:exchange`, `:exw`|**exchange_window_with_nearest**|Exchange a window with next one|
|`:vertical resize <num>`, `:vert res <num>`|**resize_window_width**|Resize the width of a window|
|`:vertical resize +<num>`, `:vert res +<num>`|**increase_window_width**|Increase the width of a window|
|`:vertical resize -<num>`, `:vert res -<num>`|**decrease_window_width**|Decrease the width of a window|
|`:resize <num>`, `:res <num>`|**resize_window_height**|Resize the height of a window|
|`:resize +<num>`, `:res +<num>`|**increase_window_height**|Increase the height of a window|
|`:resize -<num>`, `:res -<num>`|**decrease_window_height**|Decrease the height of a window|
|`:new`|**open_new_window**|Open a new window|
|`:sp`, `:split`|**open_new_window_with_hsplit**|Open a new window with horizontal split|
|`:vs`, `:vsplit`|**open_new_window_with_vsplit**|Open a new window with vertical split|
|`:reload`|**reload_current_window**|Reload a current window|

### File

|Command|ID|Note|
|:---:|:---:|:---|
|`:mkdir<any>`, `:md<any>`|**makedir**|Make a directory|

### Vim Emulation

|Command|ID|Note|
|:---:|:---:|:---|
|`:-<num>`|**move_caret_up**|Move the caret on the number of lines specified by `<num>`.|
|`:+<num>`|**move_caret_down**|Move the caret down on the number of lines specified by `<num`>.|
|`:<num>`|**jump_caret_to_BOF**|Jump the caret to the specified line by `<num>`.|

### Process

|Command|ID|Note|
|:---:|:---:|:---|
|`:sh`, `:shell`, `:terminal`, `:term`|**start_shell**|Start a terminal|
|`:!<any>`|**start_external**|Start an external application. This environment variable is dependent on the application specified in the shell option. By appending `;` at the end, it keeps the console window without closing immediately.|
|`:e<any>`, `:edit<any>`, `:execute<any>`|**execute**|Open file with the associated application. It is wrapper of **ShellExecute**.|
|`:exit`|**exit_config_gui**|Exit win-vind|

### Hotkey

|Command|ID|Note|
|:---:|:---:|:---|
|`:win`, `:start`|**open_startmenu**|Open a start menu|
|`:ex`, `:explorer`|**start_explorer**|Start a Explorer|
|`:find`, `:open`|**open**|Open another file|
|`:w`|**save**|Save a current file|
|`:fun`|**forward_ui_navigation**|Forward UI Navigation|
|`:bun`|**backward_ui_navigation**|Backward UI Navigation|
|`:decide`|**decide_focused_ui_object**|Decide a focused UI object|

### Option

|Command|ID|Note|
|:---:|:---:|:---|
|`:target`|**enable_targeting_of_dedicate_to_window**|Enable targeting (Dedicate to One Window)|
|`:untarget`|**disable_targeting_of_dedicate_to_window**|Disable targeting (Dedicate to One Window)|

If **Dedicate to One window** enables, you can select one window with Enable Targeting function. In this case, it makes the mode automatically switch to Editor Normal Mode on the targeting window. When the foreground window change to another, it makes the mode switch to Insert Mode. The targeting becomes disable with Disable Targeting function. In other words, this feature transforms some normal editors compared with Vim to fake Vim. The computing cost is so small.

### System Command

|Command|ID|Note|
|:---:|:---:|:---|
|`:set<any>`|**system_command_set**|System Command set|
|`:source<any>`|**system_command_source**|System Command source|
|`:{mode-prefix}map<any>`|**system_command_map**|System Command map|
|`:{mode-prefix}mapclear<any>`|**system_command_mapclear**|System Command mapclear|
|`:{mode-prefix}noremap<any>`|**system_command_noremap**|System Command noremap|
|`:{mode-prefix}unmap<any>`|**system_command_unmap**|System Command unmap|
|`:comclear<any>`|**system_command_comclear**|System Command comclear|
|`:command<any>`|**system_command_command**|System Command command|
|`:delcommand<any>`|**system_command_delcommand**|System Command delcommand|

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
<br>
<br>
