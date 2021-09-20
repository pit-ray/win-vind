---
layout: full
homepage: true
disable_anchors: true
title: Home - win-vind
nav: Home
icon: home
translation_label: English
description: Vim Key Binder for Windows
---


## Description  

**win-vind** is a toolbox to operate Windows inspired by Vim and its plugins.  

### Features
- Lightweight binding system
- Vim-like mode management
- GUI operation without mouse
- Vim emulation everywhere
- Low-level key mapping (e.g. Capslock -> Ctrl)
- Running in user permission
- Oneshot use for Vim or AHK (e.g. `$ win-vind -f easy_click_left`)
- Tiling window manager
- Hinting feature like Vimium or EasyMotion for GUI.


##### Demo

<img src="{{ site.url }}/imgs/4xxdemo.gif" title="Demo" width="624">  


##### Configuration File Example

You can configure it in .vimrc style. What you can do in .vindrc is switch options, set parameters, remap low-level keys, and define function bindings.

```vim
" ------------ Example ------------
" Virtual command line options
set shell = cmd
set cmd_fontsize = 14
set cmd_roughpos = LowerLeft
set cmd_maxchar = 100

" Enable block style caret
set blockstylecaret
set blockstylecaret_mode = solid

" Low-level key mapping in resident mode
rmap <capslock> <ctrl>

" Define bindings in GUI normal mode
gnnoremap <c-h> select_left_window
gnnoremap <c-l> select_right_window
gnnoremap <c-k> select_upper_window
gnnoremap <c-j> select_lower_window

" Define bindings in insert mode
imap <capslock> <f16>
inoremap <f16> to_edi_normal

imap <ralt> <f17>
inoremap <f17> easy_click_left

imap <app> <f18>
inoremap <f18> window_resizer
```

## License
This software is provided by [MIT License](https://github.com/pit-ray/win-vind/blob/master/LICENSE.txt).  
