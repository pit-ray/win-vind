---
layout: default
title: How to use
nav_order: 3
descripption: "Usage of win-vind"
---
# How to use  
{: .no_toc }

This software supports only on Windows 10. Some features may be not working well on the others. If it is unstable despite Windows 10, please post issues to <a href="https://github.com/pit-ray/win-vind/issues">github.com/issues</a>.  

<hr>  

1. TOC  
{:toc}

<hr>

## Installation
1. Download the latest version by following a button. These links will probably be received some warnings. Today, it is very expensive to do code-signing to our application, so it is unavoidable as free software. <br>   

   ### 32 bit
   [Download .exe (32bit)](https://github.com/pit-ray/win-vind/releases/download/v3.1.0/setup_win-vind_3.1.0_32bit.exe.zip){: .btn }&nbsp;&nbsp;&nbsp;&nbsp;[Download .zip (32bit)](https://github.com/pit-ray/win-vind/releases/download/v3.1.0/win-vind_3.1.0_32bit.zip){: .btn}  

   ### 64 bit  
   [Download .exe (64bit)](https://github.com/pit-ray/win-vind/releases/download/v3.1.0/setup_win-vind_3.1.0_64bit.exe.zip){: .btn }&nbsp;&nbsp;&nbsp;&nbsp;[Download .zip (64bit)](https://github.com/pit-ray/win-vind/releases/download/v3.1.0/win-vind_3.1.0_64bit.zip){: .btn}  

1. Execute a downloaded installer or unzip a downloaded zip-file. 

1. Start **win-vind.exe**.

1. You can set it up by typing `:set` or `:config` or right-click the icon of win-vind on the system tray and select **Preferences**.  

1. In the end, you can terminate win-vind by `:exit` or right-click on the system tray and select **exit**.

<hr>

## Overview  
![mode-overview](https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/mode_overview_2.jpg?raw=true)  
win-vind has plenty of hotkeys and commands, but they are almost based on the original Vim or some famous plugins. The little differences are that its mode has two layers and its unique functions for GUI. Concretely, two layer modes are **GUI Mode** and **Editor Mode**.  

![gui-and-editor-pic](https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/GUIandEditor.jpg?raw=true)

### GUI Mode
You can do an operation of GUI objects, moving the mouse cursor, process execution, operating all windows by key-bindings like Vim. By the way, this mode is applied at the first start after installation. My favorite functions are **EasyClick**. It allows us to select GUI objects by operations like <a href="https://github.com/easymotion/vim-easymotion">**EasyMotion**</a> or <a href="https://github.com/philc/vimium">**Vimium**</a>.

![easy-click-demo.gif](https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/EasyClickDemo.gif?raw=true)

### Editor Mode
It enables to emulate Vim when using general text editors, for instance, notepad or Microsoft Office Word or some Web forms. However, currently not implemented the complete text analyzing system, so some behavior is different from the original. 

<hr>  

## Note 
- `F8 + F9` is safe forced termination.
- win-vind could **not** operate some windows given high-rank authorization than itself. For example, if you start **Task Manager** having the highest authorization and select its window, you cannot move, click or scroll the mouse cursor by win-vind. If you want to operate all windows, I recommend giving win-vind the administrator authorization. (Please use **Task Scheduler**).

<hr>

## Customization
### Parameters
You are able to change all parameters of UI-language, the speed of the mouse, options, etc in **Preferences** window.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/pref_3.0.png?raw=true" width=740 />  

However, it is based on GUI, so some Vimmer dislike its UI. In this case, rewrite **settings.json** with **Vim**.  The file locates in the following paths.

|Installation Type|Path|
|:---:|:---|
|Installer|C:/Users/**&lt;User-Name&gt;**/.win-vind/settings.json|
|Zip|**&lt;Zip-Root&gt;**/config/settings.json|

<br>
<hr>

### Key Config
You can customize all key bindings by rewriting the JSON settings file or using GUI easily.  

**Demo**  
<a href="https://github.com/pit-ray/win-vind/discussions/7"><img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/video_preview_customizing_mp4.png?raw=true" width="500"></a>  

Its syntaxes are based on original Vim, but have some unique keywords. You can refer all keywords at <a href="https://pit-ray.github.io/win-vind/cheat_sheet/keyword_lists/">Keyword Lists</a>.

#### Example  
```json
    {
        "name": "edi_move_caret_down",
        "guin": ["test", "Te"],
        "guii": [],
        "guiv": [],
        "edin": ["j", "+", "<C-n>", "<C-N>"],
        "edii": [],
        "ediv": ["<edin>"],
        "edivl": ["<edin>"],
        "cmd": ["+<num>", "Example"],
        "en": "Move Caret Down",
        "ja": "キャレットを下へ移動"
    },
```
**name** is a unique identifier of a linked function. **en** or **ja** are labels used on GUI of win-vind. If you give a unique key as language code and a translated label, enable to support another language.  
<br>
The meanings of these key config are shown in the following table with the proviso that the column of **String** is case-sensitive, but the column of **Meanings** is not case-sensitive.  

|String|Meanings (but depending on an arrangement of a keyboard)|
|:----:|:---|
|test|`T`->`E`->`S`->`T`|
|Te|`Shift + T`->`E`|
|j|`J`|
|+|`Shift + =`|
|&lt;C-n&gt;|`Ctrl + N`|
|&lt;C-N&gt;|`Ctrl + Shift + N`|
|&lt;edin&gt;|It means copying the key config from another mode,<br>so **Editor Visual** and **Editor Line Visual** are same as the configs of **Editor Normal**.|
|+&lt;num&gt;|&lt;num&gt; is a number of any digits. For example, `LShift + ;`->`Shift + =`->`1`->`2`.|
|Example|`Shift + ;`->`Shift + E`->`X`->`A`->`M`->`P`->`L`->`E`|
