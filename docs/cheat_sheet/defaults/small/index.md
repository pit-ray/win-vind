---
layout: page
show_in_menu: false
disable_anchors: true
title: Small Mappings - win-vind
nav: Small Mappings
---

## GUI Normal Mode

### Window Open/Close

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`<C-w>n`|[\<open_new_window\>]({{ site.url }}/cheat_sheet/functions/#open_new_window)|
|`<C-w>q`, `<C-w>c`|[\<close_current_window\>]({{ site.url }}/cheat_sheet/functions/#close_current_window)|

### Window Select

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`<C-w>h`|[\<select_left_window\>]({{ site.url }}/cheat_sheet/functions/#select_left_window)|
|`<C-w>l`|[\<select_right_window\>]({{ site.url }}/cheat_sheet/functions/#select_right_window)|
|`<C-w>k`|[\<select_upper_window\>]({{ site.url }}/cheat_sheet/functions/#select_upper_window)|
|`<C-w>j`|[\<select_lower_window\>]({{ site.url }}/cheat_sheet/functions/#select_lower_window)|
|`<C-w>s`|[\<switch_window\>]({{ site.url }}/cheat_sheet/functions/#switch_window)|

### Window Resize

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`<C-w>=`|[\<arrange_windows\>]({{ site.url }}/cheat_sheet/functions/#arrange_windows)|
|`<C-w>r`|[\<rotate_windows\>]({{ site.url }}/cheat_sheet/functions/#rotate_windows)|
|`<C-w>R`|[\<rotate_windows_in_reverse\>]({{ site.url }}/cheat_sheet/functions/#rotate_windows_in_reverse)|
|`<C-w>x`|[\<exchange_window_with_nearest\>]({{ site.url }}/cheat_sheet/functions/#exchange_window_with_nearest)|
|`<C-w><gt>`|[\<increase_window_width\>]({{ site.url }}/cheat_sheet/functions/#increase_window_width)|
|`<C-w><lt>`|[\<decrease_window_width\>]({{ site.url }}/cheat_sheet/functions/#decrease_window_width)|
|`<C-w>+`|[\<increase_window_height\>]({{ site.url }}/cheat_sheet/functions/#increase_window_height)|
|`<C-w>-`|[\<decrease_window_height\>]({{ site.url }}/cheat_sheet/functions/#decrease_window_height)|
|`<C-w>u`|[\<maximize_current_window\>]({{ site.url }}/cheat_sheet/functions/#maximize_current_window)|
|`<C-w>d`|[\<minimize_current_window\>]({{ site.url }}/cheat_sheet/functions/#minimize_current_window)|
|`<C-w><Left>`, `<C-w>H`|[\<snap_current_window_to_left\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_left)|
|`<C-w><Right>`, `<C-w>L`|[\<snap_current_window_to_right\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_right)|
|`<C-w>K`|[\<snap_current_window_to_top\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_top)|
|`<C-w>J`|[\<snap_current_window_to_bottom\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_bottom)|
|`<C-w>e`|[\<window_resizer\>]({{ site.url }}/cheat_sheet/functions/#window_resizer)|

## Command Mode

### Window Open/Close

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`close`, `cl`|[\<close_current_window\>]({{ site.url }}/cheat_sheet/functions/#close_current_window)|
|`new`|[\<open_new_window\>]({{ site.url }}/cheat_sheet/functions/#open_new_window)|
|`split`, `sp`|[\<open_new_window_with_hsplit\>]({{ site.url }}/cheat_sheet/functions/#open_new_window_with_hsplit)|
|`vs`, `vsplit`|[\<open_new_window_with_vsplit\>]({{ site.url }}/cheat_sheet/functions/#open_new_window_with_vsplit)|

### Window Select

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`switch`, `sw`|[\<switch_window\>]({{ site.url }}/cheat_sheet/functions/#switch_window)|

### Window Resize

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`winresizer`, `resizer`|[\<window_resizer\>]({{ site.url }}/cheat_sheet/functions/#window_resizer)|
|`on`, `max`, `only`|[\<maximize_current_window\>]({{ site.url }}/cheat_sheet/functions/#maximize_current_window)|
|`min`, `hide`, `hi`|[\<minimize_current_window\>]({{ site.url }}/cheat_sheet/functions/#minimize_current_window)|
|`lsp`, `lsplit`|[\<snap_current_window_to_left\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_left)|
|`rsplit`, `rsp`|[\<snap_current_window_to_right\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_right)|
|`tsp`, `tsplit`|[\<snap_current_window_to_top\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_top)|
|`bsp`, `bsplit`|[\<snap_current_window_to_bottom\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_bottom)|
|`arrange`|[\<arrange_windows\>]({{ site.url }}/cheat_sheet/functions/#arrange_windows)|
|`reload`|[\<reload_current_window\>]({{ site.url }}/cheat_sheet/functions/#reload_current_window)|
|`rotate`, `rot`|[\<rotate_windows\>]({{ site.url }}/cheat_sheet/functions/#rotate_windows)|
|`rerotate`, `rerot`|[\<rotate_windows_in_reverse\>]({{ site.url }}/cheat_sheet/functions/#rotate_windows_in_reverse)|
|`exchange`|[\<exchange_window_with_nearest\>]({{ site.url }}/cheat_sheet/functions/#exchange_window_with_nearest)|
|`vertical<space>resize`, `vert<space>res`|[\<resize_window_width\>]({{ site.url }}/cheat_sheet/functions/#resize_window_width)|
|`vertical<space>resize<space>+`, `vert<space>res<space>+`|[\<increase_window_width\>]({{ site.url }}/cheat_sheet/functions/#increase_window_width)|
|`vert<space>res<space>-`, `vertical<space>resize<space>-`|[\<decrease_window_width\>]({{ site.url }}/cheat_sheet/functions/#decrease_window_width)|
|`resize`, `res`|[\<resize_window_height\>]({{ site.url }}/cheat_sheet/functions/#resize_window_height)|
|`res<space>+`, `resize<space>+`|[\<increase_window_height\>]({{ site.url }}/cheat_sheet/functions/#increase_window_height)|
|`res<space>-`, `resize<space>-`|[\<decrease_window_height\>]({{ site.url }}/cheat_sheet/functions/#decrease_window_height)|

### Process Launcher

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`!`|[\<start_external\>]({{ site.url }}/cheat_sheet/functions/#start_external)|
|`edit`, `execute`, `e`|[\<execute\>]({{ site.url }}/cheat_sheet/functions/#execute)|
|`term`, `terminal`, `shell`, `sh`|[\<start_shell\>]({{ site.url }}/cheat_sheet/functions/#start_shell)|

<br>
<br>
<br>
<br>
