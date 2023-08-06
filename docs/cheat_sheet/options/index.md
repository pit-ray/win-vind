---
layout: page
title: Options and Parameters - win-vind
nav: Options and Parameters
disable_anchors: true
show_in_menu: false
---

## System

### **`initmode`**
**type**: str, **default**: i  
Initial mode of win-vind. The value is the mode prefix.

<hr class="dash">

### **`listen_interval`**
**type**: float, **default**: 1.0  
The time interval in seconds at which the server win-vind will retrieve command requests sent by the client with the `-c` argument.

<hr class="dash">

### **`icon_style`**
**type**: str, **default**: resources/icon32_dark.ico  
Style of the icon to be displayed on the taskbar. By default, **Dark** and **Light** styles are available. The former is `resources/icon32_dark.ico` and the latter is `resouces/icon32_light.ico`. By the way, you can use any tasktray icon you like as long as it is in `.ico` format and **32x32**.

<hr class="dash">

### **`tempdir`**
**type**: str, **default**: ~/Downloads  
Where to download the file with the update checking.

<hr class="dash">

### **`gui_fontname`**
**type**: str, **default**: Segoe UI  
Font name of GUI. If an empty string is passed, the system font will be used.

<hr class="dash">

### **`gui_fontsize`**
**type**: num, **default**: 11  
Font size of GUI

<hr class="dash">

### **`hintkeys`**
**type**: str, **default**: asdghklqwertyuiopzxcvbnmfj  
Specify the characters of hint used for EasyClick and GridMove. It accpets as input a set of non-duplicate characters and assigns them to the hints in order from the first to the last.



## Command Line

### **`vcmdline`**
**type**: bool, **default**: true  
show virtual command line

<hr class="dash">

