---
layout: default
title: Usage
nav_order: 3
descripption: "Usage of win-vind"
---
# Usage
{: .no_toc }

This software only supports Windows 10 on real machines. Therefore, it may not work on older Windows or virtual environments such as Wine or Virtual Box.  

If you have any problems or requests, please post them on GitHub Issues.  


<hr>  

1. TOC  
{:toc}

<hr>

## Installation
- Download either the zip version or the installer version that suits your preference. The installer version creates dependencies to the installed directory and the user directory, while the zip version does not create any files outside the unzipped root.  

   **32 bit**  
   [Download .exe (32bit)](https://github.com/pit-ray/win-vind/releases/download/v4.0.0/setup_win-vind_4.0.0_32bit.exe.zip){: .btn }&nbsp;&nbsp;&nbsp;&nbsp;[Download .zip (32bit)](https://github.com/pit-ray/win-vind/releases/download/v4.0.0/win-vind_4.0.0_32bit.zip){: .btn}  

   **64 bit**  
   [Download .exe (64bit)](https://github.com/pit-ray/win-vind/releases/download/v4.0.0/setup_win-vind_4.0.0_64bit.exe.zip){: .btn }&nbsp;&nbsp;&nbsp;&nbsp;[Download .zip (64bit)](https://github.com/pit-ray/win-vind/releases/download/v4.0.0/win-vind_4.0.0_64bit.zip){: .btn}  

- When you run win-vind.exe for the first time, it will start in Insert mode, so you may not know if it is working, but if you see the icon in the task tray, it is working properly.  

   <p align="center">
   <img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/taskbar.jpg?raw=true" width=500 >  
   <p align="center">Like this</p>
   </p>

<hr>

## Note 
- `<F8> + <F9>` is safe forced termination.
- win-vind could **not** operate some windows given high-rank authorization than itself. For example, if you start **Task Manager** having the highest authorization and select its window, you cannot move, click or scroll the mouse cursor by win-vind. If you want to operate all windows, I recommend giving win-vind the administrator authorization. (Please use **Task Scheduler**).

<hr>

## Quick Tutorial

### 1. Mode Transition

The basic concept is the same as Vim, but there are two **Normal Mode** and two **Visual Mode**, and **Resident Mode**.   

<p align="center">
<img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/mode_overview.png?raw=true" width=600>  
<p align="center">Default mode layer overview</p>
</p>

The two groups are GUI mode and Editor mode. The former group is for the operation of windows and mouse cursor without the mouse, and the latter group is for Vim emulation on input forms in web pages or Microsoft Office Word.  

<p align="center">
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/GUIandEditor.jpg?raw=true" width=700>
<p align="center">Concepts of GUI Mode and Editor Mode</p>
</p>

Resident Mode is an evacuation mode to prevent bindings from being collisions with shortcut keys while gaming on Steam or using Vim. For example, if you have added `<Esc>` into the keymap of Insert Mode for faithful Vim emulation, you can use Resident Mode to prevent a feature from being called by Vim's `<Esc>`.  


**Insert Mode** and **Resident Mode** pass all key messages to Windows, while **GUI Normal Mode**, **GUI Visual Mode**, **Edi Normal Mode**, **Edi Visual Mode**, and **Command Mode** block them.  

<p align="center">
<img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/mode_overview_3D.png?raw=true" width=500 >  
<p align="center">Visual Concepts of Mode</p>
</p>

After the boot, win-vind will be in **Insert Mode**. Let's make transitions of mode!  

<hr>  

### 2. GUI Operation and Window Operation  

1. Switch to GUI Normal Mode with `<Esc-Left>`.
1. Please inputs `:!mspaint` to launch Microsoft Paint.
1. You can call **EasyClick** with `FF`.
   <p align="center">
   <img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/EasyClickDemo.gif?raw=true">
   <p align="center">EasyClick Demo</p>
   </p>
1. Let's select windows with `<C-w>h` or `<C-w>l`.
1. Please select Microsoft Paint and close it with `:close`.


<hr>  

### 3. Customize Options and Maps
win-vind uses a **Run Commands** style configuration method. If you've ever written a `.vimrc`, it's easy to make it your win-vind. The following commands are supported.

<table>
  <tr>
  <th>Command</th>
  <th>Syntax</th>
  <th>Note</th>
  </tr>

  <tr>
  <td rowspan="3" border=1><code>set</code></td>
  <td><code>set {bool-option}</code></td>
  <td></td>
  </tr>

  <tr>
  <td border=1><code>set no{bool-option}</code></td>
  <td></td>
  </tr>

  <tr>
  <td border=1><code>set {key} = {val}</code></td>
  <td></td>
  </tr>

  <tr>
  <td><code>map</code></td>
  <td><code>{mode-prefix}map {input-key} {target-key}</code></td>
  <td></td>
  </tr>

  <tr>
  <td rowspan="2" border=1><code>noremap</code></td>
  <td><code>{mode-prefix}noremap {input-command} {function-id}</code></td>
  <td></td>
  </tr>

  <tr>
  <td border=1><code>{mode-prefix}noremap {input-keyset} {target-keyset}</code></td>
  <td></td>
  </tr>

  <tr>
  <td><code>unmap</code></td>
  <td><code>{mode-prefix}unmap {input-command}</code></td>
  <td></td>
  </tr>

  <tr>
  <td><code>mapclear</code></td>
  <td><code>{mode-prefix}mapclear</code></td>
  <td></td>
  </tr>

  <tr>
  <td><code>command</code></td>
  <td><code>command {input-command} {function-id}</code></td>
  <td></td>
  </tr>

  <tr>
  <td><code>delcommand</code></td>
  <td><code>delcommand {input-command}</code></td>
  <td></td>
  </tr>

  <tr>
  <td><code>comclear</code></td>
  <td><code>comclear</code></td>
  <td></td>
  </tr>
</table>


**Sample**
```vim
" ~/.win-vind/.vindrc

gnnoremap <c-f> easy_click_left     " Call easy_click_left with Ctrl + f
gnnoremap <c-h> select_left_window  " Call select_left_window with Ctrl + h
gnmap <Capslock> <ctrl>             " Map <Capslock> to <Ctrl> with low-level
gnumap FF                           " Delete FF command
gnmapclear                          " Remove all bindings of GUI Normal
```

**Example**
```vim
" str
set initmode=gn
set shell = cmd
set gui_fontname = Times New Roman  "Without quotation

" bool
set suppress_for_vim  " true
set nocharcache       " false

" num (int, float)
set cmd_fadeout = 60
set vscroll_pageratio = 0.25
```

<br>
<br>
<br>
<br>
