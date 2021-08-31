---
layout: default
title: Resident Mode
nav_order: 6
descripption: "Command lists of win-vind."
parent: Cheat Sheet
---

# Resident Mode

## Default

|Bindings|ID|
|:---:|:---:|
|`<Esc-Left>`|**to_gui_normal**|
|`<Esc-Right>`|**to_edi_normal**|
|`<Esc-Up>`|**to_insert**|

<hr>

## Customize

Write `.vindrc` in the following way. The avaliable commands are `rnoremap`, `runmap`, and `rmapclear`.

**Sample**
```vim
" ~/.win-vind/.vindrc

" Transition to Instant GUI Normal Mode with Ctrl + Shift + up
rnoremap <Ctrl-Shift-Up> to_instant_gui_normal
rmap <Capslock> <Esc> " Map <Capslock> to <Esc> with low-level
rumap <Esc-Left>      " Delete <Esc-Left> command
rmapclear             " Remove all bindings of Resident Mode
```

<br>
<br>
<br>
<br>
