---
layout: default
title: Window Control
nav_order: 1
parent: Cheat Sheet
descripption: "Window Control"
---

# Window Control  
{: .no_toc}  

<hr>

1. TOC1  
{:toc}

<hr>  

## Select Window  

### Switch a window  

|Details|Status|  
|:---|:---|  
|ID|**switch_window**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`<C-w>s`|  
|Default Command|`:sw`, `:switch`|  
  
It is the same as **Alt + Tab** of shortcuts on Windows.  You can select window by keybindings of **move_left** or **move_right**. In addition, the keys for deciding a window are **ESC** or **Enter**.  

<br>  

### Select a left window  

|Details|Status|  
|:---|:---|  
|ID|**select_left_window**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`<C-w>h`|  
|Default Command|-|  

It selects the nearest window from the left-center of a foreground window. 

<br>  

### Select a right window  

|Details|Status|  
|:---|:---|  
|ID|**select_right_window**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`<C-w>l`|  
|Default Command|-|  

It selects the nearest window from the right-center of a foreground window. 

<br>  

### Select an upper window  

|Details|Status|  
|:---|:---|  
|ID|**select_upper_window**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`<C-w>k`|  
|Default Command|-|  

It selects the nearest window from the upper-center of a foreground window. 

<br>  

### Select a lower window  

|Details|Status|  
|:---|:---|  
|ID|**select_lower_window**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`<C-w>j`|  
|Default Command|-|  

It selects the nearest window from the lower-center of a foreground window. 

<br>  


## Resize Window 

### Maximize Current Window  

|Details|Status|
|:---|:---|
|ID|**maximize_current_window**|
|N-Repeat|<span class="yes">Supprted</span>|
|Default Key|`<C-w>u`|
|Default Command|`:max`, `:on`, `:only`|

Is is the same as **Win + Up** of shortcuts on Windows.  

<br>  

### Minimize a current window  

|Details|Status|
|:---|:---|
|ID|**minimize_current_window**|
|N-Repeat|<span class="yes">Supprted</span>|
|Default Key|`<C-w>d`|  
|Default Command|`:min`, `:hi`, `:hide`|

Is is the same as **Win + Down** of shortcuts on Windows.  

<br>  

### Snap a current window to left  

|Details|Status|
|:---|:---|
|ID|**snap_current_window_to_left**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|`<C-w>H`, `<C-w><Left>`|
|Default Command|`:lsplit`, `:lsp`, `:wl`|

It moves and resizes the selected window to the left half of its monitor.  

<br>  

### Snap a current window to right  

|Details|Status|
|:---|:---|
|ID|**snap_current_window_to_right**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|`<C-w>L`, `<C-w><Right>`|
|Default Command|`:rsplit`, `:rsp`, `:wh`|

It moves and resizes the selected window to the right half of its monitor.  

<br>  

### Snap a current window to top  

|Details|Status|
|:---|:---|
|ID|**snap_current_window_to_top**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|`<C-w>K`|
|Default Command|`:tsplit`, `:tsp`, `:wt`|

It moves and resizes the selected window to the top half of its monitor.  

<br>  

### Snap a current window to bottom   

|Details|Status|
|:---|:---|
|ID|**snap_current_window_to_bottom**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|`<C-w>J`|
|Default Command|`:bsplit`, `:bsp`, `:wb`|

It moves and resizes the selected window to the bottom half of its monitor.  

<br>  


### Arrange all windows  

|Details|Status|
|:---|:---|
|ID|**arrange_windows**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|`<C-w>=`|
|Default Command|`:arrange`, `:arw`|  

It aligns all windows within each monitor based on their memory usage.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/arrange_window_min.gif?raw=true" width=600 />  


<br>  

### Rotate arranged windows in a current monitor  

|Details|Status|
|:---|:---|
|ID|**rotate_windows**|
|N-Repeat|<span class="yes">Supported</span>|
|Default Key|`<C-w>r`|
|Default Command|`:rotate`, `:rot`|

It rotates arranged windows counter-clockwise.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/rotate_min.gif?raw=true" width=600 />  

<br>  

### Rotate arranged windows in a current monitor in reverse

|Details|Status|
|:---|:---|
|ID|**rotate_windows_in_reverse**|
|N-Repeat|<span class="yes">Supported</span>|
|Default Key|`<C-w>R`|
|Default Command|`:rerotate`, `rerot`|

It rotates arranged windows clockwise.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/rotate_min_reverse.gif?raw=true" width=600 />  

<br>  

### Exchange a window with the most nearest one

|Details|Status|
|:---|:---|
|ID|**exchange_window_with_next_one**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|`<C-w>x`|
|Default Command|`:exchange`, `:exw`|  

It exchanges a foreground window with the nearest one.  

<br>  

