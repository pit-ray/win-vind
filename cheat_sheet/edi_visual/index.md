---
layout: default
title: Editor Visual Mode
nav_order: 4
descripption: "Command lists of win-vind."
parent: Cheat Sheet
---

# Editor Visual Mode
{: .no_toc}  

<hr>

1. TOC1  
{:toc}

## Default

### Mode

|Bindings|ID|Note|
|:---:|:---:|:---|
|`<Esc-Left>`|**to_gui_normal**|GUI Normal Mode|
|`<c-i>`|**to_resident**|Resident Mode|
|`:`|**to_command**|Command Mode|
|`<Esc-Right>`|**to_edi_normal**|Normal Mode (Vim: <Esc>)|

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

### Vim Emulation
#### Move

|Bindings|ID|Note|
|:---:|:---:|:---|
|`h`, `<C-h>`, `<BS>`, `<Left>`|**move_caret_left**|Move the caret to left (Vim: h)|
|`l`, `<Space>`, `<Right>`|**move_caret_right**|Move the caret to right (Vim: l)|
|`k`, `-`, `gk`, `<C-p>`, `<Up>`|**move_caret_up**|Move the caret up (Vim: k)|
|`j`, `+`, `gj`, `<C-n>`, `<Down>`, `<Enter>`, `<C-m>`|**move_caret_down**|Move the caret down (Vim: j)|
|`w`|**move_caret_word_forward**|Forward with words (Vim: w)|
|`b`|**move_caret_word_backward**|Backward with words (Vim: b)|
|`W`|**move_caret_nonblank_word_forward**|Forward with WORDS (Vim: W)|
|`B`|**move_caret_nonblank_word_backward**|Backward with WORDS (Vim: B)|

#### Jump

|Bindings|ID|Note|
|:---:|:---:|:---|
|`0`, `<Home>`, `g0`|**jump_caret_to_BOL**|Jump the caret to begin of line (Vim: 0)|
|`$`, `<End>`, `g$`|**jump_caret_to_EOL**|Jump the caret to end of line (Vim: $)|
|`gg`|**jump_caret_to_BOF**|Jump the caret to BOF (Vim: gg)|


#### Edit

|Bindings|ID|Note|
|:---:|:---:|:---|
|`y`|**yank_highlight_text**|Yank highlighted texts (Vim: y)|
|`d`, `x`, `X`|**delete_highlight_text**|Delete highlighted texts (Vim: d,x)|
|`c`, `s`, `S`|**change_highlight_text**|Change highlighted texts (Vim: c, s, S)

<hr>

## Customization

Write `.vindrc` in the following way. The avaliable commands are `evnoremap`, `evunmap`, and `evmapclear`.

**Sample**
```vim
" ~/.win-vind/.vindrc

evnoremap <Esc> to_edi_normal " Transition to Editor Normal Mode with <Esc>
evmap <Capslock> <Esc>        " Map <Capslock> to <Esc> with low-level
evumap <Home>                 " Delete <Home> command
evmapclear                    " Remove all bindings of Editor Visual Mode
```

<br>
<br>
<br>
<br>
