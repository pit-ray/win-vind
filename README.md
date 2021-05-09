<h1 align="center">win-vind</h1>

<p align="center">
  <img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/win-vind-icon.png?raw=true" width="150" height="150" />
  <p align="center"><b>Vim Key Binder for Windows</b></p>  
</p>  

<table align="center">
    <tr>
        <th colspan="2">Code status</th>
        <th colspan="2">Build status</th>
    </tr>
    <tr align="center">
        <td>Coverity</td>
        <td>Codacy</td>
        <td>MinGW-w64</td>
        <td>Visual Studio 2019</td>
    </tr>
    <tr>
        <td align="center">
            <a href="https://scan.coverity.com/projects/pit-ray-win-vind"><img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects/22417/badge.svg"/></a>  
        </td>
        <td align="center">
            <a href="https://www.codacy.com/gh/pit-ray/win-vind/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=pit-ray/win-vind&amp;utm_campaign=Badge_Grade"><img src="https://app.codacy.com/project/badge/Grade/8f2e6f2826904efd82019f5888574327" /></a>
        </td>
        <td align="center">
            <a href="https://travis-ci.org/pit-ray/win-vind"><img src="https://travis-ci.org/pit-ray/win-vind.svg?branch=master" /></a>
        </td>
        <td align="center">
            <a href="https://ci.appveyor.com/project/pit-ray/win-vind"><img src="https://ci.appveyor.com/api/projects/status/qoggl832qqbic5f5?svg=true" /></a>  
        </td>
    </tr>
</table>

## [Notice] v3.3.0-preview Avaliable Now!!
### 3.3.0 (Latest Preview Version)  
Date: 2021/5/9  

#### 32bit  
- <a href="https://github.com/pit-ray/win-vind/releases/download/v3.3.0/setup_win-vind_3.3.0_32bit.exe.zip">setup_win-vind_3.3.0_32bit.exe</a>
- <a href="https://github.com/pit-ray/win-vind/releases/download/v3.3.0/win-vind_3.3.0_32bit.zip">win-vind_3.3.0_32bit.zip</a>  

#### 64bit  
- <a href="https://github.com/pit-ray/win-vind/releases/download/v3.3.0/setup_win-vind_3.3.0_64bit.exe.zip">setup_win-vind_3.3.0_64bit.exe</a>
- <a href="https://github.com/pit-ray/win-vind/releases/download/v3.3.0/win-vind_3.3.0_64bit.zip">win-vind_3.3.0_64bit.zip</a> 
  
  

## Description
**win-vind** provides the lightweight hybrid UI system of CUI and GUI. Its core idea is to add a new mode layer to operate GUI when you want to transition from Vim to other windows for a short while.  

There are five main features.  

#### 1. GUI Operation like Vim  
You can operate windows, a mouse cursor , virtual desktops, GUI-objects with key-bindings like Vim.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/window_ctrl_demo_min.gif?raw=true" title="WindowControl-demo" width=600 >

#### 2. Vim emulation everywhere  
It enables to emulate Vim when using general text editors (e.g.`notepad.exe`, `Microsoft Office Word`).  

**Demo**  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/msword-demo.gif?raw=true" width=400/>   


#### 3. Mode Management like Vim  
There are two main modes: **GUI Mode** for GUI operation and **Editor Mode** for Vim emulation.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/mode_overview_2.jpg?raw=true" width=700 />  

#### 4. Process Control  
If you register `notepad.exe` as `notepad`, could launch it by typing `:!notepad` like below.  

**Demo**  
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/cmd-demo.gif?raw=true" width=400 title="cmd-demo">  

#### 5. Using from command line
You can operate win-vind from command lines.  

**Example**  
```sh  
$ ./win-vind.exe --func change_to_normal
```
Therefore, it enables to make Vim link to GUI or use from AutoHotKey.

If you add next scripts into .vimrc, you can switch a mode to operate GUI with `:GUINormal`.
```Vim
Plug 'skywind3000/asyncrun.vim'

command! GUINormal :AsyncRun win-vind -f change_to_normal
```

If you add next scripts into AHK Script, you can call each function.
```AHK
#f::Run, win-vind -f easy_click_left
#a::Run, win-vind -f arrange_windows
```

Refer to <a href="https://pit-ray.github.io/win-vind/cheat_sheet/link_with_vim/">Link With Vim - win-vind</a>. If you want to know the details, refer to <a href="https://pit-ray.github.io/win-vind/cheat_sheet/">Cheat Sheet - win-vind</a>.  

