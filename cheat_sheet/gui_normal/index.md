---
layout: default
title: GUI Normal Mode
nav_order: 1
descripption: "Command lists of win-vind."
parent: Cheat Sheet
---

# GUI Normal Mode
{: .no_toc}  

<hr>

1. TOC1  
{:toc}


## Default

### Mode

|Bindings|ID|
|:---:|:---:|
|`v`|**to_gui_visual**|
|`I`, `<Esc-Right>`|**to_edi_normal**|
|`i`|**to_insert**|
|`<Esc-Down>`|**to_resident**|
|`:`|**to_command**|


### Mouse

|Bindings|ID|
|:---:|:---:|
|`h`, `<BS>`, `<Left>`|**move_cursor_left**|
|`l`, `<Space>`, `<Right>`|**move_cursor_right**|
|`k`, `-`, `<Up>`|**move_cursor_up**|
|`j`, `+`, `<Down>`|**move_cursor_down**|
|`0`, `^`, `<Home>`|**jump_cursor_to_left**|
|`$`, `<End>`|**jump_cursor_to_right**|
|`gg`|**jump_cursor_to_top**|
|`G`|**jump_cursor_to_bottom**|
|`gm`|**jump_cursor_to_hcenter**|
|`M`|**jump_cursor_to_vcenter**|
|`f`|**jump_cursor_with_keybrd_layout**|
|`t`|**jump_cursor_to_active_window**|
|`<C-y>`, `<C-k>`|**scroll_up**|
|`<C-e>`, `<C-j>`|**scroll_down**|
|`<C-u>`|**scroll_up_halfpage**|
|`<C-d>`|**scroll_down_halfpage**|
|`<C-b>`|**scroll_up_onepage**|
|`<C-f>`|**scroll_down_onepage**|
|`zh`, `<C-h>`|**scroll_left**|
|`zl`, `<C-l>`|**scroll_right**|
|`zH`|**scroll_left_halfpage**|
|`zL`|**scroll_right_halfpage**|
|`o`, `O`|**click_left**|
|`a`, `A`|**click_right**|
|`FF`, `Fo`|**easy_click_left**|
|`Fa`|**easy_click_right**|
|`Fm`|**easy_click_mid**|
|`Fh`|**easy_click_hover**|

### Hotkey

|Bindings|ID|
|:---:|:---:|
|`V`|**select_all**|
|`y`, `yy`, `Y`|**hotkey_copy**|
|`p`, `P`|**hotkey_paste**|
|`dd`, `D`|**hotkey_cut**|
|`x`, `<Del>`|**hotkey_delete**|
|`X`|**hotkey_backspace**|
|`<C-r>`|**redo**|
|`u`, `U`|**undo**|
|`/`, `?`|**search_pattern**|
|`<gt>`|**goto_next_page**|
|`<lt>`|**goto_prev_page**|
|`<win>`|**open_startmenu**|

### Virtual Desktop

|Bindings|ID|
|:---:|:---:|
|`<C-v>h`|**switch_to_left_vdesktop**|
|`<C-v>l`|**switch_to_right_vdesktop**|
|`<C-v>n`|**create_new_vdesktop**|
|`<C-v>q`|**close_current_vdesktop**|
|`<C-v>s`|**taskview**|

### Tab

|Bindings|ID|
|:---:|:---:|
|`gT`|**switch_to_left_tab**|
|`gt`|**switch_to_right_tab**|


### Window

|Bindings|ID|
|:---:|:---:|
|`<C-w>q`, `<C-w>c`|**close_current_window**|
|`<C-w>s`|**switch_window**|
|`<C-w>e`|**window_resizer**|
|`<C-w>h`|**select_left_window**|
|`<C-w>l`|**select_right_window**|
|`<C-w>k`|**select_upper_window**|
|`<C-w>j`|**select_lower_window**|
|`<C-w>u`|**maximize_current_window**|
|`<C-w>d`|**minimize_current_window**|
|`<C-w>H`, `<C-w><Left>`|**snap_current_window_to_left**|
|`<C-w>L`, `<C-w><Right>`|**snap_current_window_to_right**|
|`<C-w>K`|**snap_current_window_to_top**|
|`<C-w>J`|**snap_current_window_to_bottom**|
|`<C-w>=`|**arrange_windows**|
|`<C-w>r`|**rotate_windows**|
|`<C-w>R`|**rotate_windows_in_reverse**|
|`<C-w>x`|**exchange_window_with_nearest**|
|`<C-w><gt>`|**increase_window_width**|
|`<C-w><lt>`|**decrease_window_width**|
|`<C-w>+`|**increase_window_height**|
|`<C-w>-`|**decrease_window_height**|
|`<C-w>n`|**open_new_window**|

<hr>

## Customize

Write `.vindrc` in the following way. The avaliable commands are `gnnoremap`, `gnunmap`, and `gnmapclear`.

**Sample**
```vim
" ~/.win-vind/.vindrc

gnnoremap <c-f> easy_click_left     " Call easy_click_left with Ctrl + f
gnnoremap <c-h> select_left_window  " Call select_left_window with Ctrl + h
gnmap <Capslock> <ctrl>             " Map <Capslock> to <Ctrl> with low-level
gnumap FF                           " Delete FF command
gnmapclear                          " Remove all bindings of GUI Normal
```

<br>
<br>
<br>
<br>
