---
layout: default
title: Old FAQ
nav_order: 12
parent: v3 Docs
descripption: "frequently asked questions and these answers."
---
# Old FAQ
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

## Is it possible to start the program in GUI Insert instead of GUI Normal?  
Yes. Please set **GUI Insert** at **Preferences/settings/Initial Mode**.  
![initmode](https://user-images.githubusercontent.com/42631247/113421839-b443f800-9406-11eb-9c32-df9f4dfd66f8.jpg)

link: <a href="https://github.com/pit-ray/win-vind/issues/14">GitHub Issues #14</a>  
<br>
<hr> 

## Old Usage hinting like Vimium?  
In default, you can call the hinting feature with `FF`, which you can try after the first launch of win-vind. If you **only** want to use the hinting function, you can easily do it by following the steps.  

1. Open the key-config file, which locates **~/.win-vind/bindings.json** by installer or **config/bindings.json** by zip.  
1. In Vim, you can jump to the line by `/easy_click` command.  
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
   Please refer to <a href="https://pit-ray.github.io/win-vind/how_to_use/#key-config">How to Use/Customize/Key Config</a>.  
1. Input `:set` command, or right-click on the tray and select **Preference**.  
1. **Settings/Common/Initial Mode** -> **GUI Insert** and [   OK   ].

<br>
<hr> 

## Is there a way to customize layers of mode?
Yes.

<img src="https://github.com/pit-ray/pit-ray.github.io/raw/master/win-vind/imgs/mode_overview_2.jpg?raw=true" />

This model describes just the default, so you can customize the layer.

All mode state transitions exist as functions and each function has bindings in each mode. <a href="https://pit-ray.github.io/win-vind/cheat_sheet/mode_ctrl/">Old Cheet Sheet/Mode Control</a>

**Method**
- Use GUI
1. Show **Preferences/Bindings**.
1. Search `change_to_edi_insert` and push `Edit With Vim`

- Use your editors
1. Open `~/.win-vind/bindings.json` with your editor.
1. If you use Vim, search the line by `/change_to_edi_insert`.

Then, you can see it.
```json
    {
        "name": "change_to_edi_insert",
        "guin": [],
        "guii": [],
        "guiv": [],
        "edin": ["i"],
        "edii": [],
        "ediv": [],
        "edivl": [],
        "cmd": [],
        "mycwn": [],
        "mycwi": [],
        "en": "Insert before a caret (Vim Emulation: i)",
        "ja": "キャレットの前に挿入 (Vimエミュレーション: i)"
    },
```

"`edin`": ["`i`"], means,
"`If the current mode is this`": ["`the function is called by this bindings`"]

Thus, if you want to call `change_to_edi_insert` in **GUI Insert** with `<alt-i>`.
```json
    {
        "name": "change_to_edi_insert",
        "guin": [],
        "guii": ["<alt-i>"],  <----THIS
        "guiv": [],
        "edin": ["i"],
        "edii": [],
        "ediv": [],
        "edivl": [],
        "cmd": [],
        "mycwn": [],
        "mycwi": [],
        "en": "Insert before a caret (Vim Emulation: i)",
        "ja": "キャレットの前に挿入 (Vimエミュレーション: i)"
    },
```

And, you can refer to the keywords in <a href="https://pit-ray.github.io/win-vind/cheat_sheet/keyword_lists/">Old Cheet Sheet/Keyword List</a>

link: <a href="https://github.com/pit-ray/win-vind/issues/15">GitHub Issues #15</a>  

<br>
<hr> 
