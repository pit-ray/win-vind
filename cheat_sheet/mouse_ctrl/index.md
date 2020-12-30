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

<br>

<hr>  

## Click  

### Left Click

|Details|Status|  
|:---|:---|  
|ID|**clicke_left**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`o`, `O`|  
|Default Command|-| 

It directly makes a mouse left click event by Windows API, so there is no delay.  

<br>


### Right Click  

|Details|Status|  
|:---|:---|  
|ID|**click_right**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`a`, `A`|  
|Default Command|-| 

It directly makes a mouse right click event by Windows API, so there is no delay.  

<br>


### Select All  

|Details|Status|  
|:---|:---|  
|ID|**select_all**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`V`|  
|Default Command|-| 

It is the same as **Ctrl + A** of shortcut keys on Windows.  

<br>

### EasyClick

|Details|Status|  
|:---|:---|  
|ID|**easy_click**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`F`|  
|Default Command|`:ec`| 

You can jump the cursor and click by typing the character identifiers like <a href="https://github.com/philc/vimium">Vimium</a> which is the famous Google Chrome Plugin or <a href="https://github.com/easymotion/vim-easymotion">EasyMotion</a> which is very famous as Vim Plugin. 

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/EasyClickDemo.gif?raw=true" />

It must deep-scan GUI objects in the selected window, another thread window in the same process, and these child windows. Therefore it's not very efficient in some applocations having a lot of objects. You make this efficient by enabling <a href="https://pit-ray.github.io/win-vind/cheat_sheet/options/#enable-easyclick-hwnd-cache">Enable EasyClick HWND Cache</a> option. And, the maximum number of drawn labels are only 676. In other words, all objects are detected, but not drawn than 676.  

The function performs well as long as a target application supported **UI Automation** of Windows API. For example, **EasyClick** also detects URL like **Vimium** on **Microsoft Edge**, but detects the only UI on **Google Chrome** or **Firefox**. Basically, it works well on all standard applications of Windows.    

<br>

### Update EasyClick

|Details|Status|  
|:---|:---|  
|ID|**update_easy_click**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`<C-F>`|  
|Default Command|-| 

It scans the GUI objects in the selected window, another thread window in the same process, and these child windows. And, it stores the scanned result as some caches. If you scan a window having a lot of GUI objects (e.g. **Microsoft Office Excel**), it may take a few seconds.   

<br>  

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

It makes the cursor jump to the center of the most foreground window.   

<br>
<hr>   

## Scroll  

The parameter ID of these functions are **yscroll_speed**, **xscroll_speed**, **yscroll_screen_ratio** and **xscroll_screen_ratio** in **settings.json**. The word "speed" is a relative value and there are not the concept of column or lines in GUI, so the word "Page" is defined with a ratio from 0.0 to 1.0 against the screen width or height as an alternative way. 

### Scroll Up

|Details|Status|  
|:---|:---|  
|ID|**scroll_up**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`<C-y>`, `<C-k>`|  
|Default Command|-| 

It directly makes a mouse scroll down event by Windows API.   

<br>


### Scroll Down

|Details|Status|  
|:---|:---|  
|ID|**scroll_down**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`<C-e>`, `<C-j>`|  
|Default Command|-| 

It directly makes a mouse scroll up event by Windows API.   

<br>


### Scroll Middle Page Up

|Details|Status|  
|:---|:---|  
|ID|**scroll_mid_up**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`<C-u>`|  
|Default Command|-| 

It up-scrolls a virtual mouse wheel substantially.  

<br>

### Scroll Middle Page Down

|Details|Status|  
|:---|:---|  
|ID|**scroll_mid_down**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`<C-d>`|  
|Default Command|-| 

It down-scrolls a virtual mouse wheel substantially.  

<br>


### Scroll Page Up

|Details|Status|  
|:---|:---|  
|ID|**scroll_page_up**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`<C-b>`|  
|Default Command|-| 

It up-scrolls a virtual mouse wheel very substantially.  

<br>

### Scroll Page Down  

|Details|Status|  
|:---|:---|  
|ID|**scroll_page_down**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`<C-f>`|  
|Default Command|-| 

It down-scrolls a virtual mouse wheel very substantially.  

<br>

### Scroll Left

|Details|Status|  
|:---|:---|  
|ID|**scroll_left**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`zh`, `<C-h>`|  
|Default Command|-| 

It directly makes a mouse scroll left event by Windows API.   


<br>

### Scroll Right

|Details|Status|  
|:---|:---|  
|ID|**scroll_right**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`zl`, `<C-l>`|  
|Default Command|-| 

It directly makes a mouse scroll right event by Windows API.   

<br>

### Scroll Middle Page Left  

|Details|Status|  
|:---|:---|  
|ID|**scroll_mid_left**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`zH`|  
|Default Command|-| 

It directly makes a mouse scroll left event by Windows API.   

<br>

### Scroll Middle Page Right  

|Details|Status|  
|:---|:---|  
|ID|**scroll_mid_right**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`zL`|  
|Default Command|-| 

It directly makes a mouse scroll right event by Windows API.   

