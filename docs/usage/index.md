---
layout: page
title: Usage - win-vind
nav: Usage
icon: map-signs
order: 0
disable_anchors: true
---
This software only supports Windows 10 or Windows 11 on real machines. Therefore, it may not work on older Windows or virtual environments such as Wine or Virtual Box. If you have any problems or requests, please post them to [GitHub Issues](https://github.com/pit-ray/win-vind/issues).  


## Installation

- Download the latest version of the type that suits your preference from the [Downloads page]({{ site.url }}/downloads) and install it.
Note that the installation location and configuration file location will vary depending on the installation method.

- Install and run win-vind.exe.

- If you can see the icon in the system tray, it is working properly.  

   <p align="center">
   <img src="{{ site.url }}/imgs/taskbar.jpg" width=500 >  
   <p align="center">Like this</p>
   </p>


## How to terminate win-vind
- `:exit` is the recommended termination.
- `<F8> + <F9>` is safe forced termination.

> **Note**: win-vind could **not** operate some windows given high-rank authorization than itself. For example, if you start **Task Manager** having the highest authorization and select its window, you cannot move, click or scroll the mouse cursor by win-vind. If you want to operate all windows, I recommend giving win-vind the administrator authorization. (Please use **Task Scheduler**).


## About the concept mode transition

The basic concept is the same as Vim, but there are two **Normal** mode and two **Visual** mode, and **Resident** mode.   

<p align="center">
<img src="{{ site.url }}/imgs/mode_overview.png" width=600>  
<p align="center">Default mode layer overview</p>
</p>

There are two groups: GUI mode and Editor mode. The former allows us to control windows and mouse cursor, etc. 

<p align="center">
<img src="{{ site.url }}/imgs/GUIandEditor.jpg" width=700>
<p align="center">Concepts of GUI Mode and Editor Mode</p>
</p>

The first time win-vind is started, it starts in Insert mode, and the mode can be changed by the command attached to the arrow. The mode is like a binding preset, which is dynamically switched depending on the application.
If Insert mode or Resident mode is selected, the operation is the same as in normal Windows.


The following table shows a comparison between modes.

|**Mode**|**Block key messages?**|**Roles**|
|:---:|:---:|:---:|
|**GUI Normal**|Yes|To control windows and mouse cursor. The default bindings are `<esc>-left>` or `<ctrl-]>`.|
|**GUI Visual**|Yes|To select GUI objects such as icons by holding down the left mouse button. The default binding is `v` in GUI Normal mode.|
|**Edi Normal**|Yes|To emulate Vim in input forms of web pages, Microsoft Office Word, etc. The default bindings are `<esc-right>` or `<ctrl-[>`.|
|**Edi Visual**|Yes|To select text as in Vim. The default binding is `v` in Edi Normal mode.|
|**Insert**|No|Basic input operations remain the same as in conventional Windows, but specific functions such as EasyClick are added on a daily basis. The default binding is `i`.|
|**Resident**|No|Resident mode is an evacuation mode to prevent bindings from being collisions with shortcut keys while gaming on Steam or using Vim. The default binding is `<esc-down>`.|
|**Command**|Yes|To execute commands visually using a Vim-like virtual command line that is overlaid on the screen. The default binding is `:`.|

&nbsp;

<p align="center">
<img src="{{ site.url }}/imgs/mode_overview_3D.png" width=500 >  
</p>

The initial startup mode can be changed with `initmode` option using the [mode prefix]({{ site.url }}/cheat_sheet/keywords/#mode-prefix). An example of setting the initial mode to GUI Normal is as follows.

```vim
" write this run commands into .vindrc.
set initmode = gn
  
```


## Change parameters and options

You can change the options and parameters specified using the `set` command in your .vindrc. All options are on the [cheat sheet]({{ site.url }}/cheat_sheet/options/).  

You can also try setting `:set cursor_accel=32` in Command mode, in which case it will be reset after restart and not saved.

<p align="center">
<img src="{{ site.url }}/imgs/cmdline_demo.gif" width=350/>
<p align="center">Try options via virtual command line</p>
</p>

The following is a list of syntaxes.  

|**Syntax**|**Effects**|
|:---|:---|
|`set ${OPTION_NAME}`|Set the value of the option to **true**.|
|`set no${OPTION_NAME}`|Set the value of the option to **false**.|
|`set ${OPTION_NAME} = ${VALUE}`|Set a value of the option. The value can be a string or a number that allows floating points. The string does not need quotation marks, and any character after the non-white character will be handled as the value. White spaces at both ends of the equals sign are ignored.|

&nbsp;

For example, adjusting EasyClick parameters.

```vim
" write in your .vindrc
set easyclick_bgcolor=E67E22
set easyclick_fontcolor=34495E
set easyclick_colordecay=150
set easyclick_fontname=Consolas
set easyclick_fontsize=16
set easyclick_fontweight=600
```


## Change bindings or define macros

win-vind uses **Run Commands** style configuration method. If you've ever written a `.vimrc`, it's easy to make it your win-vind. 

> **Warning**: win-vind v5 has made fundamental changes to the mapping system, please see the [migration guide]({{ site.url }}/migration) from <= 4.4.0.

Generally, there are three levels of key mapping: **key2key**, **keyset2keyset**, and **cmd2cmd**. key2key assigns one key to another. keyset2keyset assigns one key combination to another, such as `<c-s>` to `<m-h>`. cmd2cmd is a mapping scheme that generates another command in response to a sequential command input, such as `qq` to `<c-w>e`.  

The keyset syntax uses the same expression as in Vim, where keys are connected by `-` between `<` and `>`. However, there is no limit to the number of combinations, and you can connect as many as you like. (e.g. `<Esc-b-c-a-d>`).  

The following commands are supported. By the way, `{` and `}` themselves are not part of the syntax.  

|**Syntax**|**Effects**|
|:---|:---|
|`{mode}map {in-key} {out-key}`|It performs **cmd2cmd** mapping with low-level. The defined low-level map is actually propagated to Windows as keystroke. By the way, only the key2keyset format can synchronize the key state without delay.|
|`{mode}noremap {in-cmd} {func-id}`|It defines the map to call the function.|
|`{mode}noremap {in-keyset} {out-keyset}`|It performs **cmd2cmd** mapping in win-vind scope. However, since the `{func-id}` definition has higher priority than its syntax, it may result in exactly one level of recursive mapping.|
|`{mode}unmap {in-cmd}`|Remove the map corresponding to the `{in-cmd}`.|
|`{mode}mapclear`|Delete all maps.|
|`command {in-cmd} {func-id}`|It defines the command to call the function.|
|`delcommand {in-cmd}`|Remove the command corresponding to the `{in-cmd}`.|
|`comclear`|delete all commands.|
|`source`|Load another .vindrc. Either fill in the path to the .vindrc or use the syntax `user/repo` to load the .vindrc in the root directory of the GitHub repository.|

`{mode}` is the [Mode Prefix]({{ site.url }}/cheat_sheet/keywords/#mode-prefix). And only **UTF-8** format is supported for `.vindrc`.  




## Quick Tutorial

In the following, let's try win-vind in practice based on a concrete example.

### 1. GUI Operation and Window Operation  

1. Switch to **GUI Normal Mode** with `<Esc-Left>`.
1. Please inputs `:!mspaint` to launch Microsoft Paint.
1. You can call **EasyClick** with `<shift-f><shift-f>`.  
   
   <p align="center">
   <img src="{{ site.url }}/imgs/EasyClickDemo.gif">
   <p align="center">EasyClick Demo</p>
   </p>
1. Let's select windows with `<C-w>h` or `<C-w>l`.
1. Please select Microsoft Paint and close it with `:close`.

### 2. Customize win-vind  

Let's do the last tutorial!  

1. Go to **Insert Mode**.
1. This time, we will try **Instant GUI Normal Mode** with `<F8>`. It allows us to temporarily switch to the **GUI Normal Mode**.  

   <p align="center">
   <img src="{{ site.url }}/imgs/instant_gui_normal_mode.jpg" width=450 >  
   <p align="center">Instant GUI Normal Demo</p>
   </p>
1. Open your `.vindrc` with `:e`.
   <p align="center">
   <img src="{{ site.url }}/imgs/edit_vindrc_demo.jpg" width=450 >  
   <p align="center">Edit .vindrc Demo</p>
   </p>
1. Write following commands into `.vindrc`.
   ```vim
   set cmd_fontname = Times New Roman
   inoremap <Alt> <easy_click_left>
   imap <ctrl-d><ctrl-d> {win-vind@example.com}
   ngnnoremap <ctrl-1> :!notepad<cr>
   ```
1. If you done, try reloading `.vindrc` with `:source` of win-vind. (No arguments are needed.)
   <p align="center">
   <img src="{{ site.url }}/imgs/source_demo.jpg" width=450 >  
   <p align="center">Reload Demo</p>
   </p>
1. In **Insert Mode**, you can use `<Capslock>` instead of `<Ctrl>` and call EasyClick with a single `<Alt>`. And, you can insert a fixed form text with pressing Ctrl and A at the same time. **In GUI Normal Mode**, pressing Ctrl and 1 at the same time will open Notepad.

<br>
<br>
<br>
<br>
