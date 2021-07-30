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
- Fast Binding System
- Vim-like Mode Management
- GUI Operation without Mouse
- Vim Emulation Everywhere
- Process Control like `:!gvim ~/.vimrc` or `:e ~/.vimrc` (Resident command line)
- Vim-like Configuration Syntax with `.vindrc`
- Low-level key mapping (e.g. `imap <Capslock> <Ctrl>`)
- Running in User Permission
- Oneshot Use for Vim or AHK (e.g. `win-vind -f easy_click_left`)

If you want to know the details, refer to <a href="https://pit-ray.github.io/win-vind/cheat_sheet/">Cheat Sheet - win-vind</a>.  

## Demo

<img src="https://github.com/pit-ray/win-vind/blob/gh-pages/imgs/4xxdemo.gif?raw=true" title="Demo" >

<br>  

## Download
### 4.1.0 (Latest Version)  
Date: 2021/7/25  

#### 32bit  
- <a href="https://github.com/pit-ray/win-vind/releases/download/v4.1.0/setup_win-vind_4.1.0_32bit.exe.zip">setup_win-vind_4.1.0_32bit.exe</a>
- <a href="https://github.com/pit-ray/win-vind/releases/download/v4.1.0/win-vind_4.1.0_32bit.zip">win-vind_4.1.0_32bit.zip</a>  

#### 64bit  
- <a href="https://github.com/pit-ray/win-vind/releases/download/v4.1.0/setup_win-vind_4.1.0_64bit.exe.zip">setup_win-vind_4.1.0_64bit.exe</a>
- <a href="https://github.com/pit-ray/win-vind/releases/download/v4.1.0/win-vind_4.1.0_64bit.zip">win-vind_4.1.0_64bit.zip</a> 
  
## Usage
You can refer to the installation and quick tutorial in [Usage - win-vind](https://pit-ray.github.io/win-vind/usage/).  

<br>  
  
## Related Pages
- <a href="https://www.pit-ray.com/archive/category/win-vind">Description Blog of Author in Japanese</a>  
- <a href="https://www.reddit.com/user/pit-ray/posts/">Posts of reddit</a>  

## Contribute
Patches are welcome in whatever form. However, you must agree that your code will be provided by MIT License.

### To Documentation
You can contribute to the [homepage](https://pit-ray.github.io/win-vind/) by sending pull-request to the `gh-pages` branch if there is an error or a better way to describe the content. [Jekyll](https://jekyllrb.com/) is used as the framework, and can be written in markdown format. We also use [Just the Docs](https://pmarsceill.github.io/just-the-docs/) as a theme, which allows extended expressions.

### To Development
You can contribute to development by sending pull requests to the `master` branch to fix or add features, add test cases, or modify typos and expressions. When adding new code, please consider adding test cases in the test directory to satisfy branch coverage. [Codacy](https://www.codacy.com/gh/pit-ray/win-vind/dashboard?utm_source=github.com&utm_medium=referral&utm_content=pit-ray/win-vind&utm_campaign=Badge_Grade) and [Travis](https://travis-ci.org/pit-ray/win-vind) and [Actions](https://github.com/pit-ray/win-vind/actions) run by pushing, and [Coverity](https://scan.coverity.com/projects/pit-ray-win-vind) checks programmatic resources at releasing as Continuous Integration. If you want to discuss development, please create a thread in [Discussion](https://github.com/pit-ray/win-vind/discussions) with **#Development** category.


#### Quick Start for Build  
If you have already installed **MinGW-w64** or **Visual Studio 2019**, all you need is the next steps.  

##### 1. Install dependent libraries in the project root
  ```bash  
  $ ./scripts/setup_libs.bat [-mingw/-msvc] [32/64] [-update (optional)]
  ```  

##### 2. Build this project with cmake and execute it

###### Automatically (Recommended)
  ```bash
  $ ./build.bat [-debug/-release] [-mingw/-msvc] [32/64]
  $ ./debug/win-vind.exe
  ```

###### Manually (Visual Studio 2019)
  ```bash
  $ cmake -B build -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A x64 -DBIT_TYPE=64 .
  $ cmake --build build --config Debug
  $ ./debug/Debug/win-vind.exe
  ```

###### Manually (MinGW-w64)
  ```bash
  $ cmake -B debug -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DBIT_TYPE=64 .
  $ cmake --build debug --config Debug
  $ ./debug/win-vind.exe
  ```

#### Run Test 

###### Automatically (Recommended)
  ```bash
  $ ./build.bat -test
  ```

###### Manually (Visual Studio 2019)
  ```bash
  $ cmake -B test/build -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" test
  $ cmake --build test/build
  $ cd test/build ; ctest -C Debug ; cd ../..
  ```

###### Manually (MinGW-w64)
  ```bash
  $ cmake -B test/build -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" test
  $ cmake --build test/build
  $ cd test/build ; ctest -C Debug ; cd ../..
  ```

You can refer to ToDo at <a href="https://github.com/pit-ray/win-vind/projects/2">Projects/win-vind</a> and its architecture at <a href="devdocs/README.md">devdocs</a>.  

## Dependencies
|**Name**|**What is**|**Purpose**|**License**|
|:---:|:---:|:---:|:---:|
|[wxWidgets](https://github.com/wxWidgets/wxWidgets)|GUI framework|Create GUI for the system tray or popups.|[modified LGPL](https://github.com/wxWidgets/wxWidgets/blob/master/docs/licence.txt)|
|[nlohmann-json](https://github.com/nlohmann/json)|JSON parser|Parse json style settings|[MIT License](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT)|
|[maddy](https://github.com/progsource/maddy)|Markdown to HTML parser|Display the release note via **Check Update**|[MIT License](https://github.com/progsource/maddy/blob/master/LICENSE)|
|[doctest](https://github.com/onqtam/doctest)|Unit test framework|For basic unit test|[MIT License](https://github.com/onqtam/doctest/blob/master/LICENSE.txt)|
|[fff](https://github.com/meekrosoft/fff)|Macro-based fake function framework|To mock Windows API|[MIT License](https://github.com/meekrosoft/fff/blob/master/LICENSE)|



## License  

This software is provided by **MIT License**.  

## Author

- pit-ray  
[E-mail] pit-ray(at)outlook.com
