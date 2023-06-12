---
layout: page
show_in_menu: false
disable_anchors: true
title: Small Mappings - win-vind
nav: Small Mappings
---

## GUI Normal Mode

### Window Open/Close

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<c-w>n`|[\<open_new_window\>]({{ site.url }}/cheat_sheet/functions/#open_new_window)|
|`<c-w>q`, `<c-w>c`|[\<close_current_window\>]({{ site.url }}/cheat_sheet/functions/#close_current_window)|

### Window Select

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<c-w>h`|[\<select_left_window\>]({{ site.url }}/cheat_sheet/functions/#select_left_window)|
|`<c-w>l`|[\<select_right_window\>]({{ site.url }}/cheat_sheet/functions/#select_right_window)|
|`<c-w>k`|[\<select_upper_window\>]({{ site.url }}/cheat_sheet/functions/#select_upper_window)|
|`<c-w>j`|[\<select_lower_window\>]({{ site.url }}/cheat_sheet/functions/#select_lower_window)|
|`<c-w>s`|[\<switch_window\>]({{ site.url }}/cheat_sheet/functions/#switch_window)|

### Window Resize

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<c-w>=`|[\<arrange_windows\>]({{ site.url }}/cheat_sheet/functions/#arrange_windows)|
|`<c-w>r`|[\<rotate_windows\>]({{ site.url }}/cheat_sheet/functions/#rotate_windows)|
|`<c-w>r`|[\<rotate_windows_in_reverse\>]({{ site.url }}/cheat_sheet/functions/#rotate_windows_in_reverse)|
|`<c-w>x`|[\<exchange_window_with_nearest\>]({{ site.url }}/cheat_sheet/functions/#exchange_window_with_nearest)|
|`<c-w><gt>`|[\<increase_window_width\>]({{ site.url }}/cheat_sheet/functions/#increase_window_width)|
|`<c-w><lt>`|[\<decrease_window_width\>]({{ site.url }}/cheat_sheet/functions/#decrease_window_width)|
|`<c-w>+`|[\<increase_window_height\>]({{ site.url }}/cheat_sheet/functions/#increase_window_height)|
|`<c-w>-`|[\<decrease_window_height\>]({{ site.url }}/cheat_sheet/functions/#decrease_window_height)|
|`<c-w>u`|[\<maximize_current_window\>]({{ site.url }}/cheat_sheet/functions/#maximize_current_window)|
|`<c-w>d`|[\<minimize_current_window\>]({{ site.url }}/cheat_sheet/functions/#minimize_current_window)|
|`<c-w>h`, `<c-w><left>`|[\<snap_current_window_to_left\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_left)|
|`<c-w><right>`, `<c-w>l`|[\<snap_current_window_to_right\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_right)|
|`<c-w>k`|[\<snap_current_window_to_top\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_top)|
|`<c-w>j`|[\<snap_current_window_to_bottom\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_bottom)|
|`<c-w>e`|[\<window_resizer\>]({{ site.url }}/cheat_sheet/functions/#window_resizer)|

## Command Mode

### Window Open/Close

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`cl`, `close`|[\<close_current_window\>]({{ site.url }}/cheat_sheet/functions/#close_current_window)|
|`new`|[\<open_new_window\>]({{ site.url }}/cheat_sheet/functions/#open_new_window)|
|`split`, `sp`|[\<open_new_window_with_hsplit\>]({{ site.url }}/cheat_sheet/functions/#open_new_window_with_hsplit)|
|`vsplit`, `vs`|[\<open_new_window_with_vsplit\>]({{ site.url }}/cheat_sheet/functions/#open_new_window_with_vsplit)|

### Window Select

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`switch`, `sw`|[\<switch_window\>]({{ site.url }}/cheat_sheet/functions/#switch_window)|

### Window Resize

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`winresizer`, `resizer`|[\<window_resizer\>]({{ site.url }}/cheat_sheet/functions/#window_resizer)|
|`only`, `max`, `on`|[\<maximize_current_window\>]({{ site.url }}/cheat_sheet/functions/#maximize_current_window)|
|`min`, `hide`, `hi`|[\<minimize_current_window\>]({{ site.url }}/cheat_sheet/functions/#minimize_current_window)|
|`lsplit`, `lsp`|[\<snap_current_window_to_left\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_left)|
|`rsp`, `rsplit`|[\<snap_current_window_to_right\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_right)|
|`tsp`, `tsplit`|[\<snap_current_window_to_top\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_top)|
|`bsplit`, `bsp`|[\<snap_current_window_to_bottom\>]({{ site.url }}/cheat_sheet/functions/#snap_current_window_to_bottom)|
|`arrange`|[\<arrange_windows\>]({{ site.url }}/cheat_sheet/functions/#arrange_windows)|
|`reload`|[\<reload_current_window\>]({{ site.url }}/cheat_sheet/functions/#reload_current_window)|
|`rot`, `rotate`|[\<rotate_windows\>]({{ site.url }}/cheat_sheet/functions/#rotate_windows)|
|`rerot`, `rerotate`|[\<rotate_windows_in_reverse\>]({{ site.url }}/cheat_sheet/functions/#rotate_windows_in_reverse)|
|`exchange`|[\<exchange_window_with_nearest\>]({{ site.url }}/cheat_sheet/functions/#exchange_window_with_nearest)|
|`vert<space>res`, `vertical<space>resize`|[\<resize_window_width\>]({{ site.url }}/cheat_sheet/functions/#resize_window_width)|
|`vert<space>res<space>+`, `vertical<space>resize<space>+`|[\<increase_window_width\>]({{ site.url }}/cheat_sheet/functions/#increase_window_width)|
|`vertical<space>resize<space>-`, `vert<space>res<space>-`|[\<decrease_window_width\>]({{ site.url }}/cheat_sheet/functions/#decrease_window_width)|
|`resize`, `res`|[\<resize_window_height\>]({{ site.url }}/cheat_sheet/functions/#resize_window_height)|
|`res<space>+`, `resize<space>+`|[\<increase_window_height\>]({{ site.url }}/cheat_sheet/functions/#increase_window_height)|
|`res<space>-`, `resize<space>-`|[\<decrease_window_height\>]({{ site.url }}/cheat_sheet/functions/#decrease_window_height)|

### Process Launcher

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`!`|[\<start_external\>]({{ site.url }}/cheat_sheet/functions/#start_external)|
|`execute`, `e`, `edit`|[\<execute\>]({{ site.url }}/cheat_sheet/functions/#execute)|
|`sh`, `term`, `terminal`, `shell`|[\<start_shell\>]({{ site.url }}/cheat_sheet/functions/#start_shell)|

<br>
<br>
## See also
- [tiny](../tiny)
- [small](../small)
- [normal](../normal)
- [big](../big)
- [huge](../huge)
<br>
