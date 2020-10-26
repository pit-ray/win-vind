# win-vind
Simple Vim Key Binder for Windows  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/win-vind-icon.png?raw=true" width="256" height="256" align="right" title="win-vind">
<br>


Please refer to <a href="https://pit-ray.github.io/win-vind/">github.io/win-vind</a>.  

## Description
You can operate Windows by key-bindings like Vim.  
**win-vind** provides the lightweight **Hybrid UI System of CUI and GUI**.  

## Warning
It has some serious problems repeating key-stroke infinitely in `Editor Mode`.
Now, I am fixing them, so please wait for the next version in November or don't use `Editor Mode`.

## Overview
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/mode_overview_2.jpg?raw=true" width=700 title="Mode Overview">  

`win-vind` has plenty of hotkeys and commands, but they are based on the original Vim. Some little differences are that its mode has two layers and its unique functions for GUI. Concretely, they are `GUI Mode` and `Editor Mode`.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/GUIandEditor.jpg?raw=true" width=600>  

### GUI Mode
This is default mode after starting `win-vind`. The target is the mouse cursor. For instance, if you input `h`, moves it to left.    However, it does not have a concept called rows and columns, so I implemented a wealth of alternative functions. You can be referred to <a href="https://pit-ray.github.io/win-vind/cheat_sheet/">github.io/win-vind/cheat_sheet</a>.  

#### Example
You can start optional application shortcuts by a registered command as following.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/config_shortapps.jpg?raw=true" width=512>  

If you register `notepad.exe` as `notepad`, could launch it by typing `:!notepad` like below.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/cmd-demo.gif?raw=true" width=400>  
<br>

### Editor Mode
It enables to emulate Vim when using general text editors, for instance, `notepad.exe` or `Microsoft Office Word`. The target is each caret of text controls. In other words, you can operate a text editor that mainly control by a mouse with key bindings of the original Vim.
<br>

## How to use
Please visit <a href="https://pit-ray.github.io/win-vind/how_to_use/">github.io/win-vind/how_to_use</a>.  

## How to contribute
Please visit <a href="https://pit-ray.github.io/win-vind/how_to_contribute/">github.io/win-vind/how_to_contribute</a>.  

## Author
- pit-ray  
[E-mail] pit-ray(at)outlook.com
