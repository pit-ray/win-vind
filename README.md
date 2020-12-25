# win-vind
Simple Vim Key Binder for Windows  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/win-vind-icon.png?raw=true" width="256" height="256" align="right" title="win-vind">
<br>  

## Description
**win-vind** provides the lightweight **Hybrid UI System of CUI and GUI**.  

## Overview
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/mode_overview_2.jpg?raw=true" width=700 title="Mode Overview">  

`win-vind` has plenty of hotkeys and commands, but they are based on the original Vim. Some little differences are that its mode has two layers and its unique functions for GUI. Concretely, they are `GUI Mode` and `Editor Mode`.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/GUIandEditor.jpg?raw=true" width=600>  

### GUI Mode
This is default mode after starting `win-vind`. The target is the mouse cursor. For instance, if you input `h`, moves it to left.    However, it does not have a concept called rows and columns, so I implemented a wealth of alternative functions. You can be referred to <a href="https://pit-ray.github.io/win-vind/cheat_sheet/">github.io/win-vind/cheat_sheet</a>.  

#### Example
You can start optional application shortcuts by a registered command as following.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/config_shortapps.jpg?raw=true" width=512 title="shortcut_config">  

If you register `notepad.exe` as `notepad`, could launch it by typing `:!notepad` like below.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/cmd-demo.gif?raw=true" width=400 title="cmd-demo">  
<br>

### Editor Mode
It enables to emulate Vim when using general text editors, for instance, `notepad.exe` or `Microsoft Office Word`. The target is each caret of text controls. In other words, you can operate a text editor that mainly control by a mouse with key bindings of the original Vim.
<br>

## How to use
Please visit <a href="https://pit-ray.github.io/win-vind/how_to_use/">github.io/win-vind/how_to_use</a>.  

## How to contribute
Please visit <a href="https://pit-ray.github.io/win-vind/how_to_contribute/">github.io/win-vind/how_to_contribute</a>.  

## Latest Log
It has differences from the latest release.
`release` is the so-called stable version.

### Fix
- fixed the problem that line's copying functions (e.g. dd, yy) are not working on Microsoft Office Word.
- fixed the problem repeating key-stroke infinity.
- fixed not working after BackSpace in Virtual Command Line.
- fixed flicking display caused by VirtualCommandLine.

### Change
- The Command Mode was divided as KeyBindings from the inner matching system.
- This version requires C++17
- Improved Error out function.
- Supported appropriate exception-based error system.
- In bindings.json, changed command's syntax from unique one to original-Vim.
- Some classes modified to STL-based simple one.
- Supported file-properties of the installer
- Changed methods of the key eventer to faster one.
- If a command called by `!*` is not existed, will print out message.
- Prohibited multiple running of win-vind.

### New
#### General
- supported repeating commands (e.g. `5->h` is equel to `h->h->h->h->h`)  
- add a CUI-based feature to call BindedFunc from another win-vind with the inter-process communication.  
  (ex: `$./win-vind --func change_to_normal`)
- Supported the number keyword (e.g. `<num>`, `+<num>`, `-<num>`)

#### Options
- `Char Cache Option`; for faster edit.
- `Suppress For Vim`; in order to disable win-vind in Vim.

#### Functions
- Change Lines And Start Insert (`c->c`, `S`)
- Change Characters And Start Insert (`s`)
- Change Text Until EOL And Start Insert (`C`)
- Delete Text by Motion (`d{Motion}`)
- Change Text by Motion (`c{Motion}`)
- Start Explorer (`ex`, `explorer`)
- Open Windows Start Menu (`<Win>`)
- Switch Case of characters (`~`)
- EasyClick; feature like EasyMotion or Vimium. (`F`)

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/EasyClickDemo.gif?raw=true" title="EasyClick-demo">

### Bugs
- On the desktop, cannot refresh virtual command line.  

## ToDo

## Author
- pit-ray  
[E-mail] pit-ray(at)outlook.com
