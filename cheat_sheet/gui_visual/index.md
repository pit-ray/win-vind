---
layout: default
title: GUI Visual Mode
nav_order: 2
descripption: "Command lists of win-vind."
parent: Cheat Sheet
---

# GUI Visual Mode
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

### Mouse

|Bindings|ID|
|:---:|:---:|
|`h`, `<BS>`, `<Left>`|**move_cursor_left**|
|`l`, `<Space>`, `<Right>`|**move_cursor_right**|
|`k`, `-`, `<Up>`|**move_cursor_up**|
|`j`, `+`, `<Down>`|**move_cursor_down**|
|`0`, `^`, `<Home>`|**jump_cursor_to_left**|
|`$`, `<End>`|**jump_cursor_to_right**|
|`gg`|**jump_cursor_to_top**|
|`G`|**jump_cursor_to_bottom**|
|`gm`|**jump_cursor_to_hcenter**|
|`M`|**jump_cursor_to_vcenter**|
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

### Hotkey

|Bindings|ID|
|:---:|:---:|
|`y`, `yy`, `Y`|**hotkey_copy**|
|`p`, `P`|**hotkey_paste**|
|`dd`, `D`|**hotkey_cut**|
|`x`, `<Del>`|**hotkey_delete**|
|`X`|**hotkey_backspace**|

<hr>

## Customize

Write `.vindrc` in the following way. The avaliable commands are `gvnoremap`, `gvunmap`, and `gvmapclear`.

**Sample**
```vim
" ~/.win-vind/.vindrc

gvnoremap <Esc> to_gui_normal " Transition to GUI Normal Mode with <Esc>
gvmap <Capslock> <Esc>        " Map <Capslock> to <Esc> with low-level
gvumap gg                     " Delete gg command
gvmapclear                    " Remove all bindings of GUI Visual
```

<br>
<br>
<br>
<br>
