---
layout: default
title: Process Launcher
nav_order: 5
parent: Cheat Sheet
descripption: "Process Launcher"
grand_parent: v3
---

# Process Launcher  
{: .no_toc}

<hr>

1. TOC  
{:toc}


## External Application  
### Start Shell
  
|Details|Status|  
|:---|:---|  
|ID|**start_shell**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|-|
|Default Command|`:sh`, `:shell`, `:terminal`, `:term`| 

It creates a process of your favorite terminal application. The default terminal is **Command Prompt**, but you can change it by rewriting **settings.json** or using win-vind preferences window. Its application does not have to be a console application.  

<br>

### Start Any Application

|Details|Status|  
|:---|:---|  
|ID|**start_any_app**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|-|
|Default Command|`!<any>`| 

It starts a registered application at **Preferences/Shortcut Apps**. If you register **notepad.exe** as **notepad**, could launch it by typing `:!notepad` like below.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/cmd-demo.gif?raw=true" />  

<br>  

<hr>  


## Windows Standard Application  

### Open Explorer

|Details|Status|  
|:---|:---|  
|ID|**start_explorer**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|-|
|Default Command|`:ex`, `:explorer`| 

It is the same as **Win + E** of shortcuts on Windows.  

<br>  

### Open StartMenu

|Details|Status|  
|:---|:---|  
|ID|**open_start_menu**|  
|N-Repeat|<span class="no">Not Supported</span>|  
|Default Key|`<Win>`|
|Default Command|`:open`, `:start`| 

It is the same as **Win** of shortcuts on Windows.  
