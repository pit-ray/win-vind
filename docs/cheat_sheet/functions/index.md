---
layout: page
title: Functions - win-vind
nav: Functions
show_in_menu: false
disable_anchors: true
---

## Mode

### **`<to_command>`**
Enter the command mode, which is generally called with `:`.
In the command mode, the typed characters are displayed on the virtual command line.
You can operate the virtual command line as shown in the following table.

|**Key**|**Operation**|
|:---:|:---:|
|`<enter>`|Execute the current command|
|`<bs>`|Delete characters|
|`<up>`|Backward history|
|`<down>`|Forward history|
|`<tab>`|Complete commands|

<br>

<p align="center">
  <img src="https://pit-ray.github.io/win-vind/imgs/cmdline_demo.gif" class="img-fluid"/>
  <p align="center">Example: Change options via the virtual command line</p>
</p>

**Related Options**
- [vcmdline](../options/#vcmdline)
- [cmd_bgcolor](../options/#cmd_bgcolor)
- [cmd_fontcolor](../options/#cmd_fontcolor)
- [cmd_fontname](../options/#cmd_fontname)
- [cmd_fontsize](../options/#cmd_fontsize)
- [cmd_fontweight](../options/#cmd_fontweight)
- [cmd_fontextra](../options/#cmd_fontextra)
- [cmd_roughpos](../options/#cmd_roughpos)
- [cmd_xmargin](../options/#cmd_xmargin)
- [cmd_ymargin](../options/#cmd_ymargin)
- [cmd_fadeout](../options/#cmd_fadeout)
- [cmd_monitor](../options/#cmd_monitor)

**See Also**
- [\<command\>](./#command)
- [\<delcommand\>](./#delcommand)
- [\<comclear\>](./#comclear)

<hr class="dash">

### **`<to_gui_normal>`**
Transition to GUI normal mode. In GUI normal mode, the typed keys are not transmitted to Windows, so you can create any mapping you like without considering shortcut key conflicts.

**See Also**
- [\<to_gui_visual\>](./#to_gui_visual)
- [\<to_instant_gui_normal\>](./#to_instant_gui_normal)
- [\<gnmap\>](./#map)
- [\<gnnoremap\>](./#noremap)
- [\<gnunmap\>](./#unmap)
- [\<gnmapclear\>](./#mapclear)

<hr class="dash">

### **`<to_gui_visual>`**
Enter GUI visual mode. In this mode, the mouse is always in the click state and input is blocked from Windows as in normal mode.

**See Also**
- [\<to_gui_normal\>](./#to_gui_normal)
- [\<to_instant_gui_normal\>](./#to_instant_gui_normal)
- [\<mgvap\>](./#map)
- [\<gvnoremap\>](./#noremap)
- [\<gvunmap\>](./#unmap)
- [\<gvmapclear\>](./#mapclear)

<hr class="dash">

### **`<to_edi_normal>`**
Switch to the editor normal mode. This mode is essentially the same as GUI normal mode, but defines a lot of text-specific mappings that emulate Vim editing in order to achieve Vim everywhere.

**See Also**
- [\<to_edi_visual\>](./#to_edi_visual)
- [\<to_edi_visual_line\>](./#to_edi_visual_line)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)
- [\<unmap\>](./#unmap)
- [\<mapclear\>](./#mapclear)

<hr class="dash">

### **`<to_edi_visual>`**
Switch to editor visual mode.
The editor visual mode corresponds to the "v" command in Vim, which allows you to make a character-based selection with the keyboard. The typed keys are not propagated to Windows. To select a line, call [\<to_edi_visual_line\>](./#to_edi_visual_line) instead. In both ways, the only difference is the initialization, and the transition destination is the editor visual mode.

**See Also**
- [\<to_edi_normal\>](./#to_edi_normal)
- [\<to_edi_visual_line\>](./#to_edi_visual_line)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)
- [\<unmap\>](./#unmap)
- [\<mapclear\>](./#mapclear)

<hr class="dash">

### **`<to_edi_visual_line>`**
Switch to editor visual mode.
Similar to `<s-v>` of Vim, the selection method is line selection. If you want to do character selection, call [\<to_edi_visual\>](./#to_edi_visual) instead.

**See Also**
- [\<to_edi_normal\>](./#to_edi_normal)
- [\<to_edi_visual\>](./#to_edi_visual)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)
- [\<unmap\>](./#unmap)
- [\<mapclear\>](./#mapclear)

<hr class="dash">

### **`<to_insert>`**
Enters the Insert mode. In this mode, you can directly input and edit text.

**See Also**
- [\<to_resident\>](./#to_resident)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)
- [\<unmap\>](./#unmap)
- [\<mapclear\>](./#mapclear)

<hr class="dash">

### **`<to_resident>`**
Enters Resident mode.

**See Also**
- [\<to_insert\>](./#to_insert)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)
- [\<unmap\>](./#unmap)
- [\<mapclear\>](./#mapclear)

<hr class="dash">

### **`<to_instant_gui_normal>`**
Temporarily switches to GUI Normal mode and performs matching, which can be used as a map-leader.

**See Also**
- [\<to_gui_normal\>](./#to_gui_normal)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)
- [\<unmap\>](./#unmap)
- [\<mapclear\>](./#mapclear)


## System Command

### **`<set>`**
System Command set.

**See Also**
- [Options](../options)
- [\<source\>](./#source)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)

<hr class="dash">

### **`<source>`**
System Command source.

**See Also**
- [\<set\>](./#set)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)

<hr class="dash">

### **`<map>`**
System Command map.

 the `map` allows remapping with user-defined mapping like the following.
```vim
nmap f h  " f --> h
nmap t f  " t --> h
```
The `noremap` performs only the default map.
```vim
nnoremap f h  " f --> h
nnoremap t f  " t --> f
```

**See Also**
- [\<noremap\>](./#noremap)
- [\<unmap\>](./#unmap)
- [\<mapclear\>](./#mapclear)
- [\<source\>](./#source)

<hr class="dash">

### **`<noremap>`**
System Command noremap.

**See Also**
- [\<map\>](./#map)
- [\<unmap\>](./#unmap)
- [\<mapclear\>](./#mapclear)
- [\<source\>](./#source)

<hr class="dash">

### **`<unmap>`**
System Command unmap.

**See Also**
- [\<mapclear\>](./#mapclear)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)
- [\<source\>](./#source)

<hr class="dash">

### **`<mapclear>`**
System Command mapclear.

**See Also**
- [\<unmap\>](./#unmap)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)
- [\<source\>](./#source)

<hr class="dash">

### **`<command>`**
System Command command.

**See Also**
- [\<delcommand\>](./#delcommand)
- [\<comclear\>](./#comclear)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)

<hr class="dash">

### **`<delcommand>`**
System Command delcommand.

**See Also**
- [\<comclear\>](./#comclear)
- [\<command\>](./#command)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)

<hr class="dash">

### **`<comclear>`**
System Command comclear.

**See Also**
- [\<delcommand\>](./#delcommand)
- [\<command\>](./#command)
- [\<map\>](./#map)
- [\<noremap\>](./#noremap)

<hr class="dash">

### **`<autocmd_add>`**

**Syntax**
```vim
autocmd {event} {aupat} {cmd}
```

It adds `{cmd}` into autocmd list for `{aupat}`, autocmd pattern, corresponding to `{event}`.
As such as Vim, this function append `{cmd}` into a list rather than overwriting it even if the same `{cmd}` has already existed in a list.
The rule of `{aupat}` is based on the original Vim.
The registered `{cmd}`s will execute in the order added.

**Event**

The following table shows the supported events.
The string of each event is NOT case-sensitive.

|*Event*|*When does it ignite?*|
|:---|:---|
|AppEnter|Select an application|
|AppLeave|Unselect an application|
|GUINormalEnter|Enter to the GUI normal mode|
|GUINormalLeave|Leave from the GUI normal mode|
|GUIVisualEnter|Enter to the GUI visual mode|
|GUIVisualLeave|Leave from the GUI visual mode|
|EdiNormalEnter|Enter to the editor normal mode|
|EdiNormalLeave|Leave from the editor normal mode|
|EdiVisualEnter|Enter to the editor visual mode|
|EdiVisualLeave|Leave from the editor visual mode|
|InsertEnter|Enter to the insert mode|
|InsertLeave|Leave from the insert mode|
|ResidentEnter|Enter to the resident mode|
|ResidentLeave|Leave from the resident mode|
|CmdLineEnter|Enter to the command mode|
|CmdLineLeave|Leave from the command mode|

The event does not allow us to use `*`.
If you want to add a command to multiple events at the same time, `,` without after-space is available.

<br>

**Pattern**
If the pattern contains `/`, it matches the absolute path of the executable file of the application which creates each event.
If it does not contain `/`, it is compared against the name of the executable file.
The pattern is NOT case-sensitive.

|Pattern|Interpretation|
|:---|:---|
|`*`|Matches any character|
|`?`|Matches any single character|
|`\?`|Matches the `?` character|
|`.`|Matches the `.` character|
|`~`|Matches the `~` character|

<br>
All path delimiters `\` in Windows are treated as `/` in pattern translation.  
If you want to add a command to multiple patterns at the same time, `,` without after-space is available.
All others follow the general regex.

**Examples**

```vim
" Default mapping (match any applications)
autocmd AppLeave * <to_insert>

" Limited mapping (match specific application)
autocmd AppEnter *notepad* <to_edi_normal>
autocmd AppEnter,EdiNormalEnter vim.exe <to_resident>
autocmd AppEnter C:/*/Zotero/zotero.exe <to_edi_normal>
```

**See Also**
- [\<autocmd_del\>](./#autocmd_del)

<hr class="dash">


### **`<autocmd_del>`**
**Syntax**
```vim
autocmd! {event} {aupat} {cmd}
```

It remove all autocmd matched `{event}` and `{aupat}`, then register `{cmd}` after delete.

The following syntaxes are available.

```vim
autocmd! {event} {aupat} {cmd}
autocmd! {event} {aupat}
autocmd! * {aupat}
autocmd! {event}
```

Each features are the same as the original Vim.

**Examples**

```vim
autocmd! * *vim*  " Remove all events having the pattern *vim*
autocmd! AppLeave *notepad* <to_insert>  " Remove old events and add a new event
```

**See Also**
- [\<autocmd_add\>](./#autocmd_add)

## Mouse

### **`<click_left>`**
Left button of a mouse click.

**See Also**
- [\<click_right\>](./#click_right)
- [\<click_mid\>](./#click_mid)
- [\<easyclick\>](./#easyclick)
- [\<gridmove\>](./#gridmove)

<hr class="dash">

### **`<click_right>`**
Right button of a mouse click.

**See Also**
- [\<click_left\>](./#click_left)
- [\<click_mid\>](./#click_mid)
- [\<easyclick\>](./#easyclick)
- [\<gridmove\>](./#gridmove)

<hr class="dash">

### **`<click_mid>`**
Middle button of a mouse click.

**See Also**
- [\<click_left\>](./#click_left)
- [\<click_right\>](./#click_right)
- [\<easyclick\>](./#easyclick)
- [\<gridmove\>](./#gridmove)

<hr class="dash">

### **`<move_cursor_left>`**
Move the mouse cursor to the left.

**See Also**
- [\<move_cursor_right\>](./#move_cursor_right)
- [\<move_cursor_up\>](./#move_cursor_up)
- [\<move_cursor_down\>](./#move_cursor_down)

<hr class="dash">

### **`<move_cursor_right>`**
Move the mouse cursor to the right.

**See Also**
- [\<move_cursor_left\>](./#move_cursor_left)
- [\<move_cursor_up\>](./#move_cursor_up)
- [\<move_cursor_down\>](./#move_cursor_down)

<hr class="dash">

### **`<move_cursor_up>`**
Move the mouse cursor up.

**See Also**
- [\<move_cursor_left\>](./#move_cursor_left)
- [\<move_cursor_right\>](./#move_cursor_right)
- [\<move_cursor_down\>](./#move_cursor_down)

<hr class="dash">


### **`<move_cursor_down>`**
Move the mouse cursor down.

**See Also**
- [\<move_cursor_left\>](./#move_cursor_left)
- [\<move_cursor_right\>](./#move_cursor_right)
- [\<move_cursor_up\>](./#move_cursor_up)

<hr class="dash">

### **`<easyclick>`**
Move a cursor using hints on the UI objects without clicking.

> **Note:**
> In versions prior to 5.1.0, there where commands `<easy_click_left>`, `<easy_click_right>`, `<easy_click_mid>`, and `<easy_click_hover>`, which have been merged into `<easyclick>`. For compatibility, the previous commands are automatically replaced by the following.
> <table>
> <tr>
>   <th>Conventional Name</th>
>   <th>Automatically Replaced Name</th>
> </tr>
> <tr>
>   <td><code>&lt;easy_click_left&gt;</code></td>
>   <td><code>&lt;easyclick&gt;&lt;click_left&gt;</code></td>
> </tr>
> <tr>
>   <td><code>&lt;easy_click_right&gt;</code></td>
>   <td><code>&lt;easyclick&gt;&lt;click_right&gt;</code></td>
> </tr>
> <tr>
>   <td><code>&lt;easy_click_mid&gt;</code></td>
>   <td><code>&lt;easyclick&gt;&lt;click_mid&gt;</code></td>
> </tr>
> <tr>
>   <td><code>&lt;easy_click_hover&gt;</code></td>
>   <td><code>&lt;easyclick&gt;</code></td>
> </tr>
> </table>

<p align="center">
<img src="{{ site.url }}/imgs/easyclick.png" class="img-fluid">
<p align="center">Move a cursor using hints.</p>
</p>

**See Also**
- [\<gridmove\>](./#gridmove)
- [\<click_left\>](./#click_left)
- [\<click_right\>](./#click_right)
- [\<click_mid\>](./#click_mid)
- [\<jump_cursor_to_active_window\>](./#jump_cursor_to_active_window)
- [\<jump_cursor_with_keybrd_layout\>](./#jump_cursor_with_keybrd_layout)

<hr class="dash">

### **`<easyclick_all>`**
This function is the [\<easyclick\>](./#easyclick) to work on all visible applications even if they are not in focus.

**See Also**
- [\<easyclick\>](./#easyclick)
- [\<gridmove\>](./#gridmove)
- [\<click_left\>](./#click_left)
- [\<click_right\>](./#click_right)

<hr class="dash">

### **`<gridmove>`**
Move a cursor using tiled hints laid on the entire screen.
To change fonts or colors, you can set the several options, such as `gridmove_bgcolor`, `gridmove_fontcolor`, `gridmove_fontname`, `gridmove_fontsize`, and `gridmove_fontweight`.
In order to change the grid size, set the size with `gridmove_size` option. It assumes a text as its value, such as `12x8` for horizontal 12 cells and vertical 8 cells.

<p align="center">
<img src="{{ site.url }}/imgs/gridmove.png" class="img-fluid">
<p align="center">Move a cursor using tiled hints.</p>
</p>

**See Also**
- [\<easyclick\>](./#easyclick)
- [\<click_left\>](./#click_left)
- [\<click_right\>](./#click_right)
- [\<click_mid\>](./#click_mid)
- [\<jump_cursor_to_active_window\>](./#jump_cursor_to_active_window)
- [\<jump_cursor_with_keybrd_layout\>](./#jump_cursor_with_keybrd_layout)

<hr class="dash">


### **`<focus_textarea>`**
Select the text area closest to the cursor and move the mouse cursor over it.
If there are multiple text areas, the selection is based on the minimum Euclidean distance between the mouse cursor and the center point of the bounding box of the text area.

In the previous version of win-vind, this function was attached to the Editor Normal Mode as the `autofocus_textarea` option, but it is now independent. Currently, the `autofocus_textarea` option is deprecated. For compatibility, `autofocus_textarea` defines a mapping such as `autocmd EdiNormalEnter * <focus_textarea>`.

<p align="center">
<img src="{{ site.url }}/imgs/focus_textarea.png" class="img-fluid">
<p align="center">Focus the nearest text area and move the cursor.</p>
</p>

<hr class="dash">


### **`<jump_cursor_to_left>`**
Jump the mouse cursor to the left.

**See Also**
- [\<jump_cursor_to_right\>](./#jump_cursor_to_right)
- [\<jump_cursor_to_top\>](./#jump_cursor_to_top)
- [\<jump_cursor_to_bottom\>](./#jump_cursor_to_bottom)
- [\<jump_cursor_to_hcenter\>](./#jump_cursor_to_hcenter)
- [\<jump_cursor_to_vcenter\>](./#jump_cursor_to_vcenter)

<hr class="dash">

### **`<jump_cursor_to_right>`**
Jump the mouse cursor to the right.

**See Also**
- [\<jump_cursor_to_left\>](./#jump_cursor_to_left)
- [\<jump_cursor_to_top\>](./#jump_cursor_to_top)
- [\<jump_cursor_to_bottom\>](./#jump_cursor_to_bottom)
- [\<jump_cursor_to_hcenter\>](./#jump_cursor_to_hcenter)
- [\<jump_cursor_to_vcenter\>](./#jump_cursor_to_vcenter)

<hr class="dash">


### **`<jump_cursor_to_top>`**
Jump the mouse cursor to the top.

**See Also**
- [\<jump_cursor_to_left\>](./#jump_cursor_to_left)
- [\<jump_cursor_to_right\>](./#jump_cursor_to_right)
- [\<jump_cursor_to_bottom\>](./#jump_cursor_to_bottom)
- [\<jump_cursor_to_hcenter\>](./#jump_cursor_to_hcenter)
- [\<jump_cursor_to_vcenter\>](./#jump_cursor_to_vcenter)

<hr class="dash">

### **`<jump_cursor_to_bottom>`**
Jump the mouse cursor to the bottom.

**See Also**
- [\<jump_cursor_to_left\>](./#jump_cursor_to_left)
- [\<jump_cursor_to_right\>](./#jump_cursor_to_right)
- [\<jump_cursor_to_top\>](./#jump_cursor_to_top)
- [\<jump_cursor_to_hcenter\>](./#jump_cursor_to_hcenter)
- [\<jump_cursor_to_vcenter\>](./#jump_cursor_to_vcenter)

<hr class="dash">

### **`<jump_cursor_to_hcenter>`**
Jump the mouse cursor to the horizontal center.

**See Also**
- [\<jump_cursor_to_left\>](./#jump_cursor_to_left)
- [\<jump_cursor_to_right\>](./#jump_cursor_to_right)
- [\<jump_cursor_to_top\>](./#jump_cursor_to_top)
- [\<jump_cursor_to_bottom\>](./#jump_cursor_to_bottom)
- [\<jump_cursor_to_vcenter\>](./#jump_cursor_to_vcenter)

<hr class="dash">

### **`<jump_cursor_to_vcenter>`**
Jump the mouse cursor to the vertical center.

**See Also**
- [\<jump_cursor_to_left\>](./#jump_cursor_to_left)
- [\<jump_cursor_to_right\>](./#jump_cursor_to_right)
- [\<jump_cursor_to_top\>](./#jump_cursor_to_top)
- [\<jump_cursor_to_bottom\>](./#jump_cursor_to_bottom)
- [\<jump_cursor_to_hcenter\>](./#jump_cursor_to_hcenter)

<hr class="dash">

### **`<jump_cursor_to_active_window>`**
Jump the mouse cursor to the foreground window.

**See Also**
- [\<jump_cursor_with_keybrd_layout\>](./#jump_cursor_with_keybrd_layout)
- [\<easyclick\>](./#easyclick)
- [\<gridmove\>](./#gridmove)

<hr class="dash">

### **`<jump_cursor_with_keybrd_layout>`**
Jump the mouse cursor by keyboard mapping.

**See Also**
- [\<jump_cursor_to_active_window\>](./#jump_cursor_to_active_window)
- [\<easyclick\>](./#easyclick)
- [\<gridmove\>](./#gridmove)

<hr class="dash">

### **`<scroll_up>`**
Scroll the mouse wheel up.

**See Also**
- [\<scroll_up_onepage\>](./#scroll_up_onepage)
- [\<scroll_up_halfpage\>](./#scroll_up_halfpage)
- [\<scroll_down\>](./#scroll_down)

<hr class="dash">

### **`<scroll_up_halfpage>`**
Scroll the mouse wheel up with a half page.

**See Also**
- [\<scroll_up\>](./#scroll_up)
- [\<scroll_up_onepage\>](./#scroll_up_onepage)
- [\<scroll_down\>](./#scroll_down)

<hr class="dash">

### **`<scroll_up_onepage>`**
Scroll the mouse wheel up with a page.

**See Also**
- [\<scroll_up\>](./#scroll_up)
- [\<scroll_up_halfpage\>](./#scroll_up_halfpage)
- [\<scroll_down\>](./#scroll_down)

<hr class="dash">

### **`<scroll_down>`**
Scroll the mouse wheel down.

**See Also**
- [\<scroll_down_onepage\>](./#scroll_down_onepage)
- [\<scroll_down_halfpage\>](./#scroll_down_halfpage)
- [\<scroll_up\>](./#scroll_up)

<hr class="dash">

### **`<scroll_down_halfpage>`**
Scroll the mouse wheel down with a half page.

**See Also**
- [\<scroll_down\>](./#scroll_down)
- [\<scroll_down_onepage\>](./#scroll_down_onepage)
- [\<scroll_up\>](./#scroll_up)

<hr class="dash">

### **`<scroll_down_onepage>`**
Scroll the mouse wheel down with a page.

**See Also**
- [\<scroll_down\>](./#scroll_down)
- [\<scroll_down_halfpage\>](./#scroll_down_halfpage)
- [\<scroll_up\>](./#scroll_up)

<hr class="dash">

### **`<scroll_left>`**
Scroll the mouse wheel left.

**See Also**
- [\<scroll_left_halfpage\>](./#scroll_left_halfpage)
- [\<scroll_right\>](./#scroll_right)

<hr class="dash">

### **`<scroll_left_halfpage>`**
Scroll the mouse wheel left with a half page.

**See Also**
- [\<scroll_left\>](./#scroll_left)
- [\<scroll_right\>](./#scroll_right)

<hr class="dash">

### **`<scroll_right>`**
Scroll the mouse wheel right.

**See Also**
- [\<scroll_right_halfpage\>](./#scroll_right_halfpage)
- [\<scroll_left\>](./#scroll_left)

<hr class="dash">

### **`<scroll_right_halfpage>`**
Scroll the mouse wheel right with a half page.

**See Also**
- [\<scroll_right\>](./#scroll_right)
- [\<scroll_left\>](./#scroll_left)


## Window

### **`<window_resizer>`**
Start window resizer. It respects Vim plugin [simeji/winresizer](https://github.com/simeji/winresizer).

**See Also**
- [\<resize_window_width\>](./#resize_window_width)
- [\<increase_window_width\>](./#increase_window_width)
- [\<arrange_windows\>](./#arrange_windows)

<hr class="dash">

### **`<switch_window>`**
Switch a window.

**See Also**
- [\<window_resizer\>](./#window_resizer)
- [\<select_left_window\>](./#select_left_window)
- [\<select_upper_window\>](./#select_upper_window)

<hr class="dash">

### **`<select_left_window>`**
Select the left window.

**See Also**
- [\<select_right_window\>](./#select_right_window)
- [\<select_upper_window\>](./#select_upper_window)
- [\<select_lower_window\>](./#select_lower_window)
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">

### **`<select_right_window>`**
Select the right window.

**See Also**
- [\<select_left_window\>](./#select_left_window)
- [\<select_upper_window\>](./#select_upper_window)
- [\<select_lower_window\>](./#select_lower_window)
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">

### **`<select_upper_window>`**
Select the upper window.

**See Also**
- [\<select_lower_window\>](./#select_lower_window)
- [\<select_left_window\>](./#select_left_window)
- [\<select_right_window\>](./#select_right_window)
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">

### **`<select_lower_window>`**
Select the lower window.

**See Also**
- [\<select_upper_window\>](./#select_upper_window)
- [\<select_left_window\>](./#select_left_window)
- [\<select_right_window\>](./#select_right_window)
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">

### **`<move_window_left>`**
Moves the selected widow to the left.
The amount of window movement is based on the [window_velocity](../options/#window_velocity) parameter as in [window_resizer](./#window_resizer).
If a number is entered before the command, such as `20<c-w><c-h>`, the window will be moved by `20 * window_velocity`.

The window can only be moved as far as it is visible on the screen.
Therefore, excessive movement will cause the window to stop at the edge of the screen.
If you are using multiple displays, the movement range is determined by the combined resolution of all displays.

**See Also**
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">


### **`<move_window_right>`**
Moves the selected widow to the right.
The amount of window movement is based on the [window_velocity](../options/#window_velocity) parameter as in [window_resizer](./#window_resizer).
If a number is entered before the command, such as `20<c-w><c-l>`, the window will be moved by `20 * window_velocity`.

The window can only be moved as far as it is visible on the screen.
Therefore, excessive movement will cause the window to stop at the edge of the screen.
If you are using multiple displays, the movement range is determined by the combined resolution of all displays.

**See Also**
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">

### **`<move_window_up>`**
Moves the selected widow to the up.
The amount of window movement is based on the [window_velocity](../options/#window_velocity) parameter as in [window_resizer](./#window_resizer).
If a number is entered before the command, such as `20<c-w><c-k>`, the window will be moved by `20 * window_velocity`.

The window can only be moved as far as it is visible on the screen.
Therefore, excessive movement will cause the window to stop at the edge of the screen.
If you are using multiple displays, the movement range is determined by the combined resolution of all displays.

**See Also**
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">

### **`<move_window_down>`**
Moves the selected widow to the down.
The amount of window movement is based on the [window_velocity](../options/#window_velocity) parameter as in [window_resizer](./#window_resizer).
If a number is entered before the command, such as `20<c-w><c-j>`, the window will be moved by `20 * window_velocity`.

The window can only be moved as far as it is visible on the screen.
Therefore, excessive movement will cause the window to stop at the edge of the screen.
If you are using multiple displays, the movement range is determined by the combined resolution of all displays.

**See Also**
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">

### **`<maximize_current_window>`**
Maximize the current window.

**See Also**
- [\<minimize_current_window\>](./#minimize_current_window)
- [\<resize_window_width\>](./#resize_window_width)
- [\<resize_window_height\>](./#resize_window_height)
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">

### **`<minimize_current_window>`**
Minimize the current window.

**See Also**
- [\<maximize_current_window\>](./#maximize_current_window)
- [\<resize_window_width\>](./#resize_window_width)
- [\<resize_window_height\>](./#resize_window_height)
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">

### **`<resize_window_width>`**
Set the width of a window. You have to pass the pixel value as an argument using the command line.

**See Also**
- [\<resize_window_height\>](./#resize_window_height)
- [\<increase_window_width\>](./#increase_window_width)
- [\<decrease_window_width\>](./#decrease_window_width)
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">


### **`<increase_window_width>`**
Increase the width of a window.

**See Also**
- [\<decrease_window_width\>](./#decrease_window_width)
- [\<increase_window_height\>](./#increase_window_height)
- [\<decrease_window_height\>](./#decrease_window_height)
- [\<resize_window_width\>](./#resize_window_width)

<hr class="dash">


### **`<decrease_window_width>`**
Decrease the width of a window.

**See Also**
- [\<increase_window_width\>](./#increase_window_width)
- [\<increase_window_height\>](./#increase_window_height)
- [\<decrease_window_height\>](./#decrease_window_height)
- [\<resize_window_width\>](./#resize_window_width)

<hr class="dash">


### **`<resize_window_height>`**
Set the height of a window. You have to pass the pixel value as an argument using the command line.

**See Also**
- [\<resize_window_width\>](./#resize_window_width)
- [\<increase_window_height\>](./#increase_window_height)
- [\<decrease_window_height\>](./#decrease_window_height)
- [\<window_resizer\>](./#window_resizer)

<hr class="dash">


### **`<increase_window_height>`**
Increase the height of a window.

**See Also**
- [\<decrease_window_height\>](./#decrease_window_height)
- [\<increase_window_width\>](./#increase_window_width)
- [\<decrease_window_width\>](./#decrease_window_width)
- [\<resize_window_height\>](./#resize_window_height)

<hr class="dash">

### **`<decrease_window_height>`**
Decrease the height of a window.

**See Also**
- [\<increase_window_height\>](./#increase_window_height)
- [\<increase_window_width\>](./#increase_window_width)
- [\<decrease_window_width\>](./#decrease_window_width)
- [\<resize_window_height\>](./#resize_window_height)

<hr class="dash">

### **`<arrange_windows>`**
Arrange windows with tile style.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<exchange_window_with_nearest>`**
Exchange a window with the nearest window.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<rotate_windows>`**
Rotate windows in the current monitor.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<rotate_windows_in_reverse>`**
Rotate windows in the current monitor in reverse.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<snap_current_window_to_left>`**
Snap the current window to the left.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<snap_current_window_to_right>`**
Snap the current window to the right.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<snap_current_window_to_top>`**
Snap the current window to the top.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<snap_current_window_to_bottom>`**
Snap the current window to the bottom.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<open_new_window>`**
Open a new window.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<reload_current_window>`**
Reload the current window.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<open_new_window_with_hsplit>`**
Open a new window with a horizontal split.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<open_new_window_with_vsplit>`**
Open a new window with a vertical split.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<close_current_window>`**
Close the current window.

<!--
**See Also**
- [\<a\>](./#a)
-->


## Process

### **`<execute>`**
Open file with the associated application. This is a wrapper for the famous Windows API, **ShellExecute**, which behaves the same as double-clicking in Explorer. Therefore, you can open any format files and URLs. For example, `:e ~/.vimrc` or `:e https://www.google.com`. If there is no argument, it will open .vindrc loaded at initialization.



<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<exit>`**
Exit win-vind.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<sleep>`**
Sleep win-vind for N seconds.
As the same as Vim, this command is called with commands of the command mode or some bindings.
The duration of time to sleep is specified by the arguments of commands (e.g., `:sleep 10`) or the prefix number of bindings (e.g., `10gs`), as shown in the below examples.
When `m` is included, sleep for N milliseconds.
The default is one seconds.

**Example for command line**

```vim
:sleep       " sleep for one second
:sleep 5     " sleep for five seconds
:sleep 100m  " sleep for 100 milliseconds
```

**Example for .vindrc**

```vim
map <ctrl-1> :sleep 5<cr><easyclick>  " Launch easyclick after 5 seconds.
```


<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<start_external>`**
Start an external application. This environment variable is dependent on the application specified in the `shell` option. By appending `;` at the end, it keeps the console window without closing immediately. If the explorer is the foreground window, the current directory of a terminal will be that directory.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<start_shell>`**
Start a terminal. If the explorer is the foreground window, the current directory of a terminal will be that directory.

## Vim Emulation

### **`<to_insert_BOL>`**
**Vim Emulation:** `I`  
Insert to begin of line.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<to_insert_EOL>`**
**Vim Emulation:** `A`  
Append end of line.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<to_insert_append>`**
**Vim Emulation:** `a`  
Append after a caret.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<to_insert_nlabove>`**
**Vim Emulation:** `O`  
Begin new line above a caret.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<to_insert_nlbelow>`**
**Vim Emulation:** `o`  
Begin new line below a caret.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_caret_left>`**
**Vim Emulation:** `h`  
Move the caret to left.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_caret_down>`**
**Vim Emulation:** `j`  
Move the caret down.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_caret_up>`**
**Vim Emulation:** `k`  
Move the caret up.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_caret_right>`**
**Vim Emulation:** `l`  
Move the caret to right.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_fwd_word>`**
**Vim Emulation:** `w`  
Move words forward for normal mode.

It performs word-motion using an algorithm that is completely identical to Vim.
However, it is not available in visual mode, since the text is selected and copied once and retrieved via the clipboard for text parsing.
Instead, you can [move_fwd_word_simple](./#move_fwd_word_simple) for visual mode, which is faster and simpler (of course, it is also available for other modes)

The `iskeyword` option is fixed to the default value of Vim in Windows and cannot change it currently.
There is an option [charbreak](../options/#charbreak) to set the criteria for considering a Unicode character as a single character. 

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_fwd_word_simple>`**
**Vim Emulation:** `w`  
Move words forward fast.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_bck_word>`**
**Vim Emulation:** `b`  
Move words backward for normal mode.

It performs word-motion using an algorithm that is completely identical to Vim.
However, it is not available in visual mode, since the text is selected and copied once and retrieved via the clipboard for text parsing.
Instead, you can [move_bck_word_simple](./#move_fwd_word_simple) for visual mode, which is faster and simpler (of course, it is also available for other modes)

The `iskeyword` option is fixed to the default value of Vim in Windows and cannot change it currently.
There is an option [charbreak](../options/#charbreak) to set the criteria for considering a Unicode character as a single character. 

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_bck_word_simple>`**
**Vim Emulation:** `b`  
Move words backward fast.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_fwd_bigword>`**
**Vim Emulation:** `W`  
Move WORDS forward.

It performs WORD-motion using an algorithm that is completely identical to Vim.
However, it is not available in visual mode, since the text is selected and copied once and retrieved via the clipboard for text parsing.
Instead, you can [move_fwd_word_simple](./#move_fwd_word_simple) for visual mode, which is faster and simpler (of course, it is also available for other modes)

The `iskeyword` option is fixed to the default value of Vim in Windows and cannot change it currently.
There is an option [charbreak](../options/#charbreak) to set the criteria for considering a Unicode character as a single character. 

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_bck_bigword>`**
**Vim Emulation:** `B`  
Move WORDS backward.

It performs WORD-motion using an algorithm that is completely identical to Vim.
However, it is not available in visual mode, since the text is selected and copied once and retrieved via the clipboard for text parsing.
Instead, you can [move_bck_word_simple](./#move_fwd_word_simple) for visual mode, which is faster and simpler (of course, it is also available for other modes)

The `iskeyword` option is fixed to the default value of Vim in Windows and cannot change it currently.
There is an option [charbreak](../options/#charbreak) to set the criteria for considering a Unicode character as a single character. 

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_end_word>`**
**Vim Emulation:** `e`  
Forward to the end of words.

It performs word-motion using an algorithm that is completely identical to Vim.
However, it is not available in visual mode, since the text is selected and copied once and retrieved via the clipboard for text parsing.

The `iskeyword` option is fixed to the default value of Vim in Windows and cannot change it currently.
There is an option [charbreak](../options/#charbreak) to set the criteria for considering a Unicode character as a single character. 

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_end_bigword>`**
**Vim Emulation:** `E`  
Forward to the end of WORDS.

It performs WORD-motion using an algorithm that is completely identical to Vim.
However, it is not available in visual mode, since the text is selected and copied once and retrieved via the clipboard for text parsing.

The `iskeyword` option is fixed to the default value of Vim in Windows and cannot change it currently.
There is an option [charbreak](../options/#charbreak) to set the criteria for considering a Unicode character as a single character. 

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_bckend_word>`**
**Vim Emulation:** `ge`  
Backward to the end of words.

It performs word-motion using an algorithm that is completely identical to Vim.
However, it is not available in visual mode, since the text is selected and copied once and retrieved via the clipboard for text parsing.

The `iskeyword` option is fixed to the default value of Vim in Windows and cannot change it currently.
There is an option [charbreak](../options/#charbreak) to set the criteria for considering a Unicode character as a single character. 

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<move_bckend_bigword>`**
**Vim Emulation:** `gE`  
Backward to the end of WORDS.

It performs WORD-motion using an algorithm that is completely identical to Vim.
However, it is not available in visual mode, since the text is selected and copied once and retrieved via the clipboard for text parsing.

The `iskeyword` option is fixed to the default value of Vim in Windows and cannot change it currently.
There is an option [charbreak](../options/#charbreak) to set the criteria for considering a Unicode character as a single character. 

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<jump_caret_to_BOF>`**
**Vim Emulation:** `gg`  
Jump the caret to BOF.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<jump_caret_to_BOL>`**
**Vim Emulation:** `0`  
Jump the caret to begin of line.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<jump_caret_to_EOF>`**
**Vim Emulation:** `G`  
Jump the caret to EOF.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<jump_caret_to_EOL>`**
**Vim Emulation:** `$`  
Jump the caret to end of line.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<change_char>`**
**Vim Emulation:** `s`  
Change Characters.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<change_highlight_text>`**
**Vim Emulation:** `c`, `s`, `S`  
Change highlighted texts.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<change_line>`**
**Vim Emulation:** `cc`, `S`  
Change Lines.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<change_until_EOL>`**
**Vim Emulation:** `C`  
Change until EOL.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<change_with_motion>`**
**Vim Emulation:** `c{motion}`  
Change texts with motion.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<delete_after>`**
**Vim Emulation:** `x`  
Delete chars after the caret.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<delete_before>`**
**Vim Emulation:** `X`  
Delete chars before the caret.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<delete_highlight_text>`**
**Vim Emulation:** `d`, `x`, `X`  
Delete highlighted texts.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<delete_line>`**
**Vim Emulation:** `dd`  
Delete lines.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<delete_line_until_EOL>`**
**Vim Emulation:** `D`  
Delete texts until end of line.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<delete_with_motion>`**
**Vim Emulation:** `d{motion}`  
Delete texts with motion.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<join_next_line>`**
**Vim Emulation:** `J`  
Join a next line.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<put_after>`**
**Vim Emulation:** `p`  
Put texts after the caret.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<put_before>`**
**Vim Emulation:** `P`  
Put texts before the caret.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<repeat_last_change>`**
**Vim Emulation:** `.`  
Repeat last simple change.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<replace_char>`**
**Vim Emulation:** `r`  
Replace a char.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<replace_sequence>`**
**Vim Emulation:** `R`  
Replace Mode.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<switch_char_case>`**
**Vim Emulation:** `~`  
Switch char case.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<yank_highlight_text>`**
**Vim Emulation:** `y`  
Yank highlighted texts.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<yank_line>`**
**Vim Emulation:** `yy`, `Y`  
Yank lines.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<yank_with_motion>`**
**Vim Emulation:** `y{motion}`  
Yank lines with motion.

<!--
**See Also**
- [\<a\>](./#a)
-->

## Hotkey

### **`<backward_ui_navigation>`**
Backward UI Navigation.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<decide_focused_ui_object>`**
Decide a focused UI object.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<forward_ui_navigation>`**
Forward UI Navigation.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<goto_next_page>`**
Forward to the next page.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<goto_prev_page>`**
Go backward to the previous page.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<hotkey_backspace>`**
Backspace.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<hotkey_copy>`**
Copy.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<hotkey_cut>`**
Cut.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<hotkey_delete>`**
Delete.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<hotkey_paste>`**
Paste.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<open>`**
Open another file.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<open_startmenu>`**
Open the Start Menu.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<redo>`**
Redo.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<save>`**
Save the current file.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<search_pattern>`**
Search Pattern.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<select_all>`**
Select all.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<start_explorer>`**
Start Explorer.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<undo>`**
Undo.

## Virtual Desktop


### **`<close_current_vdesktop>`**
Close a current virtual desktop.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<create_new_vdesktop>`**
Create a new virtual desktop.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<switch_to_left_vdesktop>`**
Switch to a left virtual desktop.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<switch_to_right_vdesktop>`**
Switch to a right virtual desktop.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<taskview>`**
Task View.

## Tab

### **`<close_current_tab>`**
Close a current tab.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<open_new_tab>`**
Open a new tab.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<switch_to_left_tab>`**
Switch to a left tab.

<!--
**See Also**
- [\<a\>](./#a)
-->

<hr class="dash">

### **`<switch_to_right_tab>`**
Switch to a right tab.

<!--
**See Also**
- [\<a\>](./#a)
-->


## File

### **`<makedir>`**
Create a directory. If you call it with a relative path such as `:mkdir foo`, it creates it in the explorer directory. If no explorer is found, it creates it in `~/Desktop/foo`. If you call it with an absolute path like `:mkdir C:/Users/You/Desktop/bar`, it will create a directory along the path recursively.

<!--
**See Also**
- [\<a\>](./#a)
-->


<br>
<br>
<br>
<br>
