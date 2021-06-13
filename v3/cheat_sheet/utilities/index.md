---
layout: default
title: Utilities
nav_order: 7
parent: Cheat Sheet for v3
descripption: "Utilities"
---

# Utilities
{: .no_toc}

<hr>

1. TOC
{:toc}

<hr>

## File  

### Save Opened File

|Details|Status|  
|:---|:---|  
|ID|**save_opened_file**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|-|  
|Default Command|`:w`, `:w_f`, `:w_a`, `:w_c`, `:up`, `:wall`| 

It is the same as **Ctrl + S** of shortcuts on Windows.  

<br>  


### Open Other File

|Details|Status|  
|:---|:---|  
|ID|**open_other_file**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|-|
|Default Command|`:find`, `:open`, `:e`, `:e!`, `:edit`, `:edit!`|

It is the same as **Ctrl + O** of shortcuts on Windows.  


<br>  

### Make Directories  

|Details|Status|  
|:---|:---|  
|ID|**make_dir**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|-|
|Default Command|`:mkdir`, `:md`, `:mkdir <any>`, `:mkdir <any>`| 

It has two calling patterns.  
- Absolutely  
If you want to make **TestDirectory** in **C:/ParentDirectory**, write like a bellow.  
```
:mkdir C:/ParentDirectory/TestDirectory
```

- Relatively
On the premise that the current directory are opened by **explorer.exe**, if you want to make **TestDirectory** in current directory, type as follow.  
```
:mkdir TestDirectory
```
It is same as **New folder** on explorer.exe. If no window of **explorer.exe** is opened, it regard the current directory as **~/Desktop**.

<br>

<hr>  

## Search

### Search Pattern  

|Details|Status|  
|:---|:---|  
|ID|**make_dir**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`/`, `?`|
|Default Command|-|

In the future, we plan to implement a sophisticated text analyzing system to emulate searches like Vim. However, we adopted **F3** of shortcuts on Windows as the alternative method now.  

<br>  

<hr>

## Page  

### Move to Next Page  

|Details|Status|  
|:---|:---|  
|ID|**move_to_next_page**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`<gt>`|
|Default Command|-|

It is the same as **Alt + Right** of shortcuts on Windows.  

<br>  

### Move to Prev Page  

|Details|Status|  
|:---|:---|  
|ID|**move_to_prev_page**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`<lt>`|
|Default Command|-|

It is the same as **Alt + Left** of shortcuts on Windows.  

<br>  

<hr>  


## Undo / Redo

### Undo  

|Details|Status|  
|:---|:---|  
|ID|**sc_undo**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`u`, `U`|
|Default Command|-|

It is the same as **Ctrl + Z** of shortcuts on Windows.  

<br>  

### Redo  

|Details|Status|  
|:---|:---|  
|ID|**sc_redo**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`<C-r>`|
|Default Command|-|

It is the same as **Ctrl + Y** of shortcuts on Windows.  

<br>  

<hr>

## Clipboard  

### Copy  

|Details|Status|  
|:---|:---|  
|ID|**cb_copy**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`y`, `yy`, `Y`|
|Default Command|-|

It is the same as **Ctrl + C** of shortcuts on Windows.  

<br>  

### Paste  

|Details|Status|  
|:---|:---|  
|ID|**cb_paste**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`p`, `P`|
|Default Command|-|

It is the same as **Ctrl + V** of shortcuts on Windows.  

<br>  

### Cut  

|Details|Status|  
|:---|:---|  
|ID|**cb_cut**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`dd`, `D`|
Default Command|-|

It is the same as **Ctrl + X** of shortcuts on Windows.  

<br>  

### Delete  

|Details|Status|  
|:---|:---|  
|ID|**cb_delete**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`x`, `<Del>`|
|Default Command|-|

It is the same as **Ctrl + C** and **Delete** of shortcuts on Windows. The differences with **cb_cut** is that it is copied once.  

<br>  

### BackSpace  

|Details|Status|  
|:---|:---|  
|ID|**cb_bacK_space**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`X`|
|Default Command|-|

It is **BackSpace** key with storing to a clipboard.  
