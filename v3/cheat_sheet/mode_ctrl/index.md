---
layout: default
title: Mode Control
nav_order: 4
parent: Cheat Sheet for v3
descripption: "Mode Control"
---

# Mode Control  
{: .no_toc}

<hr>

1. TOC  
{:toc}  

<hr>  

All modes on win-vind consist of two-layer architectures which for controlling GUI and general editors. However, its "two layers" are only in default case the design is changeable on your needs by rewriting **bindings.json**. All modes exist as the same level classes in internal implementations. 

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/mode_overview_2.jpg?raw=true" />

## GUI Mode  

There are for controlling GUI objects, for example, the mouse, windows, some GUI components. It has three modes of **Normal**, **Insert**, and **Visual**. In other words, they are a mode absorbing inputs, a mode which the same as default Windows, a mode long-pressing a left button of the mouse.  

### Change to Normal  
  
|Details|Status|  
|:---|:---|  
|ID|**change_to_normal**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`<Esc-Left>`, `<Esc-Right>`, `<Esc-Up>`, `<Esc-Down>`, `<F9>`|  
|Default Command|-| 

It changes the mode to **GUI Normal Mode**. When calling it, it makes all states of keys release and refreshes once. Therefore if win-vind is out of control by some bugs, you can reset all states. Under **GUI Normal Mode**, your all inputs are ignored before passing all processes having lower authorizations.  
<br>  

### Change to Insert  

|Details|Status|  
|:---|:---|  
|ID|**change_to_insert**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`i`|  
|Default Command|-| 

It changes the mode to **GUI Insert Mode** and goes back to the normal Windows before applying win-vind. If set it up as the initial mode of win-vind, you can virtually start win-vind by inputting some binded keys.  

<br>  


### Change to Visual  

|Details|Status|  
|:---|:---|  
|ID|**change_to_visual**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`v`|  
|Default Command|-| 

It changes the mode to **GUI Visual Mode**. A left button of a mouse is kept pressing under **GUI Visual Mode**, but if you send releasing events of left button by pressing a the left button of a mouse, this is the same mode as absorbing inputed keys.  


<br>  

<hr>  


## Editor Mode  
### Change to Editor Normal  

|Details|Status|  
|:---|:---|  
|ID|**change_to_edi_normal**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`I`, `gI`|  
|Default Command|-| 

It changes the vim emulation mode. Under Editor Mode, it is the same as **Esc** of the original Vim.  

<br>  

### Change to Editor Insert  

|Details|Status|  
|:---|:---|  
|ID|**change_to_edi_insert**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`i`|  
|Default Command|-| 

It is the same as **i** on the original Vim.  

<br>  

### Change to Editor BOL Insert  

|Details|Status|  
|:---|:---|  
|ID|**change_to_edi_BOLinsert**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`I`, `gI`|  
|Default Command|-| 

It is the same as **I** on the original Vim.  

<br>  

### Change to Editor EOL Insert  

|Details|Status|  
|:---|:---|  
|ID|**change_to_edi_EOLinsert**|
N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`A`|  
|Default Command|-| 

It is the same as **A** on the original Vim.  

<br>  

### Change to Editor Back Insert  

|Details|Status|  
|:---|:---|  
|ID|**change_to_bkinsert**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`a`|  
|Default Command|-| 

It is the same as **a** on the original Vim.  

<br>  

### Change to Editor Insert Below  

|Details|Status|  
|:---|:---|  
|ID|**change_to_edi_nlinsert_below**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`o`|  
|Default Command|-| 

It is the same as **o** on the original Vim.  

<br>  

### Change to Editor Insert Above  

|Details|Status|  
|:---|:---|  
|ID|**change_to_edi_nlinsert_above**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`O`|  
|Default Command|-| 

It is the same as **O** on the original Vim.  

<br>  

### Change to Editor Visual  

|Details|Status|  
|:---|:---|  
|ID|**change_to_edi_visual**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`v`, `<C-v>`|  
|Default Command|-| 

It is the same as **v** on the original Vim.  

<br>  

### Change to Editor Line Visual  

|Details|Status|  
|:---|:---|  
|ID|**change_to_edi_line_visual**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`V`|  
|Default Command|-| 

It is the same as **V** on the original Vim.  

<br>  

<hr>  

## Common  
### Command Mode  

|Details|Status|  
|:---|:---|  
|ID|**command_mode**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`:`|  
|Default Command|-| 

It emulates the command mode of Vim. When you press **Enter**, it will call the function matched the **cmd** in  **bindings.json**. You can then use **Esc** to cancel. And, moving the history is available with **Up** or **Down**.  However, the order of history is currently not updated. It is recommended to use <a href="https://pit-ray.github.io/win-vind/cheat_sheet/options/#virtual-command-line">**Virtual Command Line**</a> for visualization when using this function.  

