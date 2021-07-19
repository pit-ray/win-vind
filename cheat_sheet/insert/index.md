---
layout: default
title: Insert Mode
nav_order: 5
descripption: "Command lists of win-vind."
parent: Cheat Sheet
---

# Insert Mode

## Default

|Bindings|ID|Note|
|:---:|:---:|:---|
|`<Esc-Left>`|**to_gui_normal**|GUI Normal Mode|
|`<Esc-Right>`|**to_edi_normal**|Editor Normal Mode|
|`<F8>`|**to_instant_gui_normal**|Temporarily switches to GUI mode and performs matching, which can be used as a map-leader.|
|`<Esc-Down>`|**to_resident**|Resident Mode|

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
