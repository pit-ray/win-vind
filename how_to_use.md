---
layout: default
title: How to use
nav_order: 3
descripption: "Usage of win-vind"
---
This software supports only on Windows 10. Some features may be not working well on the others. If it is unstable despite Windows 10, please post issues to <a href="https://github.com/pit-ray/win-vind/issues">github.com/issues</a>.  

1. TOC  
{:toc}

<hr>

## Installation
1. Download the latest version by following a button. These links will probably be received some warnings. Today, it is very expensive to do code-signing to our application, so it is unavoidable as free software. <br>   
[Download Installer (.exe)](https://github.com/pit-ray/win-vind/releases/download/v2.1.1/setup_win-vind_2.1.1.exe){: .btn }&nbsp;&nbsp;&nbsp;&nbsp;[Download Zip (.zip)](https://github.com/pit-ray/win-vind/releases/download/v2.1.1/win-vind_2.1.1.zip){: .btn}  <br>  
If you want not to receive some warnings, execute next commands.  <br>    
- **Installer version**  
```bash
$ curl -OL https://github.com/pit-ray/win-vind/releases/download/v2.1.1/setup_win-vind_2.1.1.exe
```  
- **Zip version**  
```bash
$ curl -OL https://github.com/pit-ray/win-vind/releases/download/v2.1.1/win-vind_2.1.1.zip
```

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
You can do an operation of GUI objects, moving the mouse cursor, process execution, operating all windows by key-bindings like Vim. By the way, this mode is applied at the first start after installation. My favorite functions are **EasyClick**. It allows you to select GUI objects by operations like <a href="https://github.com/easymotion/vim-easymotion">**EasyMotion**</a> or <a href="https://chrome.google.com/webstore/detail/vimium/dbepggeogbaibhgnhhndojpepiihcmeb?hl=ja">**Vimium**</a>.

![easy-click-demo.gif](https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/EasyClickDemo.gif?raw=true)

### Editor Mode
It enables to emulate Vim when using general text editors, for instance, notepad or Microsoft Office Word or some Web forms. However, currently not implemented the complete text analyzing system, so some behavior is different from the original. 

<hr>  

## Note 
- `F8 + F9` is safe forced termination.
- win-vind **cannot** operate some windows given high-rank authorization than itself. For example, if you start **Task Manager** having the highest authorization and select its window, you cannot move, click or scroll the mouse cursor by win-vind. If you want to operate all windows, I recommend giving win-vind the administrator authorization. (Please use **Task Scheduler**.

<hr>


## Customization
### Parameters

### Key Config
You can customize all key bindings by rewriting the JSON settings file easily. The location of the file is different depending on the installation way. Refer to the following table.

|Installation|Path|
|:---:|:---|
|Installer|C:/Users/**&lt;User-Name&gt;**/.win-vind/bindings.json|
|Zip|**&lt;Zip-Root&gt;**/config/bindings.json|

Its syntaxes are based on original Vim, but have some unique keywords.  
#### Mode Keyword
|Keyword|Means|
|:---:|:---:|
|guin|GUI Normal Mode|
|guii|GUI Insert Mode|
|guiv|GUI Visual Mode|
|edin|Editor Normal Mode|
|edii|Editor Insert Mode|
|ediv|Editor Visual Mode|
|edivl|Editor Line Visual Mode|
|cmd|Command Mode|

#### Specific Ascii Key Code
|Key Code|Means or Usage|
|:---:|:---|
|&lt;space>|Space Key|
|&lt;hbar>|Ascii code '-'|
|<gt>|Ascii code '>'|
|<lt>|Ascii code '<'|
  
#### 
|&lt;ime&gt;|Key for switching IME|
|<tab>|Tab Key|
|<cr>|Enter Key, Return Key|
|<enter>|Enter Key, Return Key|

        {"return",      VKC_ENTER},
        {"capslock",    VKC_CAPSLOCK},
        {"left",        VKC_LEFT},
        {"right",       VKC_RIGHT},
        {"up",          VKC_UP},
        {"down",        VKC_DOWN},
        {"bs",          VKC_BKSPACE},

        {"shift",       VKC_SHIFT},
        {"s",           VKC_SHIFT},
        {"lshift",      VKC_LSHIFT},
        {"ls",          VKC_LSHIFT},
        {"rshift",      VKC_RSHIFT},
        {"rs",          VKC_RSHIFT},

        {"ctrl",        VKC_CTRL},
        {"c",           VKC_CTRL},
        {"lctrl",       VKC_LCTRL},
        {"lc",          VKC_LCTRL},
        {"rctrl",       VKC_RCTRL},
        {"rc",          VKC_RCTRL},

        {"win",         VKC_WIN},
        {"lwin",        VKC_LWIN},
        {"rwin",        VKC_RWIN},

        {"alt",         VKC_ALT},
        {"a",           VKC_ALT},
        {"lalt",        VKC_LALT},
        {"la",          VKC_LALT},
        {"ralt",        VKC_RALT},
        {"ra",          VKC_RALT},

        {"m",           VKC_ALT},
        {"lm",          VKC_LALT},
        {"rm",          VKC_RALT},

        {"nocvt",       VKC_NOCONVERT},
        {"cvt",         VKC_CONVERT},
        {"kana",        VKC_KANA},
        {"app",         VKC_APP},
        {"esc",         VKC_ESC},

        {"f1",          VKC_F1},
        {"f2",          VKC_F2},
        {"f3",          VKC_F3},
        {"f4",          VKC_F4},
        {"f5",          VKC_F5},
        {"f6",          VKC_F6},
        {"f7",          VKC_F7},
        {"f8",          VKC_F8},
        {"f9",          VKC_F9},
        {"f10",         VKC_F10},
        {"f11",         VKC_F11},
        {"f12",         VKC_F12},
        {"f13",         VKC_F13},
        {"f14",         VKC_F14},
        {"f15",         VKC_F15},
        {"f16",         VKC_F16},
        {"f17",         VKC_F17},
        {"f18",         VKC_F18},
        {"f19",         VKC_F19},
        {"f20",         VKC_F20},
        {"f21",         VKC_F21},
        {"f22",         VKC_F22},
        {"f23",         VKC_F23},
        {"f24",         VKC_F24},

        {"snapshot",    VKC_SNAPSHOT},
        {"scroll",      VKC_SCROLL},
        {"pause",       VKC_PAUSE},
        {"insert",      VKC_INSERT},
        {"home",        VKC_HOME},
        {"pageup",      VKC_PAGEUP},
        {"del",         VKC_DELETE},
        {"end",         VKC_END},
        {"pagedown",    VKC_PAGEDOWN},

        {"numlock",     VKC_NUMLOCK}
