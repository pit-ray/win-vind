---
layout: page
disable_anchors: true
title: Migration Guide
nav: Migration
order: 3
icon: shoe-prints
---

## from <= 4.4.0 to 5.0.0

> **Where v4 documents go?**
> To keep simplicity, the document pages describes only for latest version.
> Therefore you cannot read web page style documents of conventional win-vind, but you can read old documents in markdown preview style in GitHub.  
> For v4.3.3, please refer to [this documents](https://github.com/pit-ray/win-vind/blob/v4.3.3/docs/cheat_sheet/index.md).

### 1. Syntax of .vindrc
**Discussed in [#96](https://github.com/pit-ray/win-vind/discussions/96)**
#### The difference between map and noremap
The conventional `map` and `noremap` have different purposes. The map is designed to propagate defined macros to other applications except for win-vind, whereas the noremap effects in win-vind score only.

However, the **NEW** `map` and the **NEW** `noremap` have similar features of Vim and are separated on whether allow remapping like Vim.

Specifically, the `map` allows remapping with user-defined mapping like the following.
```vim
nmap f h  " f --> h
nmap t f  " t --> h
```
The noremap performs only the default map.
```vim
nnoremap f h  " f --> h
nnoremap t f  " t --> f
```

#### Arguments of map/noremap
The command kind is the same, but the way to interpret the arguments.

|**Command** | **Syntax**|
|:--- |:---|
|noremap|`noremap [trigger-cmd] [target-cmd]`|
|map|`map [trigger-cmd] [target-cmd]`|
|unmap|`unmap [trigger-cmd]`|
|mapclear|`mapclear`|
|command|`command [trigger-cmd] [targer-cmd]`|
|delcommand|`delcommand [trigger-cmd] [target-cmd]`|
|comclear|`comclear`|

Note: The syntax is not included `[` and `]`.

The **[trigger-cmd]** is assumed to key typing only and the **[target-cmd]** has three types of intepretation.

|**Type of [target-cmd]**|**Example**|**Notes**|
|:---|:---|:---|
|Function Name|`map FF <easy_click_left>`|Calls the pre-defined functions.|
|Internal Macro|`map XX a<ctrl-f>bcd`|Generate macros inside the internal scope of win-vind. This feature uses to define some shortcuts to a function or some combined mapping consisting of multiple pre-defined functions.|
|External Macro|`map g {This text is inserted}`|Define macros that are propagated outside of win-vind by enclosing them in `{` and `}`. This emulates the action of the user pressing the keyboard itself, and a single key to single key mapping (e.g. `map a {b}`) is the most efficient low-level mapping done.|

These **[target-cmd]** can be incorporated into a single map as follows.
```vim
map g <easy_click_left>b{This text is inserted}<switch_window>hh<cr>
```
The mapping represents a macro that is triggered by `g`, activates easy_click, jumps to the position of the hint in `b`, enters the string "This text is inserted", and then selects the two-left window with switch_window.

This version will preform an optimization process that merges several maps into one map, unless it contains a command to change the mode. For example, the following mappings will be merged into one.

* Raw map
```vim
nmap b h
nmap o b
nmap p o
````
* Optimized map (**THIS VERSION**)
```vim
nmap p h
```

Below are some examples of use.
1. Define mode change mapping
   ```vim
   imap <win-[> <to_edi_normal>
   imap <win-]> <to_gui_normal>
   ``` 
1. Text input macros
   ```vim
   nmap mail {pit-ray@example.com}
   ```
1. Web page launcher
   ```vim
   nmap <ctrl-1> :execute https://example.com<cr>
   ```
1. Application launcher
   ```vim
   nmap <ctrl-2> :! notepad<cr>
   ```
1. Copy the current line to the bottom line as in Vim.
   ```vim
   enmap t yyGp
   ```

#### Mode Prefix
**Mentioned in [#91](https://github.com/pit-ray/win-vind/issues/91)**  

We received many requests to register maps across several modes, so we added batch-mapping with the same grouping as in Vim. 

You can mode prefix to specify modes.

|**Prefix**|**Mode**|
|:---:|:---|
| ` ` |GUI Normal, GUI Visual, Edi Normal, Edi Visual|
|`g` | GUI Normal, GUI Visual|
|`e` | Edi Normal, Edi Visual|
|`n` | GUI Normal, Edi Normal|
|`v` | GUI Visual, Edi Visual|
|`gn` | GUI Normal|
|`gv` | GUI Visual|
|`en` | Edi Normal|
|`ev` | Edi Visual|
|`i` | Insert Mode|
|`r` | Resident Mode|
|`c` | Command Mode|

However, external macros in `cmap` and `cnoremap` are not input to other applications and behave the same as internal macros.

### 2. Self-Mapping
**Discussed in [#123](https://github.com/pit-ray/win-vind/issues/123)**  

You can disable the absorption of some keys and allow them to be input, as in `map <alt> {<alt>}`. However, this is only valid for a single key.
If the target command consists of multiple characters like `map g abcgd` and contains a trigger command, the following warning statement will be printed to log and no mapping will be done.

```

[Warning] Some part of the command generated from mapping `g * :e https://google.com<return>` was ignored to avoid an infinite loop because it was mapped to itself by mapping `g * :e https://google.com<return>`. If you wish to enter the generated command as is, enclose it in `{}`.

```

### 3. Changed default mapping
**Discussed in [#118](https://github.com/pit-ray/win-vind/discussions/118)**  

Since the mode transition combined with ESC in win-vind was not well received, we adopted the same command as in Vim.

|**Type of map**|**Function ID** |**Conventional trigger of map** |**New trigger of map**|
|:---:|:---:|:---:|:---:|
|imap | `to_gui_normal` | `<Esc-Left>` | `<Ctrl-]>`|
|imap | `to_edi_normal` | `<Esc-Right>` | `<Ctrl-[>`|


### 4. Renamed  function name
The conventional `<syscmd_*>` function names are renamed to [simple ones](https://github.com/pit-ray/win-vind/blob/9ec52bb02b2e74784dd347ce259abb936b28d9fe/src/bind/mapdefault.cpp#L466-L522).

### 5. Eliminated options and replacements
The following is the correspondence between the options that were removed and their replacements. The `-` is completely obsolete.

|**Eliminated options** | **Replacements**|
|:---: | :---:|
|`window_accel` | `window_velocity`|
|`window_tweight` | `window_velocity`|
|`window_maxv` | `window_velocity`|
|`cursor_tweight` | `cursor_resolution`|
|`cursor_maxv` | `-`|
|`cmd_maxchar` | `-`|
|`cmd_maxhist` | `-`|

Details of the new options are as follows.

|**New options**|**Notes**|
|:---:|:---|
|`window_velocity` | Pixel-level velocity in the constatnt acceleration motion of the window in winresizer.|
|`cursor_resolution` | A weight for scaling the time of constant acceleration motion of the mouse cursor.|
|`listen_interval`|The time interval in seconds at which the server win-vind will retrieve command requests sent by the client with the `-c` argument in terminal. ([#112](https://github.com/pit-ray/win-vind/issues/112))|


### 6. New word-motion

Add the following word-motion which behave almost exactly like Vim. ([#57](https://github.com/pit-ray/win-vind/issues/57), [#75](https://github.com/pit-ray/win-vind/pull/75))

  |**ID**|**Feature**|**Emulation**|
  |:---:|:---|:---:|
  |**move_fwd_word**|words forward for normal mode.|`w`|
  |**move_fwd_word_simple**|words forward fast.|`w`|
  |**move_bck_word**|words backward for normal mode.|`b`|
  |**move_bck_word_simple**|words backward fast.|`b`|
  |**move_fwd_bigword**|WORDS forward.|`W`|
  |**move_bck_bigword**|WORDS backward.|`B`|
  |**move_end_word**|Forward to the end of words.|`e`|
  |**move_end_bigword**|Forward to the end of WORDS.|`E`|
  |**move_bckend_word**|Backward to the end of words.|`ge`|
  |**move_bckend_bigword**|Backward to the end of WORDS.|`gE`|

  These functions do not work in visual mode except for `w` and `b`, because they copy the text once and retrieve the text via the clipboard. `iskeyword` option is fixed to the default value of Vim in Windows and cannot change it currently.  
  There is an option `charbreak` to set the criteria for considering a Unicode character as a single character. 

  |ID|Type|Default|Note|
  |:---:|:---:|:---:|:---|
  |`charbreak`|str|grapheme|Mode for how to split a single Unicode character. The `grapheme` mode treats a combination character as a single character. The `codepoint` mode processes the combination character for each codepoint.|


### 7. New option in terminal
**Discussed in [#101](https://github.com/pit-ray/win-vind/discussions/101), [#97](https://github.com/pit-ray/win-vind/discussions/97)**  
Please see [this document]({{ site.url }}/usage/#use-as-automation-command-by-server-client).

