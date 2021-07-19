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

|Bindings|ID|Note|
|:---:|:---:|:---|
|`<Esc-Left>`|**to_gui_normal**|GUI Normal Mode|
|`<Esc-Down>`|**to_resident**|Resident Mode|

### Mouse

|Bindings|ID|Note|
|:---:|:---:|:---|
|`h`, `<BS>`, `<Left>`|**move_cursor_left**|Move the mouse cursor to left|
|`l`, `<Space>`, `<Right>`|**move_cursor_right**|Move the mouse cursor to right|
|`k`, `-`, `<Up>`|**move_cursor_up**|Move the mouse cursor up|
|`j`, `+`, `<Down>`|**move_cursor_down**|Move the Mouse Cursor Down|
|`0`, `^`, `<Home>`|**jump_cursor_to_left**|Jump the Mouse cursor to left|
|`$`, `<End>`|**jump_cursor_to_right**|Jump the mouse cursor to right|
|`gg`|**jump_cursor_to_top**|Jump the mouse cursor to top|
|`G`|**jump_cursor_to_bottom**|Jump the mouse cursor to bottom|
|`gm`|**jump_cursor_to_hcenter**|Jump the mouse cursor to horizontal center|
|`M`|**jump_cursor_to_vcenter**|Jump the mouse cursor to vertical center|
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

### Hotkey

|Bindings|ID|Note|
|:---:|:---:|:---|
|`y`, `yy`, `Y`|**hotkey_copy**|Copy|
|`p`, `P`|**hotkey_paste**|Paste|
|`dd`, `D`|**hotkey_cut**|Cut|
|`x`, `<Del>`|**hotkey_delete**|Delete|
|`X`|**hotkey_backspace**|BackSpace|

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
