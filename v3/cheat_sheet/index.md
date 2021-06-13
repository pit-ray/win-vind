---
layout: default
title: Cheat Sheet
nav_order: 5
descripption: "Command lists of win-vind."
parent: v3
has_children: true
---


## Default Key Config  

### Window Control  

#### Select Window

|Binding|Command|Feature|
|:---:|:---:|:---|
|`<C-w>s`|`:switch`|Switch windows|
|`<C-w>h`|-|Select a left window|
|`<C-w>l`|-|Select a right window|
|`<C-w>k`|-|Select an upper window|
|`<C-w>j`|-|Select a lower window|

#### Resize Window

|Binding|Command|Feature|
|:---:|:---:|:---|
|`<C-w>u`|`:max`|Maximize a current window|
|`<C-w>d`|`:min`|Minimize a current window|
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


#### Open Window

|Binding|Command|Feature|
|:---:|:---:|:---|
|`<C-w>n`|`:new`|Open a new window|
|-|`:split`|Open a new window with hsplit|
|-|`:vsplit`|Open a new window with vsplit|
|-|`:reload`|Reload a current window|


#### Tab

|Binding|Command|Feature|
|:---:|:---:|:---|
|-|`:tabnew`|Open a new tab|
|`gT`|`:tabprevious`|Switch to a left tab|
|`gt`|`:tabnext`|Switch to a right tab|
|-|`:close`|Close a current window|


### Mouse Control
  
#### Click 

|Binding|Command|Feature|
|:---:|:---:|:---|
|`o`|-|Left Click|
|`a`|-|Right Click|
|`V`|-|Select all|
|`FF`, `Fo`|`:ecl`|<a href="https://pit-ray.github.io/win-vind/cheat_sheet/mouse_ctrl/#easyclick">EasyClick (Left Click)</a> (hinting feature)|
|`Fa`|`:ecr`|EasyClick (Right Click)|
|`Fm`|`:ecm`|EasyClick (Mid Click)|
|`Fh`|`:ech`|EasyClick (Hovering)|

#### Move

|Binding|Command|Feature|
|:---:|:---:|:---|
|`h`|-|Move cursor left|
|`j`|-|Move cursor down|
|`k`|-|Move cursor up|
|`l`|-|Move cursor right|

#### Jump

|Binding|Command|Feature|
|:---:|:---:|:---|
|`0`|-|Jump cursor to left|
|`$`|-|Jump cursor to right|
|`gg`|-|Jump cursor to top|
|`G`|-|Jump cursor to bottom|
|`gm`|-|Jump to horizontal center|
|`M`|-|Jump to vertical center|
|`f`|-|<a href="https://pit-ray.github.io/win-vind/cheat_sheet/mouse_ctrl/#jump-cursor-by-keyboard-mapping">Jump Cursor by Keyboard Mapping</a>|
|`t`|-|Jump to active window|

#### Scroll

|Binding|Command|Feature|
|:---:|:---:|:---|
|`<C-y>`|-|Scroll up|
|`<C-e>`|-|Scroll down|
|`<C-u>`|-|Scroll middle page up|
|`<C-d>`|-|Scroll middle page down|
|`<C-b>`|-|Scroll page up|
|`<C-f>`|-|Scroll page down|
|`zh`|-|Scroll left|
|`zl`|-|Scroll right|
|`zH`|-|Scroll middle page left|
|`zL`|-|Scroll middle page right|

### Virtual Desktop Control  
  
|Binding|Command|Feature|
|:---:|:---:|:---|
|`<C-v>h`|-|Switch virtual desktops to left|
|`<C-v>l`|-|Switch virtual desktops to right|
|`<C-v>c`|-|Create a new virtual desktop|
|`<C-v>s`|-|Task View|

### Mode Control

|Binding|Command|Feature|
|:---:|:---:|:---|
|`<Esc-Left>`, `<Esc-Right>`|-|GUI Normal|
|`i`|-|GUI Insert|
|`v`|-|GUI Visual|
|`I`|-|Editor Mode|
|`:`|-|Command Mode|

### Process Launcher
  
|Binding|Command|Feature|
|:---:|:---:|:---|
|-|`:sh`, `:term`|Start Command Prompt|
|-|`:!<any>`|Start Any Application|
|-|`:ex`|Open Explorer|
|`<Win>`|`:start`|Open StartMenu|

### Utilities

|Binding|Command|Feature|
|:---:|:---:|:---|
|-|`:w`|Save opened file|
|-|`:open`|Open other file|
|-|`:mkdir <any>`|Make Directories|
|`/`, `?`|-|Search pattern|
|`<gt>`|-|Move to Next Page|
|`<lt>`|-|Move to Previous Page|
|`u`|-|Undo|
|`<C-r>`|-|Redo|
|`y`|-|Copy|
|`p`|-|Paste|
|`dd`|-|Cut|
|`x`|-|Delete|
|`X`|-|BackSpace|

### Others
  
|Binding|Command|Feature|
|:---:|:---:|:---|
|`<LAlt><LAlt>`|`:enable d2w`|Enable targeting for Dedicate to One Window option|
|`<RAlt><RAlt>`|`:disable d2w`|Disiable targeting for Dedicate to One Window option|
|-|`:set`|Show Config Window of win-vind|
|-|`:exit`|Exit win-vind|  

### Editor Mode (Vim Emulation)

#### Mode

|Binding|Command|Feature|
|:---:|:---:|:---|
|`<esc>`|-|Normal|
|`i`|-|Insert|
|`I`|-|BOL Insert|
|`a`|-|Back Insert|
|`o`|-|Insert Below|
|`O`|-|Insert Above|
|`v`|-|Visual|
|`V`|-|Line Visual|

#### Move

|Binding|Command|Feature|
|:---:|:---:|:---|
|`h`|-|Move caret left|
|`l`|-|Move caret right|
|`k`|-|Move caret up|
|`j`|-|Move caret down|
|`w`|-|move caret words forward|
|`b`|-|move caret words backward|
|`W`|-|move caret WORDS forward|
|`B`|-|move caret WORDS backward|


#### Jump

|Binding|Command|Feature|
|:---:|:---:|:---|
|`0`|-|Jump caret to BOL|
|`$`|-|Jump caret to EOL|
|`gg`|`:<num>`|Jump caret to N-Line (Default: BOF)|
|`G`|-|Jump caret to N-Line (Default: EOF)|


#### Yank

|Binding|Command|Feature|
|:---:|:---:|:---|
|`y`|-|Copy highlight text|
|`yy`|-|Copy lines|
|`y{Motion}`|-|Copy with motion|


#### Put

|Binding|Command|Feature|
|:---:|:---:|:---|
|`p`|-|Paste after|
|`P`|-|Paste before|

#### Delete

|Binding|Command|Feature|
|:---:|:---:|:---|
|`d`|-|Delete highlight text|
|`dd`|-|Delete Lines|
|`D`|-|Delete line until EOL|
|`x`|-|Delete after|
|`X`|-|Delete before|
|`d{Motion}`|-|Delete with motion|
|`c{Motion}`|-|Delete with motion and insert|
|`cc`, `S`|-|Delete lines and insert|
|`s`|Delete chars and insert|
|`C`|Delete line until EOL and insert|

#### Replace

|Binding|Command|Feature|
|:---:|:---:|:---|
|`r`|-|Replace chars|
|`R`|-|Replace sequences|
|`~`|-|Switch case of character|


#### Layout

|Binding|Command|Feature|
|:---:|:---:|:---|
|`J`|-|Remove EOL|