### **`cmd_bgcolor`**
**type**: str, **default**: 323232  
Background color in the virtual command line. (# is optional)

<hr class="dash">

### **`cmd_fontcolor`**
**type**: str, **default**: c8c8c8  
Font color in the virtual command line. (# is optional)

<hr class="dash">

### **`cmd_fontname`**
**type**: str, **default**: Consolas  
Font name for virtual command line. If an empty string is passed, the system font will be used.

<hr class="dash">

### **`cmd_fontsize`**
**type**: num, **default**: 23  
Font size in virtual command line

<hr class="dash">

### **`cmd_fontweight`**
**type**: num, **default**: 400  
Font weight in virtual command line. Its maximum value is 1000.

<hr class="dash">

### **`cmd_fontextra`**
**type**: num, **default**: 1  
Horizontal character spacing in virtual command line.

<hr class="dash">

### **`cmd_roughpos`**
**type**: str, **default**: LowerMid  
Rough position of virtual command line. The choices are `UpperLeft`, `UpperMid`, `UpperRight`, `MidLeft`, `Center`, `MidRight`, `LowerLeft`, `LowerMid`, or `LowerRight`.

<hr class="dash">

### **`cmd_xmargin`**
**type**: num, **default**: 32  
Use `cmd_roughpos` to determine the rough position, and `cmd_xmargin` to determine the detailed horizontal position. The units are in pixels.

<hr class="dash">

### **`cmd_ymargin`**
**type**: num, **default**: 96  
Use `cmd_roughpos` to determine the rough position, and `cmd_ymargin` to determine the detailed vertical position. The units are in pixels.

<hr class="dash">

### **`cmd_fadeout`**
**type**: num, **default**: 5  
Fade-out time in seconds for the virtual command line. If you want the command line to always be visible, make this value large enough.

<hr class="dash">

### **`cmd_monitor`**
**type**: str, **default**: primary  
The monitor on which to draw the command line. The choices are `primary`, `all`, `active`, `${NUMBER}`. The `primary` displays the command line on the primary monitor only. `all` draws command lines on all monitos. `active` displays command lines on the monitor where the selected window is located. `${NUMBER}` shows the command line on `${NUMBER}`th monitor. The `${NUMBER}` is a number starting from 0 and assigned from the left monitor. For example, `set cmd_monitor=1`.


## EasyClick

### **`easyclick_bgcolor`**
**type**: str, **default**: 323232  
Font background color of hints in EasyClick

<hr class="dash">

### **`easyclick_fontcolor`**
**type**: str, **default**: c8c8c8  
Font color of hints in EasyClick

<hr class="dash">

### **`easyclick_fontname`**
**type**: str, **default**: Consolas  
Font name of hints in EasyClick

<hr class="dash">

### **`easyclick_fontsize`**
**type**: num, **default**: 14  
Font size of hints in EasyClick

<hr class="dash">

### **`easyclick_fontweight`**
**type**: num, **default**: 500  
Font weight of hits in EasyClick. Its maximum value is 1000.


## GridMove

### **`gridmove_bgcolor`**
**type**: str, **default**: 323232  
Font background color of hints in GridMove

<hr class="dash">

### **`gridmove_fontcolor`**
**type**: str, **default**: c8c8c8  
Font color of hints in GridMove

<hr class="dash">

### **`gridmove_fontname`**
**type**: str, **default**: Consolas  
Font name of hints in GridMove

<hr class="dash">

### **`gridmove_fontsize`**
**type**: num, **default**: 14  
Font size of hints in GridMove

<hr class="dash">

### **`gridmove_fontweight`**
**type**: num, **default**: 500  
Font weight of hits in GridMove. Its maximum value is 1000.

<hr class="dash">

### **`gridmove_size`**
**type**: str, **default**: 12x8  
The grid size in GridMove. It assumes a text as its value, such as `12x8` for horizontal 12 cells and vertical 8 cells.


## Mouse

### **`cursor_accel`**
**type**: num, **default**: 90  
Pixel-level acceleration in the constatnt acceleration motion of the mouse cursor.

<hr class="dash">

### **`cursor_resolution`**
**type**: num, **default**: 250  
A weight for scaling the time of constant acceleration motion of the mouse cursor.

<hr class="dash">

### **`jump_margin`**
**type**: num, **default**: 10  
A margin in pixels to prevent jumping off the screen when jumping to the edge of the screen using `jump_cursor_to_left`, etc.

<hr class="dash">

### **`hscroll_pageratio`**
**type**: num, **default**: 0.125  
The ratio of one page to the screen width to determine the amount of scrolling movement as a page.

<hr class="dash">

### **`hscroll_speed`**
**type**: num, **default**: 10  
Horizontal scrolling speed of the mouse wheel.

<hr class="dash">

### **`vscroll_pageratio`**
**type**: num, **default**: 0.125  
The ratio of one page to the screen height to determine the amount of scrolling movement as a page.

<hr class="dash">

### **`vscroll_speed`**
**type**: num, **default**: 30  
Vertical scrolling speed of the mouse wheel.

<hr class="dash">

### **`keybrd_layout`**
**type**: str, **default**:   
Keyboard layout kmp file referenced by `jump_cursor_with_keybrd_layout`. By default, only **US (101/102)** or **JP (106/109)** layouts are supported. If your keyboard is not the right one, please create your own kmp file and use its path as the value. If you leave the value empty, the KMP file will be selected automatically.


## Window

### **`arrangewin_ignore`**
**type**: str, **default**:   
A list of executable filenames to ignore in ArrangeWindows. For example, if you want to remove rainmeter and gvim from the alignment, write `set arrangewin_ignore = rainmeter, gvim`. The name is the name of the executable file without extension.

<hr class="dash">

### **`window_velocity`**
**type**: num, **default**: 100  
Pixel-level velocity in the constatnt acceleration motion of the window in winresizer.

<hr class="dash">

### **`window_hdelta`**
**type**: num, **default**: 100  
Window Width delta for resizing

<hr class="dash">

### **`window_vdelta`**
**type**: num, **default**: 100  
Window height delta for resizing

<hr class="dash">

### **`winresizer_initmode`**
**type**: num, **default**: 0  
Initial mode of window resizer ([0]: Resize, [1]: Move, [2]: Focus)


## Block Style Caret

### **`blockstylecaret`**
**type**: bool, **default**: false  
Block Style Caret

<hr class="dash">

### **`blockstylecaret_mode`**
**type**: str, **default**: solid  
Mode of block style caret.  There is a `solid` mode with fixed size and a `flex` mode with pseudo blocks by selection.

<hr class="dash">

### **`blockstylecaret_width`**
**type**: num, **default**: 15  
Width of block style caret on solid mode


## AutoFocus

### **`autofocus_textarea`**
**type**: bool, **default**: false  
Automatically focus on the nearest text area when switching to **Editor Normal Mode**

<hr class="dash">

### **`autotrack_popup`**
**type**: bool, **default**: false  
It is one of standard options on Windows. For example, if shown **Are you sure you want to move this file to the Recycle Bin?**, it automatically moves the cursor to the popup window.


## UIA Cache

### **`uiacachebuild`**
**type**: bool, **default**: false  
EasyClick and `autofocus_textarea` are slow because they scan the UI object after being called. If this option is enabled, scanning is done asynchronously and cache is used as a result. Using the cache is 30 times faster than scanning linearly, but the location information, etc. may not always be correct.

<hr class="dash">

### **`uiacachebuild_lifetime`**
**type**: num, **default**: 1500  
Cache lifetime (ms). A high value reduces the computational cost, but decreases the reliability of the cache. A low value increases the computational cost due to frequent cache creation, but guarantees reliability.

<hr class="dash">

### **`uiacachebuild_staybegin`**
**type**: num, **default**: 500  
The time between when the mouse cursor stops moving and when it starts to build a cache. In order to reduce unnecessary computational cost, it is desirable not to create a cache when there is no operation. Therefore, it should be updated only immediately after the mouse stops. The value of this option is the time(ms) that the mouse cursor is considered to be stopped. Please refer to [appendix](#overview-of-stay-range-in-uiacachebuild)

<hr class="dash">

### **`uiacachebuild_stayend`**
**type**: num, **default**: 2000  
In order to reduce unnecessary computational cost, it is desirable not to create a cache when there is no operation. The value of this option is the time(ms) between the time the cursor stops moving and the time it stops creating a cache. Please refer to appendix [appendix](#overview-of-stay-range-in-uiacachebuild)


<p align="center">
<img src="{{ site.url }}/imgs/uiacachebuild_stay.png" class="img-fluid">
<p align="center">uiacachebuild_staybegin and uiacachebuild_stayend overview</p>
</p>

## Shell

### **`shell`**
**type**: str, **default**: cmd  
Name of the shell to use for `:!` commands

<hr class="dash">

### **`shell_startupdir`**
**type**: str, **default**:   
The current directory where commands (e.g. `:shell`, `:terminal`, `:!`) will be executed. For these commands, the current directory is the directory if there is Exeplorer, or the user directory otherwise. If this option is not empty, then the current directory is fixed to a value directory.

<hr class="dash">

### **`shellcmdflag`**
**type**: str, **default**: -c  
Flag passed to the shell to execute `:!` commands


## Vim Emulation

### **`charbreak`**
**type**: str, **default**: grapheme  
Mode for how to split a single Unicode character. The `grapheme` mode treats a combination character as a single character. The `codepoint` mode processes the combination character for each codepoint.

<hr class="dash">

### **`charcache`**
**type**: bool, **default**: false  
It is a very small cache for one character used by `x` or `X` commands. If it is enabled, the clipboard is opened per once typing. Therefore, you will get the same behavior as the original Vim, whereas the performance maybe drop a litte.


## Options

### **`dedicate_to_window`**
**type**: bool, **default**: false  
If **Dedicate to One window** enables, you can select one window with `enable_targeting_of_dedicate_to_window`. In this case, it makes the mode automatically switch to Editor Normal Mode on the targeting window. When the foreground window change to another, it makes the mode switch to Insert Mode. The targeting becomes disable with `disable_targeting_of_dedicate_to_window`. In other words, this feature transforms some normal editors to fake Vim. The computing cost is so small.

<hr class="dash">

### **`suppress_for_vim`**
**type**: bool, **default**: false  
It makes the mode of win-vind automatically switch to **Resident Mode** on the applications included the strings called **VIM** in an executable file name. Thus, it allows us to smoothly move from win-vind having the same key-bindings as Vim to Vim applications.




<br>
<br>
<br>
<br>
