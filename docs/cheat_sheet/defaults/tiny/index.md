---
layout: page
show_in_menu: false
disable_anchors: true
title: Tiny Mappings - win-vind
nav: Tiny Mappings
---

## GUI Normal Mode

### Mode Transition

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`<esc-left>`|[\<to_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_gui_normal)|
|`<esc-down>`|[\<to_resident\>]({{ site.url }}/cheat_sheet/functions/#to_resident)|
|`:`|[\<to_command\>]({{ site.url }}/cheat_sheet/functions/#to_command)|
|`i`|[\<to_insert\>]({{ site.url }}/cheat_sheet/functions/#to_insert)|

### Mouse Movement

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`h`, `<bs>`, `<left>`|[\<move_cursor_left\>]({{ site.url }}/cheat_sheet/functions/#move_cursor_left)|
|`<space>`, `<right>`, `l`|[\<move_cursor_right\>]({{ site.url }}/cheat_sheet/functions/#move_cursor_right)|
|`<up>`, `k`, `-`|[\<move_cursor_up\>]({{ site.url }}/cheat_sheet/functions/#move_cursor_up)|
|`+`, `j`, `<down>`|[\<move_cursor_down\>]({{ site.url }}/cheat_sheet/functions/#move_cursor_down)|

### Mouse Clicking

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`o`, `O`|[\<click_left\>]({{ site.url }}/cheat_sheet/functions/#click_left)|
|`A`, `a`|[\<click_right\>]({{ site.url }}/cheat_sheet/functions/#click_right)|

### Mouse Scrolling

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`<c-y>`, `<c-k>`|[\<scroll_up\>]({{ site.url }}/cheat_sheet/functions/#scroll_up)|
|`<c-j>`, `<c-e>`|[\<scroll_down\>]({{ site.url }}/cheat_sheet/functions/#scroll_down)|
|`<c-u>`|[\<scroll_up_halfpage\>]({{ site.url }}/cheat_sheet/functions/#scroll_up_halfpage)|
|`<c-d>`|[\<scroll_down_halfpage\>]({{ site.url }}/cheat_sheet/functions/#scroll_down_halfpage)|
|`<c-b>`|[\<scroll_up_onepage\>]({{ site.url }}/cheat_sheet/functions/#scroll_up_onepage)|
|`<c-f>`|[\<scroll_down_onepage\>]({{ site.url }}/cheat_sheet/functions/#scroll_down_onepage)|
|`<c-h>`, `zh`|[\<scroll_left\>]({{ site.url }}/cheat_sheet/functions/#scroll_left)|
|`zl`, `<c-l>`|[\<scroll_right\>]({{ site.url }}/cheat_sheet/functions/#scroll_right)|
|`zh`|[\<scroll_left_halfpage\>]({{ site.url }}/cheat_sheet/functions/#scroll_left_halfpage)|
|`zl`|[\<scroll_right_halfpage\>]({{ site.url }}/cheat_sheet/functions/#scroll_right_halfpage)|

### Mouse Jumping

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`<home>`, `^`, `0`|[\<jump_cursor_to_left\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_left)|
|`<end>`, `$`|[\<jump_cursor_to_right\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_right)|
|`gg`|[\<jump_cursor_to_top\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_top)|
|`G`|[\<jump_cursor_to_bottom\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_bottom)|
|`gm`|[\<jump_cursor_to_hcenter\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_hcenter)|
|`M`|[\<jump_cursor_to_vcenter\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_vcenter)|
|`t`|[\<jump_cursor_to_active_window\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_active_window)|

### Complex Mouse Controls

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`f`|[\<jump_cursor_with_keybrd_layout\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_with_keybrd_layout)|
|`Fo`, `FF`|[\<easyclick\>]({{ site.url }}/cheat_sheet/functions/#easyclick)[\<click_left\>]({{ site.url }}/cheat_sheet/functions/#click_left)|
|`Fa`|[\<easyclick\>]({{ site.url }}/cheat_sheet/functions/#easyclick)[\<click_right\>]({{ site.url }}/cheat_sheet/functions/#click_right)|
|`Fm`|[\<easyclick\>]({{ site.url }}/cheat_sheet/functions/#easyclick)[\<click_mid\>]({{ site.url }}/cheat_sheet/functions/#click_mid)|
|`Fh`|[\<easyclick\>]({{ site.url }}/cheat_sheet/functions/#easyclick)|
|`<ctrl-m>`|[\<gridmove\>]({{ site.url }}/cheat_sheet/functions/#gridmove)|

## Insert Mode

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`<Esc-Left>`, `<ctrl-]>`|[\<to_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_gui_normal)|
|`<F8>`|[\<to_instant_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_instant_gui_normal)|
|`<Esc-Down>`|[\<to_resident\>]({{ site.url }}/cheat_sheet/functions/#to_resident)|

## Resident Mode

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`<Esc-Left>`|[\<to_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_gui_normal)|
|`<Esc-Down>`|[\<to_resident\>]({{ site.url }}/cheat_sheet/functions/#to_resident)|
|`<Esc-up>`|[\<to_insert\>]({{ site.url }}/cheat_sheet/functions/#to_insert)|

## Command Mode

### Mode Transition

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`guinormal`, `gn`|[\<to_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_gui_normal)|
|`resident`|[\<to_resident\>]({{ site.url }}/cheat_sheet/functions/#to_resident)|
|`insert`, `i`|[\<to_insert\>]({{ site.url }}/cheat_sheet/functions/#to_insert)|

### System Commands

|**Trigger Commands**|**Called Command**|
|:---:|:---:|
|`exit`|[\<exit\>]({{ site.url }}/cheat_sheet/functions/#exit)|
|`set`|[\<set\>]({{ site.url }}/cheat_sheet/functions/#set)|
|`${MODE}map`|[\<${MODE}map\>]({{ site.url }}/cheat_sheet/functions/#map)|
|`${MODE}noremap`|[\<${MODE}noremap\>]({{ site.url }}/cheat_sheet/functions/#noremap)|
|`${MODE}unmap`|[\<${MODE}unmap\>]({{ site.url }}/cheat_sheet/functions/#unmap)|
|`${MODE}mapclear`|[\<${MODE}mapclear\>]({{ site.url }}/cheat_sheet/functions/#mapclear)|
|`command`, `com`|[\<command\>]({{ site.url }}/cheat_sheet/functions/#command)|
|`delcommand`, `delc`|[\<delcommand\>]({{ site.url }}/cheat_sheet/functions/#delcommand)|
|`comc`, `comclear`|[\<comclear\>]({{ site.url }}/cheat_sheet/functions/#comclear)|
|`so`, `source`|[\<source\>]({{ site.url }}/cheat_sheet/functions/#source)|

<br>
<br>
<br>
<br>
