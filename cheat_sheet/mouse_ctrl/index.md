---
layout: default
title: Mouse Control
nav_order: 2
parent: Cheat Sheet
descripption: "Mouse Control"
---

# Mouse Control  
{: .no_toc}  

<hr>

1. TOC  
{:toc} 

<hr>  

## Move  

It is based on the physical movemant, so you can change its the acceleration or the maximium velocity or its weight in **settings.json**.  

### Move Left  

|Details|Status|  
|:---|:---|  
|ID|**move_left**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`h`, `<C-h>`, `<BS>`, `<Left>`|  
|Default Command|-|  

It moves the cursor to the left. 


<br>  

### Move Right  

|Details|Status|  
|:---|:---|  
|ID|**move_right**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`l`, `<Space>`, `<Right>`|  
|Default Command|-|  

It moves the cursor to the right.

<br>  

### Move Up  

|Details|Status|  
|:---|:---|  
|ID|**move_up**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`k`, `-`, `gk`, `<C-p>`, `<Up>`|  
|Default Command|-|  

It moves the cursor up. 

<br>  

### Move Down  

|Details|Status|  
|:---|:---|  
|ID|**move_down**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`j`, `+`, `gj`, `<C-n>`, `<Down>`, `<Enter>`, `<C-m>`|  
|Default Command|-|  

It moves the cursor down.   

<br>  


<hr>  

## Jump  

If there are multiple displays, they are processed as one combined screen.  

### Jump to Left  

|Details|Status|  
|:---|:---|  
|ID|**jump_to_left**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`0`, `^`, `<Home>`, `g0`, `g^`|  
|Default Command|-|  

It moves the cursor to the left end of the screen.   

<br>  

### Jump to Right

|Details|Status|  
|:---|:---|  
|ID|**jump_to_right**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`$`, `<End>`, `g$`|  
|Default Command|-|  

It moves the cursor to the right end of the screen.   

<br>  


### Jump to Top  

|Details|Status|  
|:---|:---|  
|ID|**jump_to_top**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`gg`|  
|Default Command|-|  

It moves the cursor to the top of the screen.   

<br>  

### Jump to Bottom  

|Details|Status|  
|:---|:---|  
|ID|**jump_to_bottom**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`G`|  
|Default Command|-|  

It moves the cursor to the bottom of the screen.   

<br>

### Jump to Horizontal Center  

|Details|Status|  
|:---|:---|  
|ID|**jump_to_xcenter**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`gm`, `gM`|  
|Default Command|-|  

It moves the cursor to the horizontal center. If you use multiple displays, the position are a half of the width on one combined screen.    

<br>

### Jump to Vertical Center 

|Details|Status|  
|:---|:---|  
|ID|**jump_to_ycenter**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`M`|  
|Default Command|-|  

It moves the cursor to the vertical center. If you use multiple displays, the position are a half of the height on one combined screen.    

<br>  

### Jump Cursor by Keyboard Mapping  

|Details|Status|  
|:---|:---|  
|ID|**jump_to_any**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`f`|  
|Default Command|-| 

This makes key coordinates on keyboard project coordinates of the desktop.

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/Jump2Any-image.png?raw=true" width="512">  

For example, if you type **F&lt;Space&gt;**, jump the mouse cursor to lower center.  

However, the keyboard map is defferent according to languages, so you are able to choose the keyboard type at **Preferences/Settings/Common/Keyboard Type**.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/kb_type.jpg?raw=true" width="512">
    
Additionaly, positions of key are very hardware-dependent, so if you don't like default keyboard mapping, you can make optimized <b>custom.kmp</b> as follow ways.

1. Choice **Custom** in **Preferences/Settings/Common/Keyboard Type**.  
1. Please open the **custom.kmp**. If you use the installer version, the file exists **~/.win-vind/custom.kmp**. If you use zip, it does **win-vind/config/custom.kmp**  
1. Rewrite the kmp file.   
   **Syntax**
   ```
   [Xposition]    [Yposition]    [KeyCode]
   ```  
   - The delimiter is some space and multiple spaces is same as one space. You must write a syntax per line. **[Xposition]** and **[Yposition]** are positive values. Also, Left Upper at the real keyboard is (0, 0) and the max value of X means right-end, Y does bottom. And then, **[KeyCode]** is ASCII Code or KMP Key Code. However It is **not** supported **Fn** Key. In addtion, Toggle Key for example, **CapsLock** or **NumLock**, is sometimes ignored because kept the push-release-state of the key by hardware according to a product.  
   - Example  
   <img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/kmp_ex.jpg?raw=true" width="512">

<br>

### Jump to Active Window

|Details|Status|  
|:---|:---|  
|ID|**jump_to_active_window**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`t`, `T`|  
|Default Command|-| 

It makes cursor jump center of most foreground window.   

<br>
hr>  

## Scroll  
### Scroll Up

### Scroll Down

### Scroll Middle Page Up

### Scroll Middle Page Down

### Scroll Page Up

### Scroll Page Down  

### Scroll Left

### Scroll Right

### Scroll Middle Page Left  

### Scroll Middle Page Right  

<hr>  

## Click  

### Left Click

### Right Click  

### Select All  

### EasyClick

### Update EasyClick