## Download
### 3.2.3 (Latest Version)  
Date: 2021/4/2  

#### 32bit  
- <a href="https://github.com/pit-ray/win-vind/releases/download/v3.2.3/setup_win-vind_3.2.3_32bit.exe.zip">setup_win-vind_3.2.3_32bit.exe</a>
- <a href="https://github.com/pit-ray/win-vind/releases/download/v3.2.3/win-vind_3.2.3_32bit.zip">win-vind_3.2.3_32bit.zip</a>  

#### 64bit  
- <a href="https://github.com/pit-ray/win-vind/releases/download/v3.2.3/setup_win-vind_3.2.3_64bit.exe.zip">setup_win-vind_3.2.3_64bit.exe</a>
- <a href="https://github.com/pit-ray/win-vind/releases/download/v3.2.3/win-vind_3.2.3_64bit.zip">win-vind_3.2.3_64bit.zip</a> 
  
  
## Related Pages
- <a href="https://www.pit-ray.com/archive/category/win-vind">Description Blog of Author in Japanese</a>  
- <a href="https://www.reddit.com/user/pit-ray/posts/">Posts of reddit</a>  


## Usage  
Please refer to <a href="https://pit-ray.github.io/win-vind/cheat_sheet/">CheatSheet</a> about the default bindings.  
  
You can call a pre-defined function with favorite keybindings. The above-stated mode management has the following features, and all modes can have functions to switch to each mode with your keybindings. All modes have presets of keybindings.

|Mode&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|Features|
|:-----|:---|
|GUI Normal, Edi Normal|All keys except hotkeys will be disabled. Therefore, recommended to include functions to transit to Insert Mode.|
|GUI Insert, Edi Insert|Inputted keys will not be disabled and are transmitted normally to other applications. When the commands are detected, a function will be called.|
|GUI Visual|It holds right-click.|
|Edi Visual, Edi Visual Line|Each function works on the assumption you hold down a shift key.|
|Command|This is shared Editor mode and GUI mode. A virtual command line is launched, and Enter is available to decide and BackSpace is available to erase. You can move the history with Up and Down.|

Please visit <a href="https://pit-ray.github.io/win-vind/how_to_use/">How to use - win-vind</a> for details.  

**Demo**  
You can change default bindings.    
<a href="https://github.com/pit-ray/win-vind/discussions/7"><img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/video_preview_customizing_mp4.png?raw=true" width="500"></a>


## How to contribute

Please read its architecture at <a href="devdocs/README.md">devdocs</a>.  

### Quick Start for Build  
If you have already installed MinGW-w64 or Visual Studio 2019, all you need is the next steps.  

**1.** Install needed libraries into a root directory of win-vind.  
  ```bash  
  $ ./scripts/setup_libs.bat [-mingw/-msvc] [32/64]
  ```  
**2.** Build its project.  
  ```bash
  $ ./build.bat [-debug/-release] [-mingw/-msvc] [32/64]  
  ```
**3.** Finish!  

If you want to contribute, please visit <a href="https://pit-ray.github.io/win-vind/how_to_contribute/">How to contribute - win-vind</a>.  

## ToDo  

It is the difference between master and latest release.

|State|Description|Type|Priority|
|:---:|:---|:---:|:---:|
|☐|Support to force CapsLock as another keys. (#20)|Feature|High|
|☐|Improve customizability (preset feature like a gaming device utility? or customize like `.vimrc`?)|Architecture|High|
|☐|Add a reference option in a command line.|Feature|Mid|
|☐|Capture coordinates of the key mapping from images in order to generate KMP files.|Feature|Mid|
|☐|Divide GUI and core system.|Architecture|Mid|
|☐|Implement <a href="https://github.com/simeji/winresizer">simeji/winresizer</a> in GUI|Feature|Mid|
|☐|Implement `E` or `e` in Editor Mode with UI Automation.|Feature|Mid|
|☐|Use a real command line system in Virtual Command Line.|Feature|Mid|
|☐|Increase the test coverage|Improve|Mid|
|☐|Bug Reporting feature|Feature|Low|
|☐|Modern GUI (use WinUI3 or Flutter2?)|Change|Low|
|☐|Update Notification|Feature|Low|

## License  

This software is provided by **MIT License**.  

## Author

- pit-ray  
[E-mail] pit-ray(at)outlook.com
