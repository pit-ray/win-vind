---
layout: page
show_in_menu: false
disable_anchors: true
title: Normal Mappings - win-vind
nav: Normal Mappings
---

## GUI Normal Mode

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<ctrl-[>`, `i`, `<esc-right>`|[\<to_edi_normal\>]({{ site.url }}/cheat_sheet/functions/#to_edi_normal)|

## Editor Normal Mode

### Mode Transition

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<ctrl-]>`, `<esc-left>`|[\<to_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_gui_normal)|
|`<esc-down>`|[\<to_resident\>]({{ site.url }}/cheat_sheet/functions/#to_resident)|
|`:`|[\<to_command\>]({{ site.url }}/cheat_sheet/functions/#to_command)|
|`i`|[\<to_insert\>]({{ site.url }}/cheat_sheet/functions/#to_insert)|
|`v`|[\<to_edi_visual\>]({{ site.url }}/cheat_sheet/functions/#to_edi_visual)|
|`v`|[\<to_edi_visual_line\>]({{ site.url }}/cheat_sheet/functions/#to_edi_visual_line)|

### Scrolling

|**Trigger Commands**|**Called Commands**|
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

### Shortcut

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<c-r>`|[\<redo\>]({{ site.url }}/cheat_sheet/functions/#redo)|
|`u`, `u`|[\<undo\>]({{ site.url }}/cheat_sheet/functions/#undo)|
|`gt`|[\<switch_to_left_tab\>]({{ site.url }}/cheat_sheet/functions/#switch_to_left_tab)|
|`gt`|[\<switch_to_right_tab\>]({{ site.url }}/cheat_sheet/functions/#switch_to_right_tab)|
|`?`, `/`|[\<search_pattern\>]({{ site.url }}/cheat_sheet/functions/#search_pattern)|

### Mode Transition on Vim Emulation

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`gi`, `i`|[\<to_insert_BOL\>]({{ site.url }}/cheat_sheet/functions/#to_insert_bol)|
|`a`|[\<to_insert_append\>]({{ site.url }}/cheat_sheet/functions/#to_insert_append)|
|`a`|[\<to_insert_EOL\>]({{ site.url }}/cheat_sheet/functions/#to_insert_eol)|
|`o`|[\<to_insert_nlbelow\>]({{ site.url }}/cheat_sheet/functions/#to_insert_nlbelow)|
|`o`|[\<to_insert_nlabove\>]({{ site.url }}/cheat_sheet/functions/#to_insert_nlabove)|

### Caret Movement on Vim Emulation

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<left>`, `h`, `<c-h>`, `<bs>`|[\<move_caret_left\>]({{ site.url }}/cheat_sheet/functions/#move_caret_left)|
|`<space>`, `<right>`, `l`|[\<move_caret_right\>]({{ site.url }}/cheat_sheet/functions/#move_caret_right)|
|`<up>`, `gk`, `-`, `<c-p>`, `k`|[\<move_caret_up\>]({{ site.url }}/cheat_sheet/functions/#move_caret_up)|
|`gj`, `<enter>`, `j`, `+`, `<c-m>`, `<c-n>`, `<down>`|[\<move_caret_down\>]({{ site.url }}/cheat_sheet/functions/#move_caret_down)|
|`w`|[\<move_fwd_word\>]({{ site.url }}/cheat_sheet/functions/#move_fwd_word)|
|`b`|[\<move_bck_word\>]({{ site.url }}/cheat_sheet/functions/#move_bck_word)|
|`w`|[\<move_fwd_bigword\>]({{ site.url }}/cheat_sheet/functions/#move_fwd_bigword)|
|`b`|[\<move_bck_bigword\>]({{ site.url }}/cheat_sheet/functions/#move_bck_bigword)|
|`e`|[\<move_end_word\>]({{ site.url }}/cheat_sheet/functions/#move_end_word)|
|`e`|[\<move_end_bigword\>]({{ site.url }}/cheat_sheet/functions/#move_end_bigword)|
|`ge`|[\<move_bckend_word\>]({{ site.url }}/cheat_sheet/functions/#move_bckend_word)|
|`ge`|[\<move_bckend_bigword\>]({{ site.url }}/cheat_sheet/functions/#move_bckend_bigword)|

### Caret Jumping on Vim Emulation

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`g0`, `0`, `<home>`|[\<jump_caret_to_BOL\>]({{ site.url }}/cheat_sheet/functions/#jump_caret_to_bol)|
|`<end>`, `g$`, `$`|[\<jump_caret_to_EOL\>]({{ site.url }}/cheat_sheet/functions/#jump_caret_to_eol)|
|`gg`|[\<jump_caret_to_BOF\>]({{ site.url }}/cheat_sheet/functions/#jump_caret_to_bof)|
|`g`|[\<jump_caret_to_EOF\>]({{ site.url }}/cheat_sheet/functions/#jump_caret_to_eof)|

### Edit on Vim Emulation

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`yy`, `y`|[\<yank_line\>]({{ site.url }}/cheat_sheet/functions/#yank_line)|
|`y`|[\<yank_with_motion\>]({{ site.url }}/cheat_sheet/functions/#yank_with_motion)|
|`p`|[\<put_after\>]({{ site.url }}/cheat_sheet/functions/#put_after)|
|`p`|[\<put_before\>]({{ site.url }}/cheat_sheet/functions/#put_before)|
|`dd`|[\<delete_line\>]({{ site.url }}/cheat_sheet/functions/#delete_line)|
|`d`|[\<delete_line_until_EOL\>]({{ site.url }}/cheat_sheet/functions/#delete_line_until_eol)|
|`x`, `<del>`|[\<delete_after\>]({{ site.url }}/cheat_sheet/functions/#delete_after)|
|`x`|[\<delete_before\>]({{ site.url }}/cheat_sheet/functions/#delete_before)|
|`j`|[\<join_next_line\>]({{ site.url }}/cheat_sheet/functions/#join_next_line)|
|`r`|[\<replace_char\>]({{ site.url }}/cheat_sheet/functions/#replace_char)|
|`r`|[\<replace_sequence\>]({{ site.url }}/cheat_sheet/functions/#replace_sequence)|
|`~`|[\<switch_char_case\>]({{ site.url }}/cheat_sheet/functions/#switch_char_case)|
|`d`|[\<delete_with_motion\>]({{ site.url }}/cheat_sheet/functions/#delete_with_motion)|
|`c`|[\<change_with_motion\>]({{ site.url }}/cheat_sheet/functions/#change_with_motion)|
|`cc`, `s`|[\<change_line\>]({{ site.url }}/cheat_sheet/functions/#change_line)|
|`s`|[\<change_char\>]({{ site.url }}/cheat_sheet/functions/#change_char)|
|`c`|[\<change_until_EOL\>]({{ site.url }}/cheat_sheet/functions/#change_until_eol)|
|`.`|[\<repeat_last_change\>]({{ site.url }}/cheat_sheet/functions/#repeat_last_change)|

## Editor Visual Mode

### Mode Transisiton

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<ctrl-]>`, `<esc-left>`|[\<to_gui_normal\>]({{ site.url }}/cheat_sheet/functions/#to_gui_normal)|
|`<esc-down>`|[\<to_resident\>]({{ site.url }}/cheat_sheet/functions/#to_resident)|
|`:`|[\<to_command\>]({{ site.url }}/cheat_sheet/functions/#to_command)|
|`<ctrl-[>`, `<esc-right>`|[\<to_edi_normal\>]({{ site.url }}/cheat_sheet/functions/#to_edi_normal)|

### Scrolling

|**Trigger Commands**|**Called Commands**|
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

### Caret Movement on Vim Emulation

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<left>`, `h`, `<c-h>`, `<bs>`|[\<move_caret_left\>]({{ site.url }}/cheat_sheet/functions/#move_caret_left)|
|`<space>`, `<right>`, `l`|[\<move_caret_right\>]({{ site.url }}/cheat_sheet/functions/#move_caret_right)|
|`<up>`, `gk`, `-`, `<c-p>`, `k`|[\<move_caret_up\>]({{ site.url }}/cheat_sheet/functions/#move_caret_up)|
|`gj`, `<enter>`, `j`, `+`, `<c-m>`, `<c-n>`, `<down>`|[\<move_caret_down\>]({{ site.url }}/cheat_sheet/functions/#move_caret_down)|
|`w`|[\<move_fwd_word_simple\>]({{ site.url }}/cheat_sheet/functions/#move_fwd_word_simple)|
|`b`|[\<move_bck_word_simple\>]({{ site.url }}/cheat_sheet/functions/#move_bck_word_simple)|

### Caret Jumping on Vim Emulation

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`g0`, `0`, `<home>`|[\<jump_caret_to_BOL\>]({{ site.url }}/cheat_sheet/functions/#jump_caret_to_bol)|
|`<end>`, `g$`, `$`|[\<jump_caret_to_EOL\>]({{ site.url }}/cheat_sheet/functions/#jump_caret_to_eol)|
|`gg`|[\<jump_caret_to_BOF\>]({{ site.url }}/cheat_sheet/functions/#jump_caret_to_bof)|
|`g`|[\<jump_caret_to_EOF\>]({{ site.url }}/cheat_sheet/functions/#jump_caret_to_eof)|

### Edit on Vim Emulation

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`y`|[\<yank_highlight_text\>]({{ site.url }}/cheat_sheet/functions/#yank_highlight_text)|
|`x`, `d`, `x`|[\<delete_highlight_text\>]({{ site.url }}/cheat_sheet/functions/#delete_highlight_text)|
|`s`, `s`, `c`|[\<change_highlight_text\>]({{ site.url }}/cheat_sheet/functions/#change_highlight_text)|

## Insert Mode

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<ctrl-[>`, `<esc-right>`|[\<to_edi_normal\>]({{ site.url }}/cheat_sheet/functions/#to_edi_normal)|

## Resident Mode

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`<esc-right>`|[\<to_edi_normal\>]({{ site.url }}/cheat_sheet/functions/#to_edi_normal)|

## Command Mode

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`edinormal`, `en`|[\<to_edi_normal\>]({{ site.url }}/cheat_sheet/functions/#to_edi_normal)|
|`ev`, `edivisual`|[\<to_edi_visual\>]({{ site.url }}/cheat_sheet/functions/#to_edi_visual)|
|`evl`, `edivisualline`|[\<to_edi_visual_line\>]({{ site.url }}/cheat_sheet/functions/#to_edi_visual_line)|

### Shortcut

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`w`|[\<save\>]({{ site.url }}/cheat_sheet/functions/#save)|

### Vim Emulation

|**Trigger Commands**|**Called Commands**|
|:---:|:---:|
|`-`|[\<move_caret_up\>]({{ site.url }}/cheat_sheet/functions/#move_caret_up)|
|`+`|[\<move_caret_down\>]({{ site.url }}/cheat_sheet/functions/#move_caret_down)|
|`<num>`|[\<jump_caret_to_BOF\>]({{ site.url }}/cheat_sheet/functions/#jump_caret_to_bof)|

<br>
<br>
## See also
- [tiny](../tiny)
- [small](../small)
- [normal](../normal)
- [big](../big)
- [huge](../huge)
<br>
