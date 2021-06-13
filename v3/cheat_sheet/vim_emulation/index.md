---
layout: default
title: Vim Emulation
nav_order: 8
parent: Cheat Sheet
descripption: "Vim Emulation"
---

# Vim Emulation  
{: .no_toc}

<hr>

1. TOC  
{:toc}  

<hr>  

## Move Caret  

If the key-stroke occurs infinity repeating by un-revealed bugs of win-vind, please press the same key to create a releasing event.  

### Move Caret Left  

|Details|Status|  
|:---|:---|  
|ID|**edi_move_caret_left**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`h`, `<C-h>`, `<BS>`, `<Left>`|  
|Default Command|-|

It is the same as **h** on the original Vim.  

<br>  


### Move Caret Right  

|Details|Status|  
|:---|:---|  
|ID|**edi_move_caret_right**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`l`, `<Space>`, `<Right>`|  
|Default Command|-|

It is the same as **l** on the original Vim.  

<br>  

### Move Caret Up  

|Details|Status|  
|:---|:---|  
|ID|**edi_move_caret_up**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`k`, `-`, `gk`, `<C-p>`, `<Up>`|  
|Default Command|`:-<num>`|

It is the same as **k** on the original Vim. If you call with `:-<num>`, for example, **:-5**, it makes **k**->**k**->**k**->**k**->**k**. If you inputs **:-123abc**, they are understood as **:-123**.  

<br>  

### Move Caret Down  

|Details|Status|  
|:---|:---|  
|ID|**edi_move_caret_down**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`j`, `+`, `gj`, `<C-n>`, `<Down>`, `<Enter>`, `<C-m>`|  
|Default Command|`:+<num>`|

It is the same as **j** on the original Vim. If you call with `:+<num>`, for example, **:+3**, it makes **j**->**j**->**j**.  

<br>  

### Move Caret words Forawrd  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_move_caret_words_forward**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`w`|  
|Default Command|-|

It is the same as **Ctrl + Right** of shortcuts on Windows. Currently, there are not differences between **words** and **WORDS**.  

<br>  

### Move Caret words Backward

|Details|Status|  
|:---|:---|  
|ID|**edi_n_move_caret_words_backword**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`b`|  
|Default Command|-|

It is the same as **Ctrl + Left** of shortcuts on Windows. Currently, there are not differences between **words** and **WORDS**.  

<br>  

### Move Caret WORDS Forward  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_move_caret_WORDS_forward**|
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`W`|  
|Default Command|-|

It is the same as **Ctrl + Right** of shortcuts on Windows. Currently, there are not differences between **words** and **WORDS**.  

<br>  

### Move Caret WORDS Backward  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_move_caret_WORDS_backward**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`B`|  
|Default Command|-|

It is the same as **Ctrl + Left** of shortcuts on Windows. Currently, there are not differences between **words** and **WORDS**.  

<br>  

<hr>  

## Jump Caret  

### Jump Caret to BOL  

|Details|Status|  
|:---|:---|  
|ID|**edi_jump_caret_to_BOL**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`0`, `<Home>`, `g0`|  
|Default Command|-|

It is the same as **Home** of shortcuts on Windows.  

<br>  


### Jump Caret to EOL  

|Details|Status|  
|:---|:---|  
|ID|**edi_jump_caret_to_EOL**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`$`, `<End>`, `g$`|  
|Default Command|-|

It makes the caret jump before **End Of Line**.  

```cpp
//Example

//Before
Vim is the be|st editor.

//After
Vim is the best editor|.
```

<br>  

### Jump Caret to N-Line (Default: BOF)  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_jump_caret_to_line_default_BOF**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`gg`|  
|Default Command|`:<num>`|

It is the same as **gg** on the original Vim. If **N** is one or is not inputted, it makes jump the caret to **Begining Of File**. If inputted than two, it makes it to **N** line from the header.  

<br>  

### Jump Caret to N-Line (Default: EOF)  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_jump_caret_to_line_default_EOF**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`G`|  
|Default Command|-|

It is the same as **G** on the original Vim. The behavior of N-repeat is connected with **edi_n_jump_caret_to_line_default_BOF** one.  

<br>  

<hr>  

## Yank  

### Copy Highlight Text  

|Details|Status|  
|:---|:---|  
|ID|**edi_copy_highlight_text**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`y`|  
|Default Command|-|

It is the same as **y** under Visual Mode of the original Vim. It is assumed that the targeting texts are selected, so we recommend to use under Visual Mode.  

<br>  

### Copy Line  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_copy_line**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`yy`, `Y`|  
|Default Command|-|

It is the same as **yy** on the original Vim.  

<br>  

### Copy with Motion  

|Details|Status|  
|:---|:---|  
|ID|**edi_copy_motion**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`y{Motion}`|  
|Default Command|-|

It copies the text with selections. After calling it, it makes a message loop for waiting for input. If you type some keys which are not for moving the caret, its loop breaks and it does nothing. Currently, it is not supported like **y2w**, but the patterns like **2yw** are supported.  

