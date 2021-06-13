---
layout: default
title: Options
nav_order: 20
parent: Cheat Sheet
descripption: "Option lists"
---

# Options
{: .no_toc}

<hr>

1. TOC  
{:toc}  

<hr>

|ID|Type|Default|Note|
|:---:|:---:|:---:|:---|
|`initmode`|str|i|Initial mode of win-vind. The value is mode prefix.|
|`jump_margin`|num|10|Screen position buffer|
|`cursor_accel`|num|95|Cursor acceleration|
|`cursor_tweight`|num|250|Cursor time weight|
|`cursor_maxv`|num|12|Cursor max velocity|
|`window_accel`|num|95|Window move acceleration|
|`window_tweight`|num|250|Window move time weight|
|`window_maxv`|num|12|Window move max velocity|
|`winresizer_initmode`|num|0|Initial Mode of Window Resizer ([0]: Resize, [1]: Move, [2]: Focus)|
|`vscroll_speed`|num|30|Vertical Scroll Speed|
|`hscroll_speed`|num|10|Horizontal Scroll Speed|
|`vscroll_pageratio`|num|0.125|Vertical Page Scroll Ratio|
|`hscroll_pageratio`|num|0.125|Horizontal Page Scroll Ratio|
|`cmd_maxchar`|num|32|Max Character Num|
|`cmd_maxhist`|num|10|Max Command History Num|
|`cmd_fontsize`|num|23|Font Size|
|`cmd_fontweight`|num|400|Font Weight|
|`cmd_fontcolor`|str|c8c8c8|Font Color|
|`cmd_bgcolor`|str|323232|Font Background Color|
|`cmd_roughpos`|str|LowerMid|Position of Virtual Command Line. {UpperLeft, UpperMid, UpperRight, MidLeft, Center, MidRight, LowerLeft, LowerMid, LowerRight}|
|`cmd_xmargin`|num|32|Horizontal Margin|
|`cmd_ymargin`|num|64|Vertical Margin|
|`cmd_fontextra`|num|1|Font Extra|
|`cmd_fontname`|str|Consolas|Font Name|
|`cmd_fadeout`|num|5|Fade Out Time (s)|
|`gui_fontsize`|num|11|GUI Font Size|
|`gui_fontname`|num|Consolas|GUI Font Name|
|`icon_style`|str|resources/icon32_dark.ico|Dark style or Light style are avaliable in **resources** directory. You can also use any icon you like.|
|`keybrd_layout`|str||If it is empty, choose automatically layout in US type or JP type. You can set unique kmp file.|
|`shell`|str|powershell|Name of the shell to use for \":!\" commands|
|`shellcmdflag`|str|-c|Flag passed to the shell to execute \":!\" commands|
|`shell_startupdir`|str||Current directory after shell startup|
|`autotrack_popup`|bool|false|Autotrack Popup|
|`dedicate_to_window`|bool|false|Dedicate to One Window|
|`vcmdline`|bool|true|Virtual Command Line|
|`suppress_for_vim`|bool|false|Suppress For Vim|
|`easyclick_fontsize`|num|14|EasyClick Font Size|
|`easyclick_fontweight`|num|500|EasyClick Font Weight|
|`easyclick_fontname`|str|Consolas|EasyClick Font Name|
|`easyclick_fontcolor`|str|c8c8c8|EasyClick Font Color|
|`easyclick_bgcolor`|str|323232|EasyClick Font Background Color|
|`easyclick_colordecay`|num|100|EasyClick Matching Color Decay|
|`charcache`|bool|false|Enable Char Cache|
|`window_hdelta`|num|100|Window Width delta for resizing|
|`window_vdelta`|num|100|Window height delta for resizing|
