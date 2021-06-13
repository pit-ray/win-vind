<h1 align="center">win-vind</h1>

<p align="center">
  <img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/win-vind-icon.png?raw=true" width="150" height="150" />
  <p align="center"><b>Vim Key Binder for Windows</b></p>  
</p>  

<table align="center">
    <tr>
        <th colspan="2">Code status</th>
        <th colspan="2">Build status</th>
        <th colspan="2">Test status</th>
    </tr>
    <tr align="center">
        <td>Coverity</td>
        <td>Codacy</td>
        <td>MinGW-w64</td>
        <td>Visual Studio 2019</td>
        <td>Windows</td>
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
            <a href="https://github.com/pit-ray/win-vind/actions/workflows/msvc.yml"><img src="https://github.com/pit-ray/win-vind/actions/workflows/msvc.yml/badge.svg"></a>
        </td>
        <td align="center">
            <a href="https://github.com/pit-ray/win-vind/actions/workflows/test.yml"><img src="https://github.com/pit-ray/win-vind/actions/workflows/test.yml/badge.svg"></a>
        </td>
    </tr>
</table>
  

## Description
**win-vind** provides the lightweight hybrid UI system of CUI and GUI for Windows. 

### Features
- Vim-like Configuration Syntax with `.vindrc`
- Fast Binding System
- GUI Operation without Mouse
- Vim Emulation Everywhere
- Process Control like `:!gvim ~/.vimrc` or `:e ~/.vimrc`
- Vim-like Mode Manegement
- Low-level key mapping (e.g. `imap <Capslock> <ctrl>`)
- Running in User Permission
- Oneshot Use for Vim or AHK

If you want to know the details, refer to <a href="https://pit-ray.github.io/win-vind/cheat_sheet/">Cheat Sheet - win-vind</a>.  

## Demo

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/window_ctrl_demo_min.gif?raw=true" title="WindowControl-demo" width=600 >

## Download
### 4.0.0-rc2 (Preview Version)  
Date: 2021/6/13  

#### 32bit  
- <a href="https://github.com/pit-ray/win-vind/releases/download/v4.0.0-rc2/setup_win-vind_4.0.0_32bit.exe.zip">setup_win-vind_4.0.0_32bit.exe</a>
- <a href="https://github.com/pit-ray/win-vind/releases/download/v4.0.0-rc2/win-vind_4.0.0_32bit.zip">win-vind_4.0.0_32bit.zip</a>  

#### 64bit  
- <a href="https://github.com/pit-ray/win-vind/releases/download/v4.0.0-rc2/setup_win-vind_4.0.0_64bit.exe.zip">setup_win-vind_4.0.0_64bit.exe</a>
- <a href="https://github.com/pit-ray/win-vind/releases/download/v4.0.0-rc2/win-vind_4.0.0_64bit.zip">win-vind_4.0.0_64bit.zip</a> 
  
  
## Related Pages
- <a href="https://www.pit-ray.com/archive/category/win-vind">Description Blog of Author in Japanese</a>  
- <a href="https://www.reddit.com/user/pit-ray/posts/">Posts of reddit</a>  


## To Contribute

Please read its architecture at <a href="devdocs/README.md">devdocs</a>.  

### Quick Start for Build  
If you have already installed **MinGW-w64** or **Visual Studio 2019**, all you need is the next steps.  

**1.** Install needed libraries into a root directory of win-vind.  
  ```bash  
  $ ./scripts/setup_libs.bat [-mingw/-msvc] [32/64]
  ```  
**2.** Build its project.  
  ```bash
  $ ./build.bat [-debug/-release] [-mingw/-msvc] [32/64]  
  ```
**3.** Finish!  

You can refer to ToDo in <a href="https://github.com/pit-ray/win-vind/projects/2">Projects/win-vind</a>  

Pull requests are welcome!

## License  

This software is provided by **MIT License**.  

## Author

- pit-ray  
[E-mail] pit-ray(at)outlook.com
