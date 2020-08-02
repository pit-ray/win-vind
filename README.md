# win-vind
Vim Key Binder for Windows  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/win-vind-icon.png?raw=true" width="256" height="256" align="right" title="win-vind">
<br>

**win-vind** provides the lightweight **Hybrid UI System of CUI and GUI**.  
<br>
GitHub wiki pages is <a href="https://github.com/pit-ray/win-vind/wiki">this</a>.  
日本語のGitHub wikiページは <a href="https://github.com/pit-ray/win-vind/wiki/%E3%83%9B%E3%83%BC%E3%83%A0-(Home-in-Japanese)">こちら</a>です。  
<br>

`vind` = `vi` + `bind`

## Description  
Vim, Vi is a powerful lightweight awesome editor, has the idea of OSS. Their key bindings are very intuitive and allow us to type keeping positions of hands over a keyboard without moving them to mouse. I like this idea, so I implemented the key bindings system like Vim for manipulating Windows GUI and general text editors.
This software provides the key bindings like Vim and useful shortcut systems.  


## Overview  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/mode_overview_2.jpg?raw=true" width=700 title="Mode Overview">  

`win-vind` has plenty of hotkeys and commands, but they are based on the original Vim. Some little differences are that its mode has two layers and its unique functions for GUI. Concretely, they are `GUI Mode` and `Editor Mode`.

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/GUIandEditor.jpg?raw=true" width=600>

### GUI Mode
This is default mode after starting `win-vind`. The target is the mouse cursor. For instance, if you input `h`, moves it to left.    However, it does not have a concept called rows and columns, so I implemented a wealth of alternative functions. You can be referred to <a href="https://github.com/pit-ray/win-vind/wiki/For-Users#key-bind-list">wiki</a>.  

#### Example  
You can start optional application shortcuts by a registered command as following.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/config_shortapps.jpg?raw=true" width=512>  

If you register `notepad.exe` as `notepad`, could launch it by typing `:!notepad` like below.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/cmd-demo.gif?raw=true" width=400>  
<br>

### Editor Mode  
It enables to emulate Vim when using general text editors, for instance, `notepad.exe` or `Microsoft Office Word`. The target is each caret of text controls. In other words, you can operate a text editor that mainly control by a mouse with key bindings of the original Vim.  
<br>

## How to use  
Please visit <a href="https://github.com/pit-ray/win-vind/wiki/For-Users">GitHub wiki pages</a>.  
<a href="https://github.com/pit-ray/win-vind/wiki/%E4%BD%BF%E3%81%84%E6%96%B9-(For-Users-in-Japanese)">GitHub wikiページ</a>に日本語で詳細を載せました。  

## For Developer
Please visit <a href="https://github.com/pit-ray/win-vind/wiki/For-Developers">GitHub wiki pages</a>.  
<a href="https://github.com/pit-ray/win-vind/wiki/%E9%96%8B%E7%99%BA%E8%80%85%E3%81%B8(For-Developers-in-Japanese)">GitHub wikiページ</a>に日本語で詳細を載せました。  

## TO DO
Please contribute or request.  
There are already implemented functions in <a href="https://github.com/pit-ray/win-vind/blob/master/latest_log.md">latest_log.md</a>.
<br>
#### Editor Mode Commands
- `s` / `S`
- `cw` / `C`
- `~`
- `?` / `/` / `n` / `N`
- operator commands like `diw`
- replacing commands like `:s/old/new` or `:%s/old/new/g`

#### Differences with Original Vim
- repeating commands
- jumping to any line

#### Useful
- By one key + mouse clicking, transitions Editor-Mode for a target window. (If change a selected window, turn off Editor-Mode.)
- setting of an initial mode
- config's saving system when reinstall

#### Improving
- better error message system
- improve interfaces of Preferences/Bind-List
- better useful translating system for developer


## Author
- pit-ray  
[E-mail] pit-ray(at)outlook.com
