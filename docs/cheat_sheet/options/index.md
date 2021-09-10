---
layout: page
title: Options and Parameters
disable_anchors: true
show_in_menu: false
description: parameters and useful extensions
---

|ID|Type|Default|Note|
|:---:|:---:|:---:|:---|
|`arrangewin_ignore`|str||A list of executable filenames to ignore in ArrangeWindows. For example, if you want to remove rainmeter and gvim from the alignment, write `set arrangewin_ignore = rainmeter, gvim`. The name is the name of the executable file without extension.|
|`autofocus_textarea`|bool|false|Automatically focus on the nearest text area when switching to **Editor Normal Mode**|
|`autotrack_popup`|bool|false|It is one of standard options on Windows. For example, if shown **Are you sure you want to move this file to the Recycle Bin?**, it automatically moves the cursor to the popup window.|
|`blockstylecaret_mode`|str|solid|Mode of block style caret.  There is a `solid` mode with fixed size and a `flex` mode with pseudo blocks by selection.|
|`blockstylecaret_width`|num|15|Width of block style caret on solid mode|
|`blockstylecaret`|bool|false|Block Style Caret|
|`charcache`|bool|false|It is a very small cache for one character used by `x` or `X` commands. If it is enabled, the clipboard is opened per once typing. Therefore, you will get the same behavior as the original Vim, whereas the performance maybe drop a litte.|
|`cmd_bgcolor`|str|323232|Font Background Color (# is optional)|
|`cmd_fadeout`|num|5|Fade-out time in seconds for the virtual command line. If you want the command line to always be visible, make this value large enough.|
|`cmd_fontcolor`|str|c8c8c8|Font color (# is optional)|
|`cmd_fontextra`|num|1|Horizontal character spacing in virtual command line.|
|`cmd_fontname`|str|Consolas|Font name for virtual command line. If an empty string is passed, the system font will be used.|
|`cmd_fontsize`|num|23|Font size in virtual command line|
|`cmd_fontweight`|num|400|Font weight in virtual command line. Its maximum value is 1000.|
|`cmd_maxchar`|num|32|Maximum number of characters per line in the virtual command line.|
|`cmd_maxhist`|num|10|Maximum number of histories in the virtual command line.|
|`cmd_roughpos`|str|LowerMid|Rough position of virtual command line. The choices are `UpperLeft`, `UpperMid`, `UpperRight`, `MidLeft`, `Center`, `MidRight`, `LowerLeft`, `LowerMid`, or `LowerRight`.|
|`cmd_xmargin`|num|32|Use `cmd_roughpos` to determine the rough position, and `cmd_xmargin` to determine the detailed horizontal position. The units are in pixels.|
|`cmd_ymargin`|num|64|Use `cmd_roughpos` to determine the rough position, and `cmd_ymargin` to determine the detailed vertical position. The units are in pixels.|
|`cursor_accel`|num|95|Pixel-level acceleration in the constatnt acceleration motion of the mouse cursor.|
|`cursor_maxv`|num|12|Maximum velocity of the mouse cursor|
|`cursor_tweight`|num|250|A weight for scaling the time of constant acceleration motion of the mouse cursor.|
|`dedicate_to_window`|bool|false|If **Dedicate to One window** enables, you can select one window with Enable Targeting function. In this case, it makes the mode automatically switch to Editor Normal Mode on the targeting window. When the foreground window change to another, it makes the mode switch to Insert Mode. The targeting becomes disable with Disable Targeting function. In other words, this feature transforms some normal editors to fake Vim. The computing cost is so small.|
|`easyclick_bgcolor`|str|323232|Font background color of hints in EasyClick|
|`easyclick_colordecay`|num|100|Matching color decay of hints in EasyClick|
|`easyclick_fontcolor`|str|c8c8c8|Font color of hints in EasyClick|
|`easyclick_fontname`|str|Arial|Font name of hints in EasyClick|
|`easyclick_fontsize`|num|14|Font size of hints in EasyClick|
|`easyclick_fontweight`|num|500|Font weight of hits in EasyClick. Its maximum value is 1000.|
|`gui_fontname`|str|Segoe UI|Font name of GUI. If an empty string is passed, the system font will be used.|
|`gui_fontsize`|num|11|Font size of GUI|
|`hscroll_pageratio`|num|0.125|The ratio of one page to the screen width to determine the amount of scrolling movement as a page.
|`hscroll_speed`|num|10|Horizontal scrolling speed of the mouse wheel.|
|`icon_style`|str|resources/icon32_dark.ico|Style of the icon to be displayed on the taskbar. By default, **Dark** and **Light** styles are available. The former is `resources/icon32_dark.ico` and the latter is `resouces/icon32_light.ico`. By the way, you can use any tasktray icon you like as long as it is in `.ico` format and **32x32**.|
|`initmode`|str|i|Initial mode of win-vind. The value is the mode prefix.|
|`jump_margin`|num|10|A margin in pixels to prevent jumping off the screen when jumping to the edge of the screen using `jump_cursor_to_left`, etc.|
|`keybrd_layout`|str||Keyboard layout kmp file referenced by `jump_cursor_with_keybrd_layout`. By default, only **US (101/102)** or **JP (106/109)** layouts are supported. If your keyboard is not the right one, please create your own kmp file and use its path as the value. If you leave the value empty, the KMP file will be selected automatically. Also, if you like, you can share the keyboard you created with a pull request to the `default_config` directory.|
|`shell_startupdir`|str||The current directory where commands (e.g. `:shell`, `:terminal`, `:!`) will be executed. For these commands, the current directory is the directory if there is Exeplorer, or the user directory otherwise. If this option is not empty, then the current directory is fixed to a value directory.|
|`shell`|str|cmd|Name of the shell to use for `:!` commands|
|`shellcmdflag`|str|-c|Flag passed to the shell to execute `:!` commands|
|`suppress_for_vim`|bool|false|It makes the mode of win-vind automatically switch to **Resident Mode** on the applications included the strings called **VIM** in an executable file name. Thus, it allows us to smoothly move from win-vind having the same key-bindings as Vim to Vim applications.|
|`uiacachebuild_lifetime`|num|1500|Cache lifetime (ms). A high value reduces the computational cost, but decreases the reliability of the cache. A low value increases the computational cost due to frequent cache creation, but guarantees reliability.|
|`uiacachebuild_staybegin`|num|500|The time between when the mouse cursor stops moving and when it starts to build a cache. In order to reduce unnecessary computational cost, it is desirable not to create a cache when there is no operation. Therefore, it should be updated only immediately after the mouse stops. The value of this option is the time(ms) that the mouse cursor is considered to be stopped. Please refer to [appendix](#overview-of-stay-range-in-uiacachebuild)|
|`uiacachebuild_stayend`|num|2000|In order to reduce unnecessary computational cost, it is desirable not to create a cache when there is no operation. The value of this option is the time(ms) between the time the cursor stops moving and the time it stops creating a cache. Please refer to appendix [appendix](#overview-of-stay-range-in-uiacachebuild)|
|`uiacachebuild`|bool|false|EasyClick and `autofocus_textarea` are slow because they scan the UI object after being called. If this option is enabled, scanning is done asynchronously and cache is used as a result. Using the cache is 30 times faster than scanning linearly, but the location information, etc. may not always be correct.|
|`vcmdline`|bool|true|show virtual command line|
|`vscroll_pageratio`|num|0.125|The ratio of one page to the screen height to determine the amount of scrolling movement as a page.
|`vscroll_speed`|num|30|Vertical scrolling speed of the mouse wheel.|
|`window_accel`|num|95|Pixel-level acceleration in the constatnt acceleration motion of the window in winresizer.|
|`window_hdelta`|num|100|Window Width delta for resizing|
|`window_maxv`|num|12|Maximum velocity of the window in winresizer.|
|`window_tweight`|num|250|A weight for scaling the time of constant acceleration motion of the window in winresizer.|
|`window_vdelta`|num|100|Window height delta for resizing|
|`winresizer_initmode`|num|0|Initial mode of window resizer ([0]: Resize, [1]: Move, [2]: Focus)|


## Appendix

### Overview of stay range in `uiacachebuild`
<p align="center">
<img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/uiacachebuild_stay.png?raw=true" width=600>  
<p align="center">uiacachebuild_staybegin and uiacachebuild_stayend overview</p>
</p>


<br>
<br>
<br>
<br>
