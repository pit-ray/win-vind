<h1 align="center">win-vind</h1>

<p align="center">
  <img src="res/icon.png?raw=true" width="150" height="150" />
  <p align="center"><b>Vim Key Binder for Windows</b></p>
  <p align="center">
    <a href="https://scan.coverity.com/projects/pit-ray-win-vind"><img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects/22417/badge.svg"/></a>
    <a href="https://www.codacy.com/gh/pit-ray/win-vind/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=pit-ray/win-vind&amp;utm_campaign=Badge_Grade"><img src="https://app.codacy.com/project/badge/Grade/8f2e6f2826904efd82019f5888574327" /></a>
    <a href="https://github.com/pit-ray/win-vind/actions/workflows/codeql-analysis.yml"><img src="https://github.com/pit-ray/win-vind/actions/workflows/codeql-analysis.yml/badge.svg?branch=master"></a>
    <a href="https://github.com/pit-ray/win-vind/actions/workflows/coverity.yml"><img src="https://github.com/pit-ray/win-vind/actions/workflows/coverity.yml/badge.svg" /> </a>
    <br>  
    <a href="https://github.com/pit-ray/win-vind/actions/workflows/mingw.yml"><img src="https://github.com/pit-ray/win-vind/actions/workflows/mingw.yml/badge.svg?branch=master" /></a>
    <a href="https://github.com/pit-ray/win-vind/actions/workflows/msvc.yml"><img src="https://github.com/pit-ray/win-vind/actions/workflows/msvc.yml/badge.svg" /></a>
    <a href="https://github.com/pit-ray/win-vind/actions/workflows/test.yml"><img src="https://github.com/pit-ray/win-vind/actions/workflows/test.yml/badge.svg" /></a>
    <a href="https://github.com/pit-ray/win-vind/actions/workflows/auto_release.yml"><img src="https://github.com/pit-ray/win-vind/actions/workflows/auto_release.yml/badge.svg" /></a>
  </p>
</p>  



## Description
**win-vind** provides a lightweight hybrid UI system of CUI and GUI for Windows. And everything is inspired by Vim and its plugins. Simply put, it is a Vim-oriented toy box consisting of various useful features.

### Features
- Fast binding system
- Vim-like mode management
- GUI operation without mouse
- Vim emulation everywhere
- Process control like `:!vim ~/.vimrc` on resident command line
- .vimrc style configuration
- Low-level key mapping (e.g. Capslock -> Ctrl)
- Running in user permission
- Oneshot use for Vim or AHK (e.g. `$ win-vind -f easy_click_left`)
- Tiling window manager
- Hinting feature like Vimium or EasyMotion for GUI.


#### Demo

<img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/4xxdemo.gif?raw=true" title="Demo" >

<br>  

## Download
### v4.2.0 (Latest Version)  
Date: 2021/8/1  

#### 32bit  
- <a href="https://github.com/pit-ray/win-vind/releases/download/vv4.2.0/setup_win-vind_v4.2.0_32bit.exe.zip">setup_win-vind_v4.2.0_32bit.exe</a>
- <a href="https://github.com/pit-ray/win-vind/releases/download/vv4.2.0/win-vind_v4.2.0_32bit.zip">win-vind_v4.2.0_32bit.zip</a>  

#### 64bit  
- <a href="https://github.com/pit-ray/win-vind/releases/download/vv4.2.0/setup_win-vind_v4.2.0_64bit.exe.zip">setup_win-vind_v4.2.0_64bit.exe</a>
- <a href="https://github.com/pit-ray/win-vind/releases/download/vv4.2.0/win-vind_v4.2.0_64bit.zip">win-vind_v4.2.0_64bit.zip</a> 


## Usage
You can refer to the installation and quick tutorial in [Usage - win-vind](https://pit-ray.github.io/win-vind/usage/).  


### .vindrc samples  

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

" Define bindings in GUI Normal mode
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

The [Functions](https://pit-ray.github.io/win-vind/cheat_sheet/functions/) page lists all supported functions, the [Options](https://pit-ray.github.io/win-vind/cheat_sheet/options/) document lists parameters and useful extensions, and the [Keyword List](https://pit-ray.github.io/win-vind/cheat_sheet/keyword_lists/) contains keyword notations for writing .vindrc. If you want to know the default bindings, refer to <a href="https://pit-ray.github.io/win-vind/cheat_sheet/">Cheat Sheet - win-vind</a>. 


## Contribute
If you would like to contribute to win-vind, see [CONTRIBUTING.md](https://github.com/pit-ray/win-vind/blob/master/CONTRIBUTING.md).

<br>

## Related Pages
- <a href="https://www.pit-ray.com/archive/category/win-vind">Description Blog of Author in Japanese</a>  
- <a href="https://www.reddit.com/user/pit-ray/posts/">Posts of reddit</a>  

## Similar Projects
- [rcmdnk/vim_ahk](https://github.com/rcmdnk/vim_ahk)
- [microsoft/PowerToys](https://github.com/microsoft/PowerToys)
- [zsims/hunt-and-peck](https://github.com/zsims/hunt-and-peck)

## License  

This software is provided by **MIT License**.  

## Author

- pit-ray