### Resize the height of a window  

|Details|Status|
|:---|:---|
|ID|**resize_window_height**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|-|
|Default Command|`:resize <num>`, `:res <num>`|

It changes the height of a foreground window based on the command.  

<br>  

### Increase the height of a window  

|Details|Status|
|:---|:---|
|ID|**increase_window_height**|
|N-Repeat|<span class="yes">Supported</span>|
|Default Key|`<C-w>+`|
|Default Command|`:resize +<num>`, `:res +<num>`|

It increases the height of a foreground window.  

<br>  

### Decrease the height of a window  

|Details|Status|
|:---|:---|
|ID|**decrease_window_height**|
|N-Repeat|<span class="yes">Supported</span>|
|Default Key|`<C-w>-`|
|Default Command|`:resize -<num>`, `:res -<num>`|

It reduces the height of a foreground window.  

<br>  

### Resize the width of a window  

|Details|Status|
|:---|:---|
|ID|**resize_window_width**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|-|
|Default Command|`:vertical resize <num>`, `:vert res <num>`|

It changes the width of a foreground window based on the command.  

<br>  

### Increase the width of a window  

|Details|Status|
|:---|:---|
|ID|**increase_window_width**|
|N-Repeat|<span class="yes">Supported</span>|
|Default Key|`<C-w><gt>`|
|Default Command|`:vertical resize +<num>`, `:vert res +<num>`|

It increases the width of a foreground window.  

<br>  

### Decrease the width of a window  

|Details|Status|
|:---|:---|
|ID|**decrease_window_width**| 
|N-Repeat|<span class="yes">Supported</span>|
|Default Key|`<C-w><lt>`|
|Default Command|`:vertical resize -<num>`, `:vert res -<num>`|

It reduces the width of a foreground window.  

<br>  


## Open Window  

### Open a new current window  

|Details|Status|
|:---|:---|
|ID|**open_new_current_window**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|`<C-w>n`|
|Default Command|`:new`|

It searchs the path of a executable file from the selected window and executes it.  If a authorization of the window is higher than itself, could not start the application.  

<br>  

### Open a new current window with horizontal split 

|Details|Status|
|:---|:---|
|ID|**open_new_current_window_with_hsplit**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|-|
|Default Command|`:sp`, `:split`|

It changes the height of the foreground window to half and calls <a href="https://pit-ray.github.io/win-vind/cheat_sheet/window_ctrl/open-a-new-current-window">Open a new current window</a> in below direction.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/horizontal_split_min.gif?raw=true" width=600 />  

<br>  

### Open a new current window with vertical split

|Details|Status|
|:---|:---|
|ID|**open_new_current_window_with_vsplit**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|-|
|Default Command|`:vs`, `:vsplit`|

It changes the width of the foreground window to half and calls <a href="https://pit-ray.github.io/win-vind/cheat_sheet/window_ctrl/open-a-new-current-window">Open a new current window</a> in right direction.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/vertical_split_min.gif?raw=true" width=600 />  

<br>  

### Close a current window  

|Details|Status|
|:---|:---|
|ID|**close_current_window**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|`<C-w>q`, `<C-w>c`|
|Default Command|`:cl`, `:close`|

Is is the same as **Alt + F4** of shortcuts on Windows.  

<br>  

### Reload a current window  

|Details|Status|
|:---|:---|
|ID|**reload_current_window**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|-|
|Default Command|`:reload`|

Is is the same as **F5** of shortcuts on Windows. 

<br>
<hr>  

## Tab  

### Switch to Left Tab  

|Details|Status|
|:---|:---|
|ID|**switch_to_right_tab**|
|N-Repeat|<span class="yes">Supported</span>|
|Default Key|`gT`|
|Default Command|`:tabprevious`|

Is is the same as **Ctrl + Shift + Tab** of shortcuts on Windows. 

<br>  

### Switch to Right Tab  

|Details|Status|
|:---|:---|
|ID|**switch_to_right_tab**|
|N-Repeat|<span class="yes">Supported</span>|
|Default Key|`gt`|
|Default Command|`:tabnext`|

Is is the same as **Ctrl + Tab** of shortcuts on Windows. 

<br>  

### Open New Tab  

|Details|Status|
|:---|:---|
|ID|**open_new_tab**|
|N-Repeat|<span class="no">Not Supported</span>|
|Default Key|-|
|Default Command|`:tabnew`|

Is is the same as **Ctrl + T** of shortcuts on Windows. 


### Close a current tab  

|Details|Status|  
|:---|:---|  
|ID|**close_current_tab**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|-|  
|Default Command|`:q`, `:qa`, `:q!`, `:qa!`, `:tabclose`| 

It is the same as **Ctrl + F4** of shortcuts on Windows.  

<br>
