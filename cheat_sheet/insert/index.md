---
layout: default
title: Insert Mode
nav_order: 5
descripption: "Command lists of win-vind."
parent: Cheat Sheet
---

# Insert Mode

## Default

|Bindings|ID|
|:---:|:---:|
|`<Esc-Left>`|**to_gui_normal**|
|`<Esc-Right>`|**to_edi_normal**|
|`<F8>`|**to_instant_gui_normal**|
|`<Esc-Down>`|**to_resident**|

<hr>

## Customize

Write `.vindrc` in the following way. The avaliable commands are `inoremap`, `iunmap`, and `imapclear`.

**Sample**
```vim
" ~/.win-vind/.vindrc

" Transition to Instant GUI Normal Mode with capslock
inoremap <Capslock> to_instant_gui_normal
imap <IME> <Esc> " Map <IME> to <Esc> with low-level 
iumap <Esc-Left> " Delete <Esc-Left> command
imapclear        " Remove all bindings of Insert Mode
```

<br>
<br>
<br>
<br>
