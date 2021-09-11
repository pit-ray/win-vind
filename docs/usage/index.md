---
layout: page
title: Usage - win-vind
nav: Usage
disable_anchors: true
---
This software only supports Windows 10 or Windows 11 on real machines. Therefore, it may not work on older Windows or virtual environments such as Wine or Virtual Box.  

If you have any problems or requests, please post them on GitHub Issues.  


## Installation

- Download either the zip version or the installer version that suits your preference. The installer version creates dependencies to the installed directory and the user directory, while the zip version does not create any files outside the unzipped root.  
   [<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>32bit Installer (.exe)</span></span>]({{ site.dl_ins_32 }})
   [<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>32bit Portable (.zip)</span></span>]({{ site.dl_zip_32 }})  
   [<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>64bit Installer (.exe)</span></span>]({{site.dl_ins_64 }})
   [<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>64bit Portable (.zip)</span></span>]({{ site.dl_zip_64 }})
   <br>

- When you run win-vind.exe for the first time, it will start in Insert mode, so you may not know if it is working, but if you see the icon in the task tray, it is working properly.
   <p align="center">
   <img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/taskbar.jpg?raw=true" width=500 >  
   <p align="center">Like this</p>
   </p>


## Note 
- `:exit` is the recommended termination.
- `<F8> + <F9>` is safe forced termination.
- win-vind could **not** operate some windows given high-rank authorization than itself. For example, if you start **Task Manager** having the highest authorization and select its window, you cannot move, click or scroll the mouse cursor by win-vind. If you want to operate all windows, I recommend giving win-vind the administrator authorization. (Please use **Task Scheduler**).


## Quick Tutorial

### 1. Mode Transition

The basic concept is the same as Vim, but there are two **Normal Mode** and two **Visual Mode**, and **Resident Mode**.   

<p align="center">
<img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/mode_overview.png?raw=true" width=600>  
<p align="center">Default mode layer overview</p>
</p>

The two groups are GUI mode and Editor mode. The former group is for the operation of windows and mouse cursor, and the latter group is for Vim emulation on input forms in web pages or Microsoft Office Word.  

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


### 2. GUI Operation and Window Operation  

1. Switch to **GUI Normal Mode** with `<Esc-Left>`.
1. Please inputs `:!mspaint` to launch Microsoft Paint.
1. You can call **EasyClick** with `FF`.
   <p align="center">
   <img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/EasyClickDemo.gif?raw=true">
   <p align="center">EasyClick Demo</p>
   </p>
1. Let's select windows with `<C-w>h` or `<C-w>l`.
1. Please select Microsoft Paint and close it with `:close`.



### 3. Customize Options and Maps
win-vind uses **Run Commands** style configuration method. If you've ever written a `.vimrc`, it's easy to make it your win-vind.  Generally, there are three levels of key mapping: **key2key**, **keyset2keyset**, and **cmd2cmd**. **key2key** maps one key to one key. **keyset2keyset** maps a keyset to a keyset(e.g. `c-s` to `<m-h>`). **cmd2cmd** performs sequential mapping (e.g. `qq` to `<c-w>e`).  

The keyset syntax uses the same expression as in Vim, where keys are connected by `-` between `<` and `>`. However, there is no limit to the number of combinations, and you can connect as many as you like. (e.g. `<Esc-b-c-a-d>`).  

The following commands are supported. By the way, `{` and `}` themselves are not part of the syntax.  

|**Syntax**|**Effect**|
|:---|:---|
|`set {option}`|Set the value of the option to **true**.|
|`set no{option}`|Set the value of the option to **false**.|
|`set {option} = {val}`|Set a value of the option. The value can be a string or a number that allows floating points. The string does not need quotation marks, and any character after the non-white character will be handled as the value. White spaces at both ends of the equals sign are ignored.|
|`{mode}map {in-key} {out-key}`|It performs **key2key** mapping with low-level. The keymap influences Windows as a whole, not just the win-vind scope. Therefore, use it with caution.|
|`{mode}noremap {in-cmd} {func-id}`|It defines the map to call the function.|
|`{mode}noremap {in-keyset} {out-keyset}`|It performs **keyset2keyset** mapping in win-vind scope. However, since the `{func-id}` definition has higher priority than its syntax, it may result in exactly one remap. For example `inoremap k move_cursor_left` and `inoremap f k` then `f` will be mapped to `move_cursor_left` instead of `move_cursor_up`.|
|`{mode}unmap {in-cmd}`|Remove the map corresponding to the `{in-cmd}`.|
|`{mode}mapclear`|Delete all maps.|
|`command {in-cmd} {func-id}`|It defines the command to call the function.|
|`delcommand {in-cmd}`|Remove the command corresponding to the `{in-cmd}`.|
|`comclear`|delete all commands.|

`{mode}` is the [Mode Prefix](https://pit-ray.github.io/win-vind/cheat_sheet/keyword_lists/#mode-prefix). And only **UTF-8** format is supported for `.vindrc`.  


Let's do the last tutorial!  

1. Go to **Insert Mode**.
1. This time, we will try **Instant GUI Normal Mode** with `<F8>`. It allows us to temporarily switch to the **GUI Normal Mode**.  

   <p align="center">
   <img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/instant_gui_normal_mode.jpg?raw=true" width=450 >  
   <p align="center">Instant GUI Normal Demo</p>
   </p>
1. Open your `.vindrc` with `:e ~/.win-vind/.vindrc`.  
   <p align="center">
   <img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/edit_vindrc_demo.jpg?raw=true" width=450 >  
   <p align="center">Edit .vindrc Demo</p>
   </p>
1. Write following commands into `.vindrc`.
   ```vim
   set cmd_fontname = Times New Roman
   imap <Capslock> <ctrl>
   inoremap <Alt> easy_click_left
   ```
1. If you done, try reloading `.vindrc` with `:source` of win-vind. (No arguments are needed.)
   <p align="center">
   <img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/source_demo.jpg?raw=true" width=450 >  
   <p align="center">Reload Demo</p>
   </p>
1. In **Insert Mode**, you can use `<Capslock>` instead of `<Ctrl>` and operate GUI with hinting like **EasyMotion** or **Vimium** by `<Alt>`.

<br>
<br>
<br>
<br>
