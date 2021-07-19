---
layout: default
title: Editor Normal Mode
nav_order: 3
descripption: "Command lists of win-vind."
parent: Cheat Sheet
---

# Editor Normal Mode
{: .no_toc}  

<hr>

1. TOC1  
{:toc}

## Default

### Mode

|Bindings|ID|Note|
|:---:|:---:|:---|
|`<Esc-Left>`|**to_gui_normal**|GUI Normal Mode|
|`<Esc-Down>`|**to_resident**|Resident Mode|
|`i`|**to_insert**|Insert Mode|
|`v`|**to_edi_visual**|Visual Mode|
|`V`|**to_edi_visual_line**|Visual-Line Mode|
|`:`|**to_command**|Command Mode|

### Mouse

|Bindings|ID|Note|
|:---:|:---:|:---|
|`<C-y>`, `<C-k>`|**scroll_up**|Scroll a mouse wheel up|
|`<C-e>`, `<C-j>`|**scroll_down**|Scroll a mouse wheel down|
|`<C-u>`|**scroll_up_halfpage**|Scroll a mouse wheel up with a half page|
|`<C-d>`|**scroll_down_halfpage**|Scroll a mouse wheel down with a half page|
|`<C-b>`|**scroll_up_onepage**|Scroll a mouse wheel up with a page|
|`<C-f>`|**scroll_down_onepage**|Scroll a mouse wheel down With a page|
|`zh`, `<C-h>`|**scroll_left**|Scroll a mouse wheel left|
|`zl`, `<C-l>`|**scroll_right**|Scroll a mouse wheel right|
|`zH`|**scroll_left_halfpage**|Scroll a mouse wheel left With a half page|
|`zL`|**scroll_right_halfpage**|Scroll a mouse wheel right with a half page|

### HotKey

|Bindings|ID|Note|
|:---:|:---:|:---|
|`<C-r>`|**redo**|Redo|
|`u`, `U`|**undo**|Undo|
|`/`, `?`|**search_pattern**|Search Pattern|

### Tab

|Bindings|ID|Note|
|:---:|:---:|:---|:---|
|`gT`|**switch_to_left_tab**|Switch to a left tab|
|`gt`|**switch_to_right_tab**|Switch to a right tab|


### Vim Emulation

#### Mode

|Bindings|ID|Note|
|:---:|:---:|:---|
|`I`, `gI`|**to_insert_BOL**|Insert to begin of line|
|`a`|**to_insert_append**|Append after a caret|
|`A`|**to_insert_EOL**|Append end of line|
|`o`|**to_insert_nlbelow**|Begin new line below a caret|
|`O`|**to_insert_nlabove**|Begin new line above a caret|

#### Move

|Bindings|ID|Note|
|:---:|:---:|:---|
|`h`, `<C-h>`, `<BS>`, `<Left>`|**move_caret_left**|Move the caret to left|
|`l`, `<Space>`, `<Right>`|**move_caret_right**|Move the caret to right|
|`k`, `-`, `gk`, `<C-p>`, `<Up>`|**move_caret_up**|Move the caret up|
|`j`, `+`, `gj`, `<C-n>`, `<Down>`, `<Enter>`, `<C-m>`|**move_caret_down**|Move the caret down|
|`w`|**move_caret_word_forward**|Forward with words|
|`b`|**move_caret_word_backward**|Backward with words|
|`W`|**move_caret_nonblank_word_forward**|Forward with WORDS|
|`B`|**move_caret_nonblank_word_backward**|Backward with WORDS|

#### Jump

|Bindings|ID|Note|
|:---:|:---:|:---|
|`0`, `<Home>`, `g0`|**jump_caret_to_BOL**|Jump the caret to begin of line|
|`$`, `<End>`, `g$`|**jump_caret_to_EOL**|Jump the caret to end of line|
|`gg`|**jump_caret_to_BOF**|Jump the caret to BOF|
|`G`|**jump_caret_to_EOF**|Jump the caret to EOF|

#### Edit

|Bindings|ID|Note|
|:---:|:---:|:---|
|`yy`, `Y`|**yank_line**|Yank lines|
|`y`|**yank_with_motion**|Yank lines with motion|
|`p`|**put_after**|Put texts after the caret|
|`P`|**put_before**|Put texts before the caret|
|`dd`|**delete_line**|Delete lines|
|`D`|**delete_line_until_EOL**|Delete texts until end of line|
|`x`, `<Del>`|**delete_after**|Delete chars after the caret|
|`X`|**delete_before**|Delete chars before the caret|
|`J`|**join_next_line**|Join a next line|
|`r`|**replace_char**|Replace a char|
|`R`|**replace_sequence**|Replace Mode|
|`~`|**switch_char_case**|Switch char case|
|`d`|**delete_with_motion**|Delete texts with motion|
|`c`|**change_with_motion**|Change texts with motion|
|`cc`, `S`|**change_line**|Change Lines|
|`s`|**change_char**|Change Characters|
|`C`|**change_until_EOL**|Change until EOL|

<hr>

## Customize

Write `.vindrc` in the following way. The avaliable commands are `ennoremap`, `enunmap`, and `enmapclear`.

**Sample**
```vim
" ~/.win-vind/.vindrc

ennoremap ; to_command " Transition to Command Mode with ;
enmap <Capslock> <Esc> " Map <Capslock> to <Esc> with low-level
enumap <Left>          " Delete <Left> command
enmapclear             " Remove all bindings of Edi Normal Mode
```

<br>
<br>
<br>
<br>
