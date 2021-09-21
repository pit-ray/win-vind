---
layout: page
disable_anchors: true
title: FAQ - win-vind
nav: FAQ
order: 3
icon: question
---

### Why is the name win-vind?  
win-vind means <u>vi</u>m key b<u>ind</u>er for <u>win</u>dows.  
<br>

### What is the GUI visual mode?  
It is the same as holding the mouse left pressing.  
<br>

### How do I make a mode transition without ESC?
ESC is just the default, and you can change everything as you like. There are several ways to solve this problem, but here is a simple solution to use key mapping in the internal scope by `noremap`. For example, make win-vind recognize `<CapsLock>` as `<ESC>`.

```vim
" In insert mode, it is recommended to do a low-level mapping
" to another key once to prevent the message from being passed to Windows.
imap <capslock> <f20>
inoremap <f20> <esc>

" Internal scope key mapping
gnnoremap <capslock> <esc>
gvnoremap <capslock> <esc>
ennoremap <capslock> <esc>
evnoremap <capslock> <esc>
```
