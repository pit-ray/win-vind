---
layout: default
title: Editor Normal Mode
nav_order: 3
descripption: "Command lists of win-vind."
parent: Cheat Sheet
---

# Editor Normal Mode
{: .no_toc}  

<hr>

1. TOC1  
{:toc}

## Default

### Mode

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`<Esc-Left>`|**to_gui_normal**|GUI Normal Mode||
|`<c-i>`|**to_resident**|Resident Mode||
|`:`|**to_command**|Command Mode||
|`i`|**to_insert**|Insert Mode||
|`v`|**to_edi_visual**|Visual Mode (Vim: v)||
|`V`|**to_edi_visual_line**|Visual-Line Mode (Vim: V)||

### Mouse

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`<C-y>`, `<C-k>`|**scroll_up**|Scroll a mouse wheel up||
|`<C-e>`, `<C-j>`|**scroll_down**|Scroll a mouse wheel down||
|`<C-u>`|**scroll_up_halfpage**|Scroll a mouse wheel up with a half page||
|`<C-d>`|**scroll_down_halfpage**|Scroll a mouse wheel down with a half page||
|`<C-b>`|**scroll_up_onepage**|Scroll a mouse wheel up with a page||
|`<C-f>`|**scroll_down_onepage**|Scroll a mouse wheel down With a page||
|`zh`, `<C-h>`|**scroll_left**|Scroll a mouse wheel left||
|`zl`, `<C-l>`|**scroll_right**|Scroll a mouse wheel right||
|`zH`|**scroll_left_halfpage**|Scroll a mouse wheel left With a half page||
|`zL`|**scroll_right_halfpage**|Scroll a mouse wheel right with a half page||
|`FF`, `Fo`|**easy_click_left**|EasyClick (Left Click)||
|`Fa`|**easy_click_right**|EasyClick (Right Click)||
|`Fm`|**easy_click_mid**|EasyClick (Mid Click)||
|`Fh`|**easy_click_hover**|EasyClick (Hovering)||

### HotKey

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`<C-r>`|**redo**|Redo||
|`u`, `U`|**undo**|Undo||
|`/`, `?`|**search_pattern**|Search Pattern||
|`<gt>`|**goto_next_page**|Forward a next page||
|`<lt>`|**goto_prev_page**|Backward a previous page||

### Tab

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`gT`|**switch_to_left_tab**|Switch to a left tab|
|`gt`|**switch_to_right_tab**|Switch to a right tab|


### Window

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`<C-w>s`|**switch_window**|Switch a window||
|`<C-w>e`|**window_resizer**|Window Resizer||
|`<C-w>h`|**select_left_window**|Select a left window||
|`<C-w>l`|**select_right_window**|Select a right window||
|`<C-w>k`|**select_upper_window**|Select an upper window||
|`<C-w>j`|**select_lower_window**|Select a lower window||
|`<C-w>u`|**maximize_current_window**|Maximize a current window||
|`<C-w>d`|**minimize_current_window**|Minimize a current window||
|`<C-w>H`, `<C-w><Left>`|**snap_current_window_to_left**|Snap a current window to left||
|`<C-w>L`, `<C-w><Right>`|**snap_current_window_to_right**|Snap a current window to right||
|`<C-w>K`|**snap_current_window_to_top**|Snap a current window to top||
|`<C-w>J`|**snap_current_window_to_bottom**|Snap a current window to bottom||
|`<C-w>=`|**arrange_windows**|Arrange windows||
|`<C-w>r`|**rotate_windows**|Rotate windows in a current monitor||
|`<C-w>R`|**rotate_windows_in_reverse**|Rotate windows in a current monitor in reverse||
|`<C-w>x`|**exchange_window_with_nearest**|Exchange a window with next one||
|`<C-w><gt>`|**increase_window_width**|Increase the width of a window||
|`<C-w><lt>`|**decrease_window_width**|Decrease the width of a window||
|`<C-w>+`|**increase_window_height**|Increase the height of a window||
|`<C-w>-`|**decrease_window_height**|Decrease the height of a window||
|`<C-w>n`|**open_new_window**|Open a new window||

