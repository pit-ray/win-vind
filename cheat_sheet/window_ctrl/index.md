---
layout: default
title: Window Control
nav_order: 5
parent: Cheat Sheet
descripption: "Window Control"
---

# Window Control  
{: .no_toc}  

<hr>

1. TOC1  
{:toc}

<hr>  

## Window  

### Switch Window  

|Details|Status|  
|:---|:---|  
|ID|**switch_window**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`&lt;C-w&gt;s`|  
|Default Command|`:sw`, `:switch`|  
  
It is same as **Alt + Tab** of shortcuts on Windows.  You can select window by keybindings of **move_left** or **move_right**. In addition, the keys for deciding a window are **ESC** or **Enter**.  

<br>  

### Maximize Current Window  
|Details|Status|
|:---|:---|
|ID|**maximize_current_window**|
|N-Repeat|<span class="yes">Supprted</span>|
|Default Key|`&lt;C-w&gt;k`, `&lt;C-w&gt;a`|
|Default Command|`:max`, `:on`, `:only`|

Is is same as **Win + Up** of shortcuts on Windows.  

<br>  

### Minimize Current Window  

|Details|Status|
|:---|:---|
|ID|minimize_current_window|
|N-Repeat|Supprted|
|Default Key|&lt;C-w&gt;j, &lt;C-w&gt;i|
|Default Command|:min, :hi, :hide|

Is is same as **Win + Down** of shortcuts on Windows.  

<br>  

### Snap Current Window to Left  

|Details|Status|
|:---|:---|
|ID|snap_current_window_to_left|
|N-Repeat|Not Supported|
|Default Key|&lt;C-w&gt;h, &lt;C-w&gt;H, &lt;C-w&gt;&lt;lt&gt;, &lt;C-w&gt;&lt;Left&gt;|
|Default Command|:lsplit, :lsp, :wl|

Is is same as **Win + Left** of shortcuts on Windows.  

<br>  

### Snap Current Window To Right  

|Details|Status|
|:---|:---|
|ID|snap_current_window_to_right|
|N-Repeat|Not Supported|
|Default Key|&lt;C-w&gt;l, &lt;C-w&gt;L, &lt;C-w&gt;&lt;gt&gt;, &lt;C-w&gt;&lt;Right&gt;|
|Default Command|:rsplit, :rsp, :wh|

Is is same as **Win + Right** of shortcuts on Windows.  

<br>  

### Open New Current Window  

|Details|Status|
|:---|:---|
|ID|open_new_current_window|
|N-Repeat|Not Supported|
|Default Key|&lt;C-w&gt;n|
|Default Command|:new|

It searchs the path of a executable file from the selected window and executes it.  If a authorization of the window is higher than itself, could not start the application.  

<br>  

### Close Current Window  

|Details|Status|
|:---|:---|
|ID|close_current_window|
|N-Repeat|Not Supported|
|Default Key|&lt;C-w&gt;q, &lt;C-w&gt;c|
|Default Command|:cl, :close|

Is is same as **Alt + F4** of shortcuts on Windows.  

<br>  

### Reload Current Window  

|Details|Status|
|:---|:---|
|ID|reload_current_window|
|N-Repeat|Not Supported|
|Default Key|-|
|Default Command|:reload|

Is is same as **F5** of shortcuts on Windows. 

<br>
<hr>  

## Tab  

### Switch to Left Tab  

|Details|Status|
|:---|:---|
|ID|switch_to_right_tab|
|N-Repeat|Supported|
|Default Key|gT|
|Default Command|:tabprevious|

Is is same as **Ctrl + Shift + Tab** of shortcuts on Windows. 

<br>  

### Switch to Right Tab  

|Details|Status|
|:---|:---|
|ID|switch_to_right_tab|
|N-Repeat|Supported|
|Default Key|gt|
|Default Command|:tabnext|

Is is same as **Ctrl + Tab** of shortcuts on Windows. 

<br>  

### Open New Tab  

|Details|Status|
|:---|:---|
|ID|open_new_tab|
|N-Repeat|Not Supported|
|Default Key|-|
|Default Command|:tabnew|

Is is same as **Ctrl + T** of shortcuts on Windows. 
