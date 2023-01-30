---
layout: full
homepage: true
disable_anchors: true
title: Home - win-vind
nav: Home
icon: home
translation_label: English
description: <img src="imgs/banner.gif" class="img-fluid">
---

## What is this?
**win-vind** provides a lightweight hybrid UI system of CUI and GUI for Windows.  
By installing this tool, you will be able to control the Windows GUI in the same way as Vim.

### 1. Vim-user friendly.
All configuration methods and mode concepts are derived from Vim, allowing for Vim-like UI operation.  
Vim users only need to understand win-vind's macro features and additional mode concepts with little learning cost.  

### 2. There are many useful built-in commands.
You don't have to worry about complicated scripts and dependencies like with traditional key binding tools.  
You can freely create user-defined commands by combining low-level optimized built-in commands.

### 3. Very portable and fully open source.
win-vind is a small single binary with no dependencies that runs with user permissions.   
It is also available from the command line as a command for UI operations like `$ win-vind -c "ggyyGp"`.


### Top Feature Demo

<video src="https://user-images.githubusercontent.com/42631247/215270933-3365065b-53db-4eca-9fc6-cd03d13e5ab0.mp4" controls class="img-fluid"></video>



### Configuration File Example

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
