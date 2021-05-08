---
layout: default
title: Cheat Sheet
nav_order: 5
descripption: "Command lists of win-vind."
has_children: true
---


## Default Key Config  

**Window Control**  

|Binding|Command|Meanings|
|:---:|:---:|:---|
|`<C-w>s`|`:switch`|Switch windows|
|`<C-w>q`|`:close`|Close a current window|
|`<C-w>n`|`:new`|Open a new window|
|-|`:split`|Open a new window with hsplit|
|-|`:vsplit`|Open a new window with vsplit|
|`<C-w>h`|-|Select a left window|
|`<C-w>l`|-|Select a right window|
|`<C-w>k`|-|Select an upper window|
|`<C-w>j`|-|Select a lower window|
|`<C-w>H`|`:lsplit`|Snap a current window to left|
|`<C-w>L`|`:rsplit`|Snap a current window to right|
|`<C-w>K`|`:tsplit`|Snap a current window to top|
|`<C-w>J`|`:bsplit`|Snap a current window to bottom|
|`<C-w>=`|`:arrange`|Arrange windows|
|`<C-w>r`|`:rotate`|Rotate windows|
|`<C-w>R`|`:rerotate`|Rotate windows in reverse|
|`<C-w>x`|`:exchange`|Exchange a window with nearest one|
|-|`:resize <num>`|Resize the height of a window|
|`<C-w>+`|`:resize +<num>`|Increase the height of a window|
|`<C-w>-`|`:resize -<num>`|Decrease the height of a window|
|-|`:vertical resize <num>`|Resize the width of a window|
|`<C-w><gt>`|`:vertical resize +<num>`|Increase the width of a window|
|`<C-w><lt>`|`:vertical resize -<num>`|Decrease the width of a window|


**Mouse Control**  
  
|Binding|Command|Meanings|
|:---:|:---:|:---|
|`o`|-|Left Click|
|`a`|-|Right Click|
|`FF`, `Fo`|`:ecl`|<a href="https://pit-ray.github.io/win-vind/cheat_sheet/mouse_ctrl/#easyclick">EasyClick (Left Click)</a> (hinting feature)|
|`Fa`|`:ecr`|EasyClick (Right Click)|
|`Fm`|`:ecm`|EasyClick (Mid Click)|
|`Fh`|`:ech`|EasyClick (Hovering)|
|`h`|-|Move Cursor Left|
|`j`|-|Move Cursor Down|
|`k`|-|Move Cursor Up|
|`l`|-|Move Cursor Right|
|`0`|-|Jump Cursor to Left|
|`f`|-|<a href="https://pit-ray.github.io/win-vind/cheat_sheet/mouse_ctrl/#jump-cursor-by-keyboard-mapping">Jump Cursor by Keyboard Mapping</a>|
|`<C-y>`|-|Scroll Up|
|`<C-e>`|-|Scroll Down|j

**Virtual Desktop Control**  
  
|Binding|Command|Meanings|
|:---:|:---:|:---|
|`<C-v>h`|-|Switch Virtual Desktop to Left|
|`<C-v>l`|-|Switch Virtual Desktop to Right|

**Mode Control**  

|Binding|Command|Meanings|
|:---:|:---:|:---|
|`<Esc-Left>`|-|Change to GUI Normal|
|`i`|-|Change to GUI Insert|
|`v`|-|Change to GUI Visual|
|`I`|-|Change to Editor Normal|
|`i`|-|Change to Editor Insert|
|`v`|-|Change to Editor Visual|
|`V`|-|Change to Editor Line Visual|

**Process Launcher**   
  
|Binding|Command|Meanings|
|:---:|:---:|:---|
|-|`:sh`, `:term`|Start Command Prompt|
|-|`:!<any>`|Start Any Application|

**Utilities**  
  
|Binding|Command|Meanings|
|:---:|:---:|:---|
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

**Others**    
  
|Binding|Command|Meanings|
|:---:|:---:|:---|
|-|`:set`|Show Config Window of win-vind|
|-|`:exit`|Exit win-vind|  