<br>  

<hr>

## Put  
### Paste After 

|Details|Status|  
|:---|:---|  
|ID|**edi_n_paste_after**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`p`|  
|Default Command|-|

It pastes the copied texts after the caret or above the current line. If the copied texts by **Editor Visual Mode**, it regards them as **Characters** and just presses **Ctrl + V**. If you copied with **Editor Line Visual** or **yy**, it regards them as **Lines** and it pastes with keeping the layout of lines. However, it wouldn't work well in the case that its texts were copied by the real mouse.  

<br>  


### Paste Before  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_paste_before**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`P`|  
|Default Command|-|

It pastes the copied texts before the caret or below the current line. If the copied texts by **Editor Visual Mode**, it regards them as **Characters** and just presses **Ctrl + V**. If you copied with **Editor Line Visual** or **yy**, it regards them as **Lines** and it pastes with keeping the layout of lines. However, it wouldn't work well in the case that its texts were copied by the real mouse.  

<br>  

<hr> 

## Delete  
### Delete Highlight Text 

|Details|Status|  
|:---|:---|  
|ID|**edi_delete_highlight_text**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`d`|  
|Default Command|-|

It is the same as **d** under Visual Mode of the original Vim. It is assumed that the targeting texts are selected, so we recommend to use under Visual Mode.  

<br>  


### Delete Line  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_delete_line**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`dd`|  
|Default Command|-|

It is the same as **dd** on the original Vim.  

<br>  


### Delete Line until EOL  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_delete_line_until_EOL**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`D`|  
|Default Command|-|

It deletes the texts from the current position of the caret until **End Of Line** with copying.  

<br>  

### Delete After  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_delete_after**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`x`, `<Del>`|  
|Default Command|-|

It deletes characters from the current position for **End Of Line**. You can choose whether it copys its characters by switching the <a href="https://pit-ray.github.io/win-vind/cheat_sheet/options/#enable-char-cache">Enable Char Cache</a> option.  

<br>  

### Delete Before  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_delete_before**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`X`|  
|Default Command|-|

It deletes characters from the current position for **Begining Of Line**. You can choose whether it copys its characters by switching the <a href="https://pit-ray.github.io/win-vind/cheat_sheet/options/#enable-char-cache">Enable Char Cache</a> option.  

<br>  

### Delete with Motion  

|Details|Status|  
|:---|:---|  
|ID|**edi_delete_motion**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`d{Motion}`|  
|Default Command|-|

Its behavior is the same as <a href="https://pit-ray.github.io/win-vind/cheat_sheet/vim_emulation/#copy-with-motion">Copy With Motion</a>.  

<br>  

### Delete with Motion And Insert  

|Details|Status|  
|:---|:---|  
|ID|**edi_delete_motion_and_start_insert**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`c{Motion}`|  
|Default Command|-|

It changes the mode to **Editor Insert Mode** after calling <a href="https://pit-ray.github.io/win-vind/cheat_sheet/vim_emulation/#copy-with-motion">Delete With Motion</a>.  

<br>  

### Delete Lines and Insert  

|Details|Status|  
|:---|:---|  
|ID|**edi_delete_lines_and_start_insert**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`cc`, `S`|  
|Default Command|-|

It deletes lines with keeping indents and changes the mode to **Editor Insert Mode**.

```cpp
//Example
// | is a position of the caret.

//Before
//Pattern1)
Vim is the b|est editor.

//Pattern2)
    Vim is the b|est editor


//After
//Pattern1)
|

//Pattern2)
    |
```

<br>  


### Delete Chars and Insert  

|Details|Status|  
|:---|:---|  
|ID|**edi_delete_chars_and_start_insert**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`s`|  
|Default Command|-|

It deletes characters and changes the mode to **Editor Insert Mode**.  

<br>


### Delete Line until EOL and Insert  

|Details|Status|  
|:---|:---|  
|ID|**edi_delete_until_eol_and_start_insert**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`C`|  
|Default Command|-|

It deletes texts from the current position to **End Of Line**.  

<hr>  


## Replace  

### Replace Char  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_replace_char**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`r`|  
|Default Command|-|

After calling it, it makes a message loop for waiting input. It is the same as **r** on the original Vim.  

<br>  

### Replace Sequence

|Details|Status|  
|:---|:---|  
|ID|**edi_n_replace_sequence**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`R`|  
|Default Command|-|

After calling it, it makes a message loop for waiting input. It is the same as **R** on the original Vim.  

<br>  


### Switch Case of Chararacter  

|Details|Status|  
|:---|:---|  
|ID|**edi_switch_char_case**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`~`|  
|Default Command|-|

It is the same as **~** of the original Vim.  

<br>  

<hr>  


## Layout  
### Remove EOL  

|Details|Status|  
|:---|:---|  
|ID|**edi_n_remove_EOL**|  
|N-Repeat|<span class="yes">Supported</span>|  
|Default Key|`J`|  
|Default Command|-|

It is the same as **J** on the original Vim.  

<br>  
