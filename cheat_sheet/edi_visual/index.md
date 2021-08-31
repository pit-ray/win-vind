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

|Bindings|ID|
|:---:|:---:|
|`<Esc-Left>`|**to_gui_normal**|
|`<Esc-Down>`|**to_resident**|
|`:`|**to_command**|
|`<Esc-Right>`|**to_edi_normal**|

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

### Vim Emulation
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
|`y`|**yank_highlight_text**|
|`d`, `x`, `X`|**delete_highlight_text**|
|`c`, `s`, `S`|**change_highlight_text**|

<hr>

## Customize

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
