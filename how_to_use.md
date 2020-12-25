---
layout: default
title: How to use
nav_order: 3
descripption: "Usage of win-vind"
---
This software supports only Windows 10. Some features may be not working well on the others. If it is unstable despite Windows 10, please post issues to <a href="https://github.com/pit-ray/win-vind/issues">github.com/issues</a>.  

## Installation
1. Download the latest version by following a button. These links will probably be received some warnings. Today, it is very expensive to do code-signing to our application, so it is unavoidable as free software.
**Installer version**  
[Download Installer (.exe)](https://github.com/pit-ray/win-vind/releases/download/v2.1.1/setup_win-vind_2.1.1.exe){: .btn }

 **Zip version**  
[Download Zip (.zip)](https://github.com/pit-ray/win-vind/releases/download/v2.1.1/win-vind_2.1.1.zip){: .btn}  

If you want not to receive some warnings, execute next commands.  
 
**Installer version**
```bash
$ curl -OL https://github.com/pit-ray/win-vind/releases/download/v2.1.1/setup_win-vind_2.1.1.exe
```

**Zip version**
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
win-vind has plenty of hotkeys and commands, but they are based on the original Vim. Some little differences are that its mode has two layers and its unique functions for GUI. Concretely, they are **GUI Mode** and **Editor Mode**.  

![gui-and-editor-pic](https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/GUIandEditor.jpg?raw=true)

### GUI Mode
The target is the mouse cursor. For instance, if you input h, moves it to left. However, it does not have a concept called rows and columns, so I implemented a wealth of alternative functions.

### Editor Mode
It enables to emulate Vim when using general text editors, for instance, notepad or Microsoft Office Word or some Web forms. The target is each caret of text controls. In other words, you can operate a text editor that mainly control by a mouse with key binds of the original Vim.

## Note 
- `F8 + F9` is safe forced termination.
- win-vind **cannot** operate some windows given high-rank authorization than itself. For example, if you start Task Manager having the highest authorization and select its window, you cannot move, click or scroll the mouse cursor by win-vind. If you want to operate all windows, I recommend giving win-vind the administrator authorization. (Please use **Task Scheduler**.

## Bindings
You can customize all key bindings by GUI based settings or rewriting JSON settings file easily.

### GUI based
!(bind-lits)[https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/bind_lits.jpg?raw=true]
There are settings in <b>Preferences</b> in the system tray.

### Rewriting JSON
If you use the installer, the file is located in **~/.win-vind/bindings.json**.  
If you get zip-version, it is existed in **win-vind/config/bindings.json**.

### Syntax
- `[key1](+)[key2]` means typing key1 and key2 with same timing.  
- `[key1]-&gt;[key2]` means typing key2 after key1.  
- It distinguished between upper and lowercase letters. For example, `a` means `a`, `A` does `Shift+a`.
- If you want to use system keys like Shift or Ctrl, please write KMP key code.
