---
layout: default
title: Usage
nav_order: 3
descripption: "Usage of win-vind"
---
# Usage
{: .no_toc }

This software supports only on Windows 10. Some features may be not working well on the others. If it is unstable despite Windows 10, please post issues to <a href="https://github.com/pit-ray/win-vind/issues">github.com/issues</a>.  

<hr>  

1. TOC  
{:toc}

<hr>

## Installation
1. Download the latest version by following a button. These links will probably be received some warnings. Today, it is very expensive to do code-signing to our application, so it is unavoidable as free software. <br>   

   **32 bit**
   [Download .exe (32bit)](https://github.com/pit-ray/win-vind/releases/download/v4.0.0/setup_win-vind_4.0.0_32bit.exe.zip){: .btn }&nbsp;&nbsp;&nbsp;&nbsp;[Download .zip (32bit)](https://github.com/pit-ray/win-vind/releases/download/v4.0.0/win-vind_4.0.0_32bit.zip){: .btn}  

   **64 bit**
   [Download .exe (64bit)](https://github.com/pit-ray/win-vind/releases/download/v4.0.0/setup_win-vind_4.0.0_64bit.exe.zip){: .btn }&nbsp;&nbsp;&nbsp;&nbsp;[Download .zip (64bit)](https://github.com/pit-ray/win-vind/releases/download/v4.0.0/win-vind_4.0.0_64bit.zip){: .btn}  

1. Execute a downloaded installer or unzip a downloaded zip-file. 

1. Start **win-vind.exe**.

1. You can set it up by typing `:set` or `:config` or right-click the icon of win-vind on the system tray and select **Preferences**.  

1. In the end, you can terminate win-vind by `:exit` or right-click on the system tray and select **exit**.

<hr>

## Overview  

**Insert Mode** and **Resident Mode** pass all key messages to Windows, while **GUI Normal Mode**, **GUI Visual Mode**, **Edi Normal Mode**, **Edi Visual Mode**, and **Command Mode** do not.  
<p align="center">
<img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/mode_overview_3D.png?raw=true" width=500 >  
<p align="center">Visual Concepts of Mode</p>
</p>


win-vind has plenty of hotkeys and commands, but they are almost based on the original Vim or some famous plugins. The little differences are that its mode has two layers and its unique functions for GUI. Concretely, two layer modes are **GUI Mode** and **Editor Mode**.  

<p align="center">
<img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/mode_overview.png?raw=true" width=600>  
<p align="center">Default mode layer overview</p>
</p>

<p align="center">
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/GUIandEditor.jpg?raw=true" width=700>
<p align="center">Concepts of GUI Mode and Editor Mode</p>
</p>

<hr>  

## Note 
- `F8 + F9` is safe forced termination.
- win-vind could **not** operate some windows given high-rank authorization than itself. For example, if you start **Task Manager** having the highest authorization and select its window, you cannot move, click or scroll the mouse cursor by win-vind. If you want to operate all windows, I recommend giving win-vind the administrator authorization. (Please use **Task Scheduler**).

<hr>


## Customization
### Options

### Maps
