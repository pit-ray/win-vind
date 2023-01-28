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
#### System Design
- Fast binding system
- Vim-like mode management
- .vimrc style configuration
- Running in user permission
- Oneshot use for Vim or AHK (e.g. `$ win-vind -f easy_click_left`)

#### Top Features
- GUI operation without mouse
- Vim emulation everywhere
- Process launcher like `:!vim ~/.vimrc` on resident command line
- Low-level key mapping (e.g. Capslock -> Ctrl)
- Keystroke macros
- Tiling window manager
- Hinting feature like Vimium or EasyMotion for GUI.


##### Demo

<img src="{{ site.url }}/imgs/4xxdemo.gif" title="Demo" width="624">  


##### Configuration File Example

You can configure it in .vimrc style. What you can do in .vindrc is switch options, set parameters, remap low-level keys, and define function bindings.

```vim
" options
set shell = cmd
set cmd_fontsize = 14
set cmd_fontname = Consolas
set blockstylecaret
set blockstylecaret_mode = solid

" bindings
imap <capslock> {<ctrl>}

inoremap <alt><alt> <easy_click_left>
inoremap <win-enter> <window_resizer>

noremap <ctrl-1> :! gvim<cr>
noremap <ctrl-2> :e http://example.com<cr>

enoremap t ggyyGp
```

## License
This software is provided by [MIT License](https://github.com/pit-ray/win-vind/blob/master/LICENSE.txt).  