### Option

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`<LAlt><LAlt>`, `<F2>`|**enable_targeting_of_dedicate_to_window**|Enable targeting (Dedicate to One Window)||
|`<RAlt><RAlt>`, `<F10>`|**disable_targeting_of_dedicate_to_window**|Disable targeting (Dedicate to One Window)||


### Vim Emulation

#### Mode

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`I`, `gI`|**to_insert_BOL**|Insert to begin of line (Vim: I)||
|`a`|**to_insert_append**|Append after a caret (Vim: a)||
|`A`|**to_insert_EOL**|Append end of line (Vim: A)||
|`o`|**to_insert_nlbelow**|Begin new line below a caret (Vim: o)||
|`O`|**to_insert_nlabove**|Begin new line above a caret (Vim: O)||

#### Move

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`h`, `<C-h>`, `<BS>`, `<Left>`|**move_caret_left**|Move the caret to left (Vim: h)||
|`l`, `<Space>`, `<Right>`|**move_caret_right**|Move the caret to right (Vim: l)||
|`k`, `-`, `gk`, `<C-p>`, `<Up>`|**move_caret_up**|Move the caret up (Vim: k)||
|`j`, `+`, `gj`, `<C-n>`, `<Down>`, `<Enter>`, `<C-m>`|**move_caret_down**|Move the caret down (Vim: j)||
|`w`|**move_caret_word_forward**|Forward with words (Vim: w)||
|`b`|**move_caret_word_backward**|Backward with words (Vim: b)||
|`W`|**move_caret_nonblank_word_forward**|Forward with WORDS (Vim: W)||
|`B`|**move_caret_nonblank_word_backward**|Backward with WORDS (Vim: B)||

#### Jump

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`0`, `<Home>`, `g0`|**jump_caret_to_BOL**|Jump the caret to begin of line (Vim: 0)||
|`$`, `<End>`, `g$`|**jump_caret_to_EOL**|Jump the caret to end of line (Vim: $)||
|`gg`|**jump_caret_to_BOF**|Jump the caret to BOF (Vim: gg)||
|`G`|**jump_caret_to_EOF**|Jump the caret to EOF (Vim: G)||

#### Edit

|Bindings|ID|Feature|Note|
|:---:|:---:|:---:|:---|
|`yy`, `Y`|**yank_line**|Yank lines (Vim: yy)||
|`y`|**yank_with_motion**|Yank lines with motion (Vim: y{Motion})||
|`p`|**put_after**|Put texts after the caret (Vim: p)||
|`P`|**put_before**|Put texts before the caret (Vim: P)||
|`dd`|**delete_line**|Delete lines (Vim: dd)||
|`D`|**delete_line_until_EOL**|Delete texts until end of line (Vim: D)||
|`x`, `<Del>`|**delete_after**|Delete chars after the caret (Vim: x)||
|`X`|**delete_before**|Delete chars before the caret (Vim: X)||
|`J`|**join_next_line**|Join a next line (Vim: J)||
|`r`|**replace_char**|Replace a char (Vim: r)||
|`R`|**replace_sequence**|Replace Mode (Vim: R)||
|`~`|**switch_char_case**|Switch char case (Vim: ~)||
|`d`|**delete_with_motion**|Delete texts with motion (Vim: d{Motion})||
|`c`|**change_with_motion**|Change texts with motion (Vim: c{Motion})||
|`cc`, `S`|**change_line**|Change Lines(Vim: cc, S)||
|`s`|**change_char**|Change Characters (Vim: s)||
|`C`|**change_until_EOL**|Change until EOL (Vim: C)||

<hr>

## Customization

Write `.vindrc` in the following way. The avaliable commands are `ennoremap`, `enunmap`, and `enmapclear`.

**Sample**
```vim
" ~/.win-vind/.vindrc

ennoremap ; to_command " Transition to Command Mode with ;
enmap <Capslock> <Esc> " Map <Capslock> to <Esc>
enumap <Left>          " Delete <Left> command
enmapclear             " Remove all bindings of Edi Normal
```

<br>
<br>
<br>
<br>
