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
   <img src="{{ site.url }}/imgs/taskbar.jpg" class="img-fluid">
   <p align="center">Like this</p>
   </p>


## How to terminate win-vind
- `:exit` is the recommended termination.
- `<F8> + <F9>` is safe forced termination.

> **Note**: win-vind could **not** operate some windows given high-rank authorization than itself. For example, if you start **Task Manager** having the highest authorization and select its window, you cannot move, click or scroll the mouse cursor by win-vind. If you want to operate all windows, I recommend giving win-vind the administrator authorization. (Please use **Task Scheduler**).


## About the concept mode transition

The basic concept is the same as Vim, but there are two **Normal** mode and two **Visual** mode, and **Resident** mode.   

<p align="center">
<img src="{{ site.url }}/imgs/mode_overview.png" class="img-fluid">
<p align="center">Default mode layer overview</p>
</p>

There are two groups: GUI mode and Editor mode. The former allows us to control windows and mouse cursor, etc. 

<p align="center">
<img src="{{ site.url }}/imgs/GUIandEditor.jpg" class="img-fluid">
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
<img src="{{ site.url }}/imgs/mode_overview_3D.png" class="img-fluid">
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
<img src="{{ site.url }}/imgs/cmdline_demo.gif" class="img-fluid">
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
set easyclick_fontname=Consolas
set easyclick_fontsize=16
set easyclick_fontweight=600
```


## Change bindings or define macros

win-vind uses **Run Commands** style configuration method. If you've ever written a `.vimrc`, it's easy to make it your win-vind. 

> **Warning**: win-vind v5 has made fundamental changes to the mapping system, please see the [migration guide]({{ site.url }}/migration) from <= 4.4.0.

### What is a mapping?
A typical key binding is a simple one that assigns a function from a set of keys. In contrast, the mapping employed in Vim is a kind of extension of key binding, a mechanism that allows recursive key assignment and command definition.

Mapping is defined using the `map` or `noremap` command. Examples of recursive key assignments are shown below.

```vim
" Mapping A
nmap b h
nmap o b
nmap p o
```

```vim
" Mapping B
nmap b h
nmap o h
nmap p h
```

Mapping A is defined as b -> h, o -> b, p -> o. However, the `map` command performs a recursive assignment, o -> b -> h, p -> o -> b -> h, and automatically generates an optimized mapping B. This allows simple commands to be combined to form complex commands efficiently.

On the other hand, the `noremap` command does not perform such recursive resolution. That is, the mapping destination is always interpreted as the default binding, and mapping to a nonexistent binding makes no sense.

For example, the following mapping does not cause anything to happen in insert mode because it is not binding to the m key by default.

```vim
" m does not exist, so nothing happens
inoremap g m
```

Below is a table listing the commands for editing mappings.

|**Syntax**|**Features**|
|:---|:---|
|`${MODE}map ${IN_CMD} ${OUT_CMD}`|Recursively define a map that is invoked by `${IN_CMD}` and generates `${OUT_CMD}`.|
|`${MODE}noremap ${IN_CMD} ${OUT_CMD}`|Non-recursively define a map that is invoked by `${IN_CMD}` and generates `${OUT_CMD}`, which is composed by the default map.|
|`${MODE}unmap ${IN_CMD}`|Remove the map corresponding to the `${IN_CMD}`.|
|`${MODE}mapclear`|Delete all maps.|
|`command ${IN_CMD} ${OUT_CMD}`|It defines the command to call the `${OUT_CMD}`.|
|`delcommand ${IN_CMD}`|Remove the command corresponding to the `{IN_CMD}`.|
|`comclear`|Delete all commands.|

&nbsp;

By the way, `${MODE}` is the [Mode Prefix]({{ site.url }}/cheat_sheet/keywords/#mode-prefix). The keyset syntax uses the same expression as in Vim, where keys are connected by `-` between `<` and `>`. However, there is no limit to the number of combinations, and you can connect as many as you like. (e.g. `<Esc-b-c-a-d>`).  

### Allow external macro

As mentioned earlier, some modes of win-vind absorb keystrokes and do not propagate them to other applications. Therefore, there is a special feature called external macro.  

The external macro is defined macros that are propagated outside of win-vind by enclosing them in `{` and `}`. This emulates the action of the user pressing the keyboard itself. A single key to single key mapping (e.g. `map a {b}`) is the most efficient low-level mapping done.  


### Examples
Below are some examples of use.

1. Define mode change mapping
   ```vim
   imap <win-]> <to_gui_normal>
   ``` 
1. Text input macros
   ```vim
   nmap mail {win-vind@example.com}
   ```
1. Web page launcher
   ```vim
   nmap <ctrl-1> :e https://example.com<cr>
   ```
1. Application launcher
   ```vim
   nmap <ctrl-2> :! notepad<cr>
   ```
1. Copy the current line to the bottom line as in Vim.
   ```vim
   enmap t yyGp
   ```

## Automatic command
win-vind can be configured to automatically execute commands for specific events and target filenames using the `autocmd` command.

### Examples

1. Default mapping for the specific event (match any applications)
   ```vim
   autocmd AppLeave * <to_insert>
   ```
1. Once notepad is selected, it will automatically switch to Editor normal mode.  
   ```vim
   "Equivalent to conventional dedicate_to_window
   autocmd AppEnter *notepad* <to_edi_normal>
   ```
1. Suppress win-vind in processes named Vim.  
   ```vim
   " Equivalent to conventional suppress_for_vim
   autocmd AppEnter,EdiNormalEnter *vim* <to_resident>
   ```

See [\<autocmd_add\>]({{ site.url }}/cheat_sheet/functions/#autocmd_add) or [\<autocmd_del\>]({{ site.url }}/cheat_sheet/functions/#autocmd_del) for more details.

## Load remote .vindrc
Inspired from many Vim plugin managers such as [vim-plug](https://github.com/junegunn/vim-plug), win-vind has a simple remote .vindrc loading capability using the `source` command.

The `source` command is originally designed to load local .vindrc, but it can also load .vindrc in the form `user/repo` from the root directory of a repository on GitHub .

As a sample, by writing the following in your .vindrc., win-vind loads the .vindrc in [pit-ray/remote_vindrc_demo](https://github.com/pit-ray/remote_vindrc_demo) repository, and `:test_remote` command can be available.

```vim
" Load remote repository .vindrc
source pit-ray/remote_vindrc_demo
```

> **Warning**: `source user/repo` does not verify the safety of the .vindrc it reads, which may be a security hole. Therefore, use it for reliable repositories or your own dotfiles configurations. As a minimum security measure, win-vind only reads the contents of source the first time a `source` command is done, and does not update the contents as `git pull` does.


## Use as automation command by server-client

The `--command` option allows us to execute an arbitrary command from a terminal.

The first time you run win-vind, it will be resident in the system tray and run as a server program. The server will check shared memory at the interval specified by the `listen_interval` option and execute any requested commands in addition to the query. All new win-vinds that already have win-vind running will become clients.

For example, if you execute the following command with win-vind already running, a window-switching program will be started.

```sh
$ win-vind -c "<switch_window>"
```

By using this, only specific functions such as EasyClick can be called from other tools such as AutoHotKey.

## Quick Tutorial

In the following, let's try win-vind in practice based on a concrete example.

### 1. GUI Operation and Window Operation  

1. Switch to **GUI Normal Mode** with `<ctrl-]>`.
1. Please inputs `:!mspaint` to launch Microsoft Paint.
1. You can call **EasyClick** with `<shift-f><shift-f>`.  
   
   <p align="center">
   <img src="{{ site.url }}/imgs/EasyClickDemo.gif" class="img-fluid">
   <p align="center">EasyClick Demo</p>
   </p>
1. Let's select windows with `<C-w>h` or `<C-w>l`.
1. Please select Microsoft Paint and close it with `:close`.

### 2. Customize win-vind  

Let's do the last tutorial!  

1. Go to **Insert Mode**.
1. This time, we will try **Instant GUI Normal Mode** with `<F8>`. It allows us to temporarily switch to the **GUI Normal Mode**.  
   
   <p align="center">
   <img src="{{ site.url }}/imgs/instant_gui_normal_mode.jpg" class="img-fluid">
   <p align="center">Instant GUI Normal Demo</p>
   </p>
1. Open your `.vindrc` with `:e`.
   
   <p align="center">
   <img src="{{ site.url }}/imgs/edit_vindrc_demo.jpg" class="img-fluid">
   <p align="center">Edit .vindrc Demo</p>
   </p>
1. Write following commands into `.vindrc`.
   
   ```vim
   " Write to your .vindrc.
   set cmd_fontname = Arial
   inoremap <Alt> <easy_click_left>
   imap <ctrl-d><ctrl-d> {win-vind@example.com}
   noremap <ctrl-1> :!notepad<cr>
   ```
1. If you done, try reloading `.vindrc` with `:source` of win-vind. (No arguments are needed.)
   
   <p align="center">
   <img src="{{ site.url }}/imgs/source_demo.jpg" class="img-fluid">
   <p align="center">Reload Demo</p>
   </p>
1. The above command allows you to call EasyClick with the `<alt>` key and `<ctrl-d><ctrl-d>` will auto-type your email address in insert mode. Also, `<ctrl-1>` will launch Notepad in normal mode.

<br>
<br>
<br>
<br>
