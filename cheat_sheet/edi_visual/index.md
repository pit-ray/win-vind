---
layout: default
title: Editor Visual Mode
nav_order: 5
descripption: "Command lists of win-vind."
parent: Cheat Sheet
---

# Editor Visual Mode
{: .no_toc}  

<hr>

1. TOC1  
{:toc}

## Default Key Config  

### Mode

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`<Esc-Left>`|**to_gui_normal**|GUI Normal Mode||
|`<c-i>`|**to_resident**|Resident Mode||
|`:`|**to_command**|Command Mode||
|`<Esc-Right>`|**to_edi_normal**|Normal Mode (Vim Emulation: <Esc>)||

### Mouse

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`<C-y>`, `<C-k>`|**scroll_up**|Scroll a mouse wheel up||
|`<C-e>`, `<C-j>`|**scroll_down**|Scroll a mouse wheel down||
|`<C-u>`|**scroll_up_halfpage**|Scroll a mouse wheel up with a half page||
|`<C-d>`|**scroll_down_halfpage**|Scroll a mouse wheel down with a half page||
|`<C-b>`|**scroll_up_onepage**|Scroll a mouse wheel up with a page||
|`<C-f>`|**scroll_down_onepage**|Scroll a mouse wheel down With a page||
|`zh`, `<C-h>`|**scroll_left**|Scroll a mouse wheel left||
|`zl`, `<C-l>`|**scroll_right**|Scroll a mouse wheel right||
|`zH`|**scroll_left_halfpage**|Scroll a mouse wheel left With a half page||
|`zL`|**scroll_right_halfpage**|Scroll a mouse wheel right with a half page||

### Vim Emulation
#### Move

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`h`, `<C-h>`, `<BS>`, `<Left>`|**move_caret_left**|Move the caret to left (Vim Emulation: h)||
|`l`, `<Space>`, `<Right>`|**move_caret_right**|Move the caret to right (Vim Emulation: l)||
|`k`, `-`, `gk`, `<C-p>`, `<Up>`|**move_caret_up**|Move the caret up (Vim Emulation: k)||
|`j`, `+`, `gj`, `<C-n>`, `<Down>`, `<Enter>`, `<C-m>`|**move_caret_down**|Move the caret down (Vim Emul|
ation: j)||
|`w`|**move_caret_word_forward**|Forward with words (Vim Emulation: w)||
|`b`|**move_caret_word_backward**|Backward with words (Vim Emulation: b)||
|`W`|**move_caret_nonblank_word_forward**|Forward with WORDS (Vim Emulation: W)||
|`B`|**move_caret_nonblank_word_backward**|Backward with WORDS (Vim Emulation: B)||

#### Jump

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`0`, `<Home>`, `g0`|**jump_caret_to_BOL**|Jump the caret to begin of line (Vim Emulation: 0)||
|`$`, `<End>`, `g$`|**jump_caret_to_EOL**|Jump the caret to end of line (Vim Emulation: $)||
|`gg`|**jump_caret_to_BOF**|Jump the caret to BOF (Vim Emulation: gg)||
|`G`|**jump_caret_to_EOF**|Jump the caret to EOF (Vim Emulation: G)||

#### Edit

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`y`|**yank_highlight_text**|Yank highlighted texts (Vim Emulation: y)||
|`d`, `x`, `X`|**delete_highlight_text**|Delete highlighted texts (Vim Emulation: d,x)||
|`c`, `s`, `S`|**change_highlight_text**|Change highlighted texts (Vim Emulation: c, s, S)||

<hr>

## Customization
