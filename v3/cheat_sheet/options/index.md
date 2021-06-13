---
layout: default
title: Options
nav_order: 20
parent: Cheat Sheet for v3
descripption: "Option lists"
---

# Options
{: .no_toc}

<hr>

1. TOC  
{:toc}  

<hr>

## Autotrack Popup
**ID: autotorack_popup**  
It is one of standard options on Windows. For example, if shown **Are you sure you want to move this file to the Recycle Bin?**, it automatically moves the cursor to the popup window.

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/autotrack_demo.gif?raw=true" >

<hr>

## Virtual Command Line
**ID: virtual_cmd_line**  
It makes inputted commands to display and the current mode on the screen. Even if this option disable, the command mode are available. The computing costs by its features are not so big. I recommend to use it.  

<hr>

## Dedicate To One Window
**ID: dedicate_to_window**  
If it enables, you can select one window with <a href="https://pit-ray.github.io/win-vind/cheat_sheet/others/#enable-targeting">Enable Targeting</a> function. In this case, it makes the mode automatically switch to **Editor Normal Mode** on the targeting window. When the foreground window change to another, it makes the mode switch to **GUI Insert Mode**. The targeting becomes disable with <a href="https://pit-ray.github.io/win-vind/cheat_sheet/others/#disable-targeting">Disable Targeting</a> function. In other words, this feature transforms some normal editors compared with **Vim** to **fake Vim**. The computing cost is so small.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/dedicate-demo.gif?raw=true" >
    
<hr>

## Suppress For Vim
**ID: suppress_for_vim**  
It makes the mode of **win-vind** automatically switch to **GUI Insert Mode** on the applications included the strings called **VIM** in an executable file name. Thus, it allows us to smoothly move from win-vind having the same key-bindings as Vim to **Vim** applications.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/for_vim.gif?raw=true" >

<hr>

## Enable Char Cache  
**ID: enable_char_cache**  
**Char Cache** is a very small cache for one character used by **x** or **X** commands. If it is enabled, the clipboard is opened per once typing. Therefore, you will get the same behavior as the original Vim, whereas the performance maybe drop a litte.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/cache_char.gif?raw=true" >

<hr>
