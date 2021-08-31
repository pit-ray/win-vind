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

|Bindings|ID|
|:---:|:---:|
|`<Esc-Left>`|**to_gui_normal**|
|`<Esc-Down>`|**to_resident**|
|`i`|**to_insert**|
|`v`|**to_edi_visual**|
|`V`|**to_edi_visual_line**|
|`:`|**to_command**|

### Mouse

|Bindings|ID|
|:---:|:---:|
|`<C-y>`, `<C-k>`|**scroll_up**|
|`<C-e>`, `<C-j>`|**scroll_down**|
|`<C-u>`|**scroll_up_halfpage**|
|`<C-d>`|**scroll_down_halfpage**|
|`<C-b>`|**scroll_up_onepage**|
|`<C-f>`|**scroll_down_onepage**|
|`zh`, `<C-h>`|**scroll_left**|
|`zl`, `<C-l>`|**scroll_right**|
|`zH`|**scroll_left_halfpage**|
|`zL`|**scroll_right_halfpage**|

### HotKey

|Bindings|ID|Note|
|:---:|:---:|:---|
|`<C-r>`|**redo**|
|`u`, `U`|**undo**|
|`/`, `?`|**search_pattern**|

### Tab

|Bindings|ID|
|:---:|:---:|
|`gT`|**switch_to_left_tab**|
|`gt`|**switch_to_right_tab**|


### Vim Emulation

#### Mode

|Bindings|ID|
|:---:|:---:|
|`I`, `gI`|**to_insert_BOL**|
|`a`|**to_insert_append**|
|`A`|**to_insert_EOL**|
|`o`|**to_insert_nlbelow**|
|`O`|**to_insert_nlabove**|

#### Move

|Bindings|ID|
|:---:|:---:|
|`h`, `<C-h>`, `<BS>`, `<Left>`|**move_caret_left**|
|`l`, `<Space>`, `<Right>`|**move_caret_right**|
|`k`, `-`, `gk`, `<C-p>`, `<Up>`|**move_caret_up**|
|`j`, `+`, `gj`, `<C-n>`, `<Down>`, `<Enter>`, `<C-m>`|**move_caret_down**|
|`w`|**move_caret_word_forward**|
|`b`|**move_caret_word_backward**|
|`W`|**move_caret_nonblank_word_forward**|
|`B`|**move_caret_nonblank_word_backward**|

#### Jump

|Bindings|ID|
|:---:|:---:|
|`0`, `<Home>`, `g0`|**jump_caret_to_BOL**|
|`$`, `<End>`, `g$`|**jump_caret_to_EOL**|
|`gg`|**jump_caret_to_BOF**|
|`G`|**jump_caret_to_EOF**|

#### Edit

|Bindings|ID|
|:---:|:---:|
|`yy`, `Y`|**yank_line**|
|`y`|**yank_with_motion**|
|`p`|**put_after**|
|`P`|**put_before**|
|`dd`|**delete_line**|
|`D`|**delete_line_until_EOL**|
|`x`, `<Del>`|**delete_after**|
|`X`|**delete_before**|
|`J`|**join_next_line**|
|`r`|**replace_char**|
|`R`|**replace_sequence**|
|`~`|**switch_char_case**|
|`d`|**delete_with_motion**|
|`c`|**change_with_motion**|
|`cc`, `S`|**change_line**|
|`s`|**change_char**|
|`C`|**change_until_EOL**|

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
