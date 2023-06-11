---
layout: notoc
show_in_menu: false
disable_anchors: true
title: Default Mappings - win-vind
nav: Default Mappings
---

win-vind has many features, but you can use a simpler one for your purposes by putting the `version` command at the beginning of your .vindrc.
If you want to use tiny version, write the following. It is important to note that **nothing but comments can be written before the `version` command**.

```vim
" Only comments can be written in here.
version tiny
" Any command can be written from.
" For example
set shell = cmd
```
  
|**Tier**|**Supported Features**|
|:---|:---|
|tiny|+mouse +syscmd|
|small|+mouse +syscmd +window +process|
|normal|+mouse +syscmd +window +process +vimemu|
|big|+mouse +syscmd +window +process +vimemu +hotkey +gvmode|
|huge|+mouse +syscmd +window +process +vimemu +hotkey +gvmode +experimental|

<br>
Following Vim, there are five tiers. `tiny` has minimal commands for mouse moving and clicking, including EasyClick and GridMove. `small` allows more flexible handling of window controls and process launches, etc. `normal` has Vim emulation mappings and allows text editing in text areas, etc. `big` adds several hotkeys that redefine some of the shortcut keys in Windows to operate Windows with more Vim-like ways. It also provides GUI Visual Mode (+gvmode), which allows for holding down the mouse. `huge` allows win-vind to have experimental features for more complex operations. The default tier is `big`.

For more information on default mappings, please refer to the following links.

- [tiny](tiny)
- [small](small)
- [normal](normal)
- [big](big)
- [huge](huge)
