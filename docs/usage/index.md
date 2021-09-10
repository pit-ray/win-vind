---
layout: page
title: Usage
disable_anchors: true
---
This software only supports Windows 10 on real machines. Therefore, it may not work on older Windows or virtual environments such as Wine or Virtual Box.  

If you have any problems or requests, please post them on GitHub Issues.  


## Installation
- Download either the zip version or the installer version that suits your preference. The installer version creates dependencies to the installed directory and the user directory, while the zip version does not create any files outside the unzipped root.  

   - [Download 32bit installer (.exe)]({{site.dl_ins_32}})
   - [Download 32bit potable (.zip)]({{site.dl_zip_32}})
   - [Download 64bit installer (.exe)]({{site.dl_ins_64}})
   - [Download 64bit potable (.zip)]({{site.dl_zip_64}})

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

<table>
  <tr>
  <th>Command</th>
  <th>Syntax</th>
  <th>Note</th>
  </tr>

  <tr>
  <td align="center" rowspan="3"><code>set</code></td>
  <td width=350><code>set {option}</code></td>
  <td>Set the value of the option to <strong>true</strong>.</td>
  </tr>

  <tr>
  <td><code>set no{option}</code></td>
  <td>Set the value of the option to <strong>false</strong>.</td>
  </tr>

  <tr>
  <td><code>set {option} = {val}</code></td>
  <td>Set a value of the option. The value can be a string or a number that allows floating points. The string does not need quotation marks, and any character after the non-white character will be handled as the value. White spaces at both ends of the equals sign are ignored.</td>
  </tr>

  <tr>
  <td align="center"><code>map</code></td>
  <td><code>{mode}map {in-key} {out-key}</code></td>
  <td>It performs <strong>key2key</strong> mapping with low-level. The keymap influences Windows as a whole, not just the win-vind scope. Therefore, use it with caution.</td>
  </tr>

  <tr>
  <td align="center" rowspan="2"><code>noremap</code></td>
  <td><code>{mode}noremap {in-cmd} {func-id}</code></td>
  <td>It defines the map to call the function.</td>
  </tr>

  <tr>
  <td><code>{mode}noremap {in-keyset} {out-keyset}</code></td>
  <td>It performs <strong>keyset2keyset</strong> mapping in win-vind scope. However, since the <code>{func-id}</code> definition has higher priority than its syntax, it may result in exactly one remap. For example <code>inoremap k move_cursor_left</code> and <code>inoremap f k</code> then <code>f</code> will be mapped to <code>move_cursor_left</code> instead of <code>move_cursor_up</code>.</td>
  </tr>

  <tr>
  <td align="center"><code>unmap</code></td>
  <td><code>{mode}unmap {in-cmd}</code></td>
  <td>Remove the map corresponding to the <code>{in-cmd}</code>.</td>
  </tr>

  <tr>
  <td align="center"><code>mapclear</code></td>
  <td><code>{mode}mapclear</code></td>
  <td>Delete all maps.</td>
  </tr>

  <tr>
  <td align="center"><code>command</code></td>
  <td><code>command {in-cmd} {func-id}</code></td>
  <td>It defines the command to call the function.</td>
  </tr>

  <tr>
  <td align="center"><code>delcommand</code></td>
  <td><code>delcommand {in-cmd}</code></td>
  <td>Remove the command corresponding to the <code>{in-cmd}</code>.</td>
  </tr>

  <tr>
  <td align="center"><code>comclear</code></td>
  <td><code>comclear</code></td>
  <td>Delete all commands.</td>
  </tr>
</table>

<hr>  

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
