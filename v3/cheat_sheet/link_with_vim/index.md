---
layout: default
title: Link with Vim
nav_order: 10
parent: Cheat Sheet
descripption: "Link With Vim"
---

# Link With Vim  
{: .no_toc}

<hr>  

1. TOC  
{:toc}

<hr>  


This page describes some tips for using win-vind with Vim.  


## Operation From Vim  

win-vind is prohibited multiple running, but you can operate win-vind from any consoles by using **--func Function-ID** option with the inter-process communication.  

<p align="center">
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/interproc_overview.png?raw=true" width=600/>  
</p>  

**Example** (long version)
```bash
$ ./win-vind.exe --func change_to_normal
```

**Example** (short version)
```bash
$ ./win-vind.exe -f change_to_normal
```

Please export path to **win-vind.exe** and add a following command into .vimrc with <a href="https://github.com/skywind3000/asyncrun.vim">AsyncRun</a> plugin.  

**Example**
```vim
Plug 'skywind3000/asyncrun.vim'

command! GUINormal :AsyncRun win-vind -f change_to_normal
```

You can transit from Vim to win-vind.  
```vim
:GUINormal
```


## Transition To Vim

Please use <a href="https://pit-ray.github.io/win-vind/cheat_sheet/options/#suppress-for-vim">Suppress For Vim</a> option. 

