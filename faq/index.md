---
layout: default
title: FAQ
nav_order: 12
descripption: "frequently asked questions and these answers."
---
# FAQ
{: .no_toc }  
If you have some questions, please send me.

1. TOC  
{:toc}  

<hr>

## Why is the name win-vind?  
win-vind means for **win**dows **vi**m key b**ind**er.  
<br>
<hr>

## Why is `o` a right click and `a` is a left click?  
I regarded original-Vim's operation for changing from Normal to Insert (e.g. `i`, `o`, `a`) as the aiming the caret position. Therefore I adopted these commands for deciding mouse positions. Additionaly, `o` is also adopted as the decide key in my favorite Vim plugins like **NERDTree**.  
<br>
<hr> 

## What is the GUI visual mode?  
It is the same as holding the mouse left pressing.  
<br>
<hr> 

## I want to use **Alt** + **Tab**.
Please use <a href="https://pit-ray.github.io/win-vind/cheat_sheet/window_ctrl/#switch-window">Switch Window</a>.  
<br>
<hr> 

## Would it be possible to start without key-bindings and be turn on win-vind whenever my wants?
Yes. You should set **GUI Insert** at **Preferences/settings/Initial Mode**.  
<br>
<hr> 

## How to use hinting like Vimium?  
In default, you can call the hinting feature with `F` (in key, `Shift` + `F`), which you can try after the first launch of win-vind. If you **only** want to use the hinting function, you can easily do it by following the steps.  

1. Open the key-config file, which locates **~/.win-vind/bindings.json** by installer or **config/bindings.json** by zip.  
1. Jump to line 1396. In Vim, you can jump to the line by `/easy_click` command.  
1. Rewrite the value of **guii**(**GUI Insert Mode**).  In the following example, we can call the feature with **Right Alt -> Right Ctrl**.  <br>  
   ```json
    {
        "name": "easy_click",
        "guin": ["F"],
        "guii": ["<ralt><rctrl>"],
        "guiv": [],
        "edin": [],
        "edii": [],
        "ediv": [],
        "edivl": [],
        "cmd": ["ec"],
        "en": "EasyClick",
        "jp": "EasyClick"
    },
   ```
   Please refer to <a href="https://pit-ray.github.io/win-vind/how_to_use/#key-config">How to Use/Customization/Key Config</a>.  
1. Input `:set` command, or right-click on the tray and select **Preference**.  
1. **Settings/Common/Initial Mode** -> **GUI Insert** and [   OK   ].

