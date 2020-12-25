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
You can customize all key bindings by rewriting the JSON settings file easily. In the future, going to just write the changes. The location of the file is different depending on the installation way. Refer to the following table.

|Installation|Path|
|:---:|:---|
|Installer|C:/Users/**&lt;User-Name&gt;**/.win-vind/bindings.json|
|Zip|**&lt;Zip-Root&gt;**/config/bindings.json|

Its syntaxes are based on original Vim, but have some unique keywords. You can refer keywords at 

