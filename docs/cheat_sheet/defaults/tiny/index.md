---
layout: page
show_in_menu: false
disable_anchors: true
title: Tiny Mappings - win-vind
nav: Tiny Mappings
---

## GUI Normal Mode

### Mode Transition

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<ctrl-]>`, `<esc-left>`|[\<to_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_gui_normal)|
|`<esc-down>`|[\<to_resident\>]({{ site.url }}/cheat_sheet/functions/#to_resident)|
|`:`|[\<to_command\>]({{ site.url }}/cheat_sheet/functions/#to_command)|
|`i`|[\<click_left\>]({{ site.url }}/cheat_sheet/functions/#click_left)[\<to_insert\>]({{ site.url }}/cheat_sheet/functions/#to_insert)|

### Mouse Movement

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`h`, `<bs>`, `<left>`|[\<move_cursor_left\>]({{ site.url }}/cheat_sheet/functions/#move_cursor_left)|
|`<right>`, `l`, `<space>`|[\<move_cursor_right\>]({{ site.url }}/cheat_sheet/functions/#move_cursor_right)|
|`k`, `-`, `<up>`|[\<move_cursor_up\>]({{ site.url }}/cheat_sheet/functions/#move_cursor_up)|
|`j`, `<down>`, `+`|[\<move_cursor_down\>]({{ site.url }}/cheat_sheet/functions/#move_cursor_down)|

### Mouse Clicking

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`o`|[\<click_left\>]({{ site.url }}/cheat_sheet/functions/#click_left)|
|`a`|[\<click_right\>]({{ site.url }}/cheat_sheet/functions/#click_right)|

### Mouse Scrolling

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<c-y>`, `<c-k>`|[\<scroll_up\>]({{ site.url }}/cheat_sheet/functions/#scroll_up)|
|`<c-j>`, `<c-e>`|[\<scroll_down\>]({{ site.url }}/cheat_sheet/functions/#scroll_down)|
|`<c-u>`|[\<scroll_up_halfpage\>]({{ site.url }}/cheat_sheet/functions/#scroll_up_halfpage)|
|`<c-d>`|[\<scroll_down_halfpage\>]({{ site.url }}/cheat_sheet/functions/#scroll_down_halfpage)|
|`<c-b>`|[\<scroll_up_onepage\>]({{ site.url }}/cheat_sheet/functions/#scroll_up_onepage)|
|`<c-f>`|[\<scroll_down_onepage\>]({{ site.url }}/cheat_sheet/functions/#scroll_down_onepage)|
|`zh`, `<c-h>`|[\<scroll_left\>]({{ site.url }}/cheat_sheet/functions/#scroll_left)|
|`<c-l>`, `zl`|[\<scroll_right\>]({{ site.url }}/cheat_sheet/functions/#scroll_right)|
|`zh`|[\<scroll_left_halfpage\>]({{ site.url }}/cheat_sheet/functions/#scroll_left_halfpage)|
|`zl`|[\<scroll_right_halfpage\>]({{ site.url }}/cheat_sheet/functions/#scroll_right_halfpage)|

### Mouse Jumping

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`^`, `0`, `<home>`|[\<jump_cursor_to_left\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_left)|
|`<end>`, `$`|[\<jump_cursor_to_right\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_right)|
|`gg`|[\<jump_cursor_to_top\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_top)|
|`G`|[\<jump_cursor_to_bottom\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_bottom)|
|`gm`|[\<jump_cursor_to_hcenter\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_hcenter)|
|`M`|[\<jump_cursor_to_vcenter\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_vcenter)|
|`t`|[\<jump_cursor_to_active_window\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_to_active_window)|

### Complex Mouse Controls

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`f`|[\<jump_cursor_with_keybrd_layout\>]({{ site.url }}/cheat_sheet/functions/#jump_cursor_with_keybrd_layout)|
|`FF`, `Fo`|[\<easyclick\>]({{ site.url }}/cheat_sheet/functions/#easyclick)[\<click_left\>]({{ site.url }}/cheat_sheet/functions/#click_left)|
|`Fa`|[\<easyclick\>]({{ site.url }}/cheat_sheet/functions/#easyclick)[\<click_right\>]({{ site.url }}/cheat_sheet/functions/#click_right)|
|`Fm`|[\<easyclick\>]({{ site.url }}/cheat_sheet/functions/#easyclick)[\<click_mid\>]({{ site.url }}/cheat_sheet/functions/#click_mid)|
|`Fh`|[\<easyclick\>]({{ site.url }}/cheat_sheet/functions/#easyclick)|
|`<ctrl-m>`|[\<gridmove\>]({{ site.url }}/cheat_sheet/functions/#gridmove)|
|`Ao`, `AA`|[\<easyclick_all\>]({{ site.url }}/cheat_sheet/functions/#easyclick_all)[\<click_left\>]({{ site.url }}/cheat_sheet/functions/#click_left)|
|`Aa`|[\<easyclick_all\>]({{ site.url }}/cheat_sheet/functions/#easyclick_all)[\<click_right\>]({{ site.url }}/cheat_sheet/functions/#click_right)|
|`Am`|[\<easyclick_all\>]({{ site.url }}/cheat_sheet/functions/#easyclick_all)[\<click_mid\>]({{ site.url }}/cheat_sheet/functions/#click_mid)|
|`Ah`|[\<easyclick_all\>]({{ site.url }}/cheat_sheet/functions/#easyclick_all)|

## Insert Mode

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<ctrl-]>`, `<Esc-Left>`|[\<to_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_gui_normal)|
|`<F8>`|[\<to_instant_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_instant_gui_normal)|
|`<Esc-Down>`|[\<to_resident\>]({{ site.url }}/cheat_sheet/functions/#to_resident)|

## Resident Mode

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<Esc-Left>`|[\<to_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_gui_normal)|
|`<Esc-Down>`|[\<to_resident\>]({{ site.url }}/cheat_sheet/functions/#to_resident)|
|`<Esc-up>`|[\<to_insert\>]({{ site.url }}/cheat_sheet/functions/#to_insert)|

## Command Mode

### Mode Transition

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`guinormal`, `gn`|[\<to_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_gui_normal)|
|`resident`|[\<to_resident\>]({{ site.url }}/cheat_sheet/functions/#to_resident)|
|`insert`, `i`|[\<to_insert\>]({{ site.url }}/cheat_sheet/functions/#to_insert)|

### System Commands

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`exit`|[\<exit\>]({{ site.url }}/cheat_sheet/functions/#exit)|
|`sleep`|[\<sleep\>]({{ site.url }}/cheat_sheet/functions/#sleep)|
|`set`|[\<set\>]({{ site.url }}/cheat_sheet/functions/#set)|
|`{mode}map`|[\<{mode}map\>]({{ site.url }}/cheat_sheet/functions/#map)|
|`{mode}noremap`|[\<{mode}noremap\>]({{ site.url }}/cheat_sheet/functions/#noremap)|
|`{mode}unmap`|[\<{mode}unmap\>]({{ site.url }}/cheat_sheet/functions/#unmap)|
|`{mode}mapclear`|[\<{mode}mapclear\>]({{ site.url }}/cheat_sheet/functions/#mapclear)|
|`com`, `command`|[\<command\>]({{ site.url }}/cheat_sheet/functions/#command)|
|`delc`, `delcommand`|[\<delcommand\>]({{ site.url }}/cheat_sheet/functions/#delcommand)|
|`comclear`, `comc`|[\<comclear\>]({{ site.url }}/cheat_sheet/functions/#comclear)|
|`source`, `so`|[\<source\>]({{ site.url }}/cheat_sheet/functions/#source)|
|`autocmd`|[\<autocmd_add\>]({{ site.url }}/cheat_sheet/functions/#autocmd_add)|
|`autocmd!`|[\<autocmd_del\>]({{ site.url }}/cheat_sheet/functions/#autocmd_del)|

<br>
<br>
## See also
- [tiny](../tiny)
- [small](../small)
- [normal](../normal)
- [big](../big)
- [huge](../huge)
<br>
