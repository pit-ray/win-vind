<p align="center"><font size="100"><b>win-vind</b></font></p>    

<p align="center">
  <img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/win-vind-icon.png?raw=true" width="200" height="200" />
  <p align="center"><b>Simple Vim Key Binder for Windows</b></p>  
</p>  

<p align="center">   
  <a href="https://ci.appveyor.com/project/pit-ray/win-vind"><img src="https://ci.appveyor.com/api/projects/status/qoggl832qqbic5f5?svg=true" /></a>  
  <a href="https://scan.coverity.com/projects/pit-ray-win-vind"><img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects/22417/badge.svg"/></a>  
  <a href="https://www.codacy.com/gh/pit-ray/win-vind/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=pit-ray/win-vind&amp;utm_campaign=Badge_Grade"><img src="https://app.codacy.com/project/badge/Grade/8f2e6f2826904efd82019f5888574327" /></a>
  <br>  
  <a href="https://github.com/pit-ray/win-vind/blob/master/LICENSE.txt" target="_blank"><img src="http://img.shields.io/badge/license-MIT-blue.svg?style=flat" /></a> 
  <a href="https://pit-ray.github.io/win-vind/" target="_blank"><img src="https://img.shields.io/badge/docs-doxygen-blue.svg" /></a> 
  <a href="https://github.com/pit-ray/win-vind/releases/latest"><img src="https://img.shields.io/github/v/release/pit-ray/win-vind" /></a>
  <a href="https://pit-ray.github.io/win-vind/downloads/" ><img src="https://img.shields.io/github/downloads/pit-ray/win-vind/total" /></a>
  <img alt="Lines of code" src="https://img.shields.io/tokei/lines/github/pit-ray/win-vind">
<br>  

## Description
**win-vind** provides the lightweight **Hybrid UI System of CUI and GUI**. Mainly, there are five point features.  

### 1. GUI Operation like Vim  
You can operate all windows, the mouse, virtual desktops, GUI-objects with key-bindings like Vim.  

**Example**  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/EasyClickDemo.gif?raw=true" title="EasyClick-demo" width=700>  

It works well in UWP applications.  
  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/on_uwp.jpg?raw=true" >

### 2. Vim emulation everywhere  
It enables to emulate Vim when using general text editors, for instance, `notepad.exe` or `Microsoft Office Word`.  

**Example**  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/edi-mode-demo.gif?raw=true" />

### 3. Mode Management like Vim  
There are two main modes: **GUI Mode** for GUI operation and **Editor Mode** for Vim emulation.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/mode_overview_2.jpg?raw=true" width=700 />  

### 4. Process Control  
If you register `notepad.exe` as `notepad`, could launch it by typing `:!notepad` like below.  

**Example**  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/cmd-demo.gif?raw=true" width=400 title="cmd-demo">  

### 5. Using from command line
You can operate win-vind from command lines.  

**Example**  
```bash
$ ./win-vind.exe --func change_to_normal
```
Therefore, it enables to make Vim link to GUI. Refer to <a href="https://pit-ray.github.io/win-vind/cheat_sheet/link_with_vim/">Link With Vim - win-vind</a>  

If you want to know the details, refer to <a href="https://pit-ray.github.io/win-vind/cheat_sheet/">Cheat Sheet - win-vind</a>.  

## How to use
Please visit <a href="https://pit-ray.github.io/win-vind/how_to_use/">How to use - win-vind</a>.  

### Default Key Config  
#### Window Control  

|Binding|Command|Meanings|
|:---:|:---:|:---:|
|`<C-w>s`|`:sw`|Switch Window|
|`<C-w>q`|`:cl`|Close Current Window|
|`<C-w>h`|`:lsplit`|Snap Current Window to Left|
|`<C-w>l`|`:rsplit`|Snap Current Window to Right|

#### Mouse Control  

|Binding|Command|Meanings|
|:---:|:---:|:---:|
|`o`|-|Left Click|
|`a`|-|Right Click|
|`F`|`:ec`|<a href="https://pit-ray.github.io/win-vind/cheat_sheet/mouse_ctrl/#easyclick">EasyClick</a> (hinting feature)|
|`h`|-|Move Cursor Left|
|`j`|-|Move Cursor Down|
|`k`|-|Move Cursor Up|
|`l`|-|Move Cursor Right|
|`0`|-|Jump Cursor to Left|
|`f`|-|<a href="https://pit-ray.github.io/win-vind/cheat_sheet/mouse_ctrl/#jump-cursor-by-keyboard-mapping">Jump Cursor by Keyboard Mapping</a>|
|`<C-y>`|-|Scroll Up|
|`<C-e>`|-|Scroll Down|

#### Virtual Desktop Control

|Binding|Command|Meanings|
|:---:|:---:|:---:|
|`<C-v>h`|-|Switch Virtual Desktop to Left|
|`<C-v>l`|-|Switch Virtual Desktop to Right|

#### Mode Control  

|Binding|Command|Meanings|
|:---:|:---:|:---:|
|`<Esc-Left>`|-|Change to GUI Normal|
|`i`|-|Change to GUI Insert|
|`v`|-|Change to GUI Visual|
|`I`|-|Change to Editor Normal|
|`i`|-|Change to Editor Insert|
|`v`|-|Change to Editor Visual|
|`V`|-|Change to Editor Line Visual|

#### Process Launcher

|Binding|Command|Meanings|
|:---:|:---:|:---:|
|-|`:sh`, `:term`|Start Command Prompt|
|-|`:!<any>`|Start Any Application|


#### Utilities

|Binding|Command|Meanings|
|:---:|:---:|:---:|
|-|`:w`|Save Opened File|
|-|`:mkdir <any>`|Make Directories|
|`<gt>`|-|Move to Next Page|
|`<lt>`|-|Move to Previous Page|
|`u`|-|Undo|
|`<C-r>`|-|Redo|
|`y`|-|Copy|
|`p`|-|Paste|
|`dd`|-|Cut|
|`x`|-|Delete|

#### Others 

|Binding|Command|Meanings|
|:---:|:---:|:---:|
|-|`:set`|Show Config Window of win-vind|
|-|`:exit`|Exit win-vind|  

If you want more details, please see the <a href="https://pit-ray.github.io/win-vind/cheat_sheet/">CheatSheet</a>.  

## How to contribute
Please visit <a href="https://pit-ray.github.io/win-vind/how_to_contribute/">How to contribute - win-vind</a>.  

<!--
## Latest Log
It has differences from the latest release.
`release` is the so-called stable version.

### Fix

### Change

### New
#### General

#### Options

#### Functions

### Bugs

## ToDo

-->  

## License  
This software is provided by **MIT License**.  

## Author
- pit-ray  
[E-mail] pit-ray(at)outlook.com
