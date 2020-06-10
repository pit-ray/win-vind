# win-vind
Vim Key Binder for Windows  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/win-vind-icon.png?raw=true" width="256" height="256" align="right" title="win-vind">
<br>

**win-vind** provides the light weight **Hybrid UI System of CUI and GUI**.  
<br>
GitHub wiki pages is <a href="https://github.com/pit-ray/win-vind/wiki">this</a>.  
日本語のGitHub wikiページは <a href="https://github.com/pit-ray/win-vind/wiki/%E3%83%9B%E3%83%BC%E3%83%A0-(Home-in-Japanese)">こちら</a>です。  
<br>


## Description  
Vim, Vi is a powerful light-weight awesome editor, has the idia of OSS. Their key binds are very intuitive and allow us to type keeping positions of hands over key board without moving them to mouse. I like this idea, so I implemented key bind system of Vim for manipulating Windows GUI and general text editors, while staying at home for the virus.  
This software provides key bind like Vim and useful shortcut system and is very light-weight.  


## Overview  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/mode_overview.jpg?raw=true" width=700 title="Mode Overview">  

`win-vind` has plenty of hot keys and commands, but they is based on the original Vim. Some little differences are that its mode has two layers and its unique functions for GUI. Concretely, they are `GUI Mode` and `Editor Mode`.

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/GUIandEditor.jpg?raw=true" width=600>


### GUI Mode
This is default mode after starting `win-vind`. The target is the mouse cursor. For instance, if you input `h`, moves it to left.    However, it doses not have a concept called rows and columns, so I implemented a wealth of alternative functions. You can be referred to <a href="https://github.com/pit-ray/win-vind/wiki/For-Users#key-bind-list">wiki</a>.  

#### Example  
You are able to start optional application shortcuts by a registered command as following.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/config_shortapps.jpg?raw=true" width=512>  

If you register `notepad.exe` as `notepad`, could launch it by typing `:!notepad` like below.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/cmd-demo.gif?raw=true" width=400>  
<br>

### Editor Mode  
It enable to emulate Vim when use general text editors, for instance, `notepad.exe` or `Microsoft Office Word`. The target is each caret of text controls. In other words, you can operate a text editor that mainly control by a mouse with key binds of the original Vim.  
<br>
<br>


## How to use  
Please visit <a href="https://github.com/pit-ray/win-vind/wiki/For-Users">GitHub wiki pages</a>.  
<a href="https://github.com/pit-ray/win-vind/wiki/%E4%BD%BF%E3%81%84%E6%96%B9-(For-Users-in-Japanese)">GitHub wikiページ</a>に日本語で詳細を載せました。  
<br>
<br>

## Build instructions  
### How to build Project  
**This project use `<mutex>` and `<atomic>`**, so some MinGW without it will fail a build. In this case, you will need to install other MinGW with `<mutex>` and `<atomic>`. (select `posix` at item called **Thread** in MinGW-Installer.)  


- Automatically Build  
I recommend executing `build.bat`  
``` shell
./build.bat [-debug/-release]
```

- Manualy Build  
If you want to manualy build, use `cmake` and `make` commands as follows.  
```shell
mkdir debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ..
mingw32-make -f Makefile
cd ..
```
<br>

## For Developer
Please visit <a href="https://github.com/pit-ray/win-vind/wiki/For-Developers">GitHub wiki pages</a>.  
<a href="https://github.com/pit-ray/win-vind/wiki/%E9%96%8B%E7%99%BA%E8%80%85%E3%81%B8(For-Developers-in-Japanese)">GitHub wikiページ</a>に日本語で詳細を載せました。  
<br>

## Development Enviroment
||Name and Version|
|---|---|
|OS|Windows 10 Home 64bit 1909|
|Framework|MinGW-W64-builds-4.3.5, CMake 3.14.4|
|Compiler|GNU GCC 8.20|
|Library|Boost 1.72.0, Win32API, STL, wxWidgets 3.0.5|
|CPU|AMD Ryzen 2600|

## Author
- pit-ray  
[E-mail] contact(at)pit-ray.com
