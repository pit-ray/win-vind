---
layout: default
title: How to Contribute
nav_order: 4
descripption: "How to contibute win-vind."
---
# How to contribute
{: .no_toc }

<hr>  

1. TOC  
{:toc}

<hr>

## Contibute Procedure
1. Make a discussion at <a href="https://github.com/pit-ray/win-vind/discussions">github.com/discussions</a> in order to prevent conflicts. In addition, post the what you will do, just in a few words. (**Optional**)  

1. Fork the repository from <a href="https://github.com/pit-ray/win-vind">pit-ray/win-vind</a>. I recommend to clone it as depth=1 like below.  
```bash
$ git clone https://github.com/pit-ray/win-vind.git
```
1. Make a unique branch for pull requests.
```bash
$ git checkout -b UNIQUE_BRANCH_NAME
```
1. You can do something for this project.  
1. Build the project for verification.  
``` bash
$ ./create_bin.bat 0.0.0 -msvc
```
1. Please add, commit and push.
```bash
$ git add .
$ git commit -m "Please write a summary of your changes."
$ git push -u origin UNIQUE_BRANCH_NAME
```
1. Please send pull requests at <a href="https://github.com/pit-ray/win-vind/pulls">Pull requests pit-ray/win-vind</a>

<br>
<hr>

## Note  
- Your written codes are provided as **MIT License**.

<br>
<hr>

## Development Environment
I recommend to install follow softwares or libraries.  

|Name|Recommended Version|Download Link|
|:---:|:---:|:---:|
|MinGW-w64|GCC-8.1.0-x86_64-posix-seh|<a href="https://sourceforge.net/projects/mingw-w64/files/mingw-w64/">MinGW-w64 SourceForge.net</a>|
|CMake|3.14.4|<a href="https://cmake.org/download/">Download - CMake</a>|
|wxWidgets|3.1.5|<a href="https://www.wxwidgets.org/downloads/">Downloads - wxWidgets</a>|
|NSIS|3.06.1|<a href="https://nsis.sourceforge.io/Download">Download - NSIS</a>|
|Windows10 SDK|10.0.19041.0|<a href="https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/">Microsoft Windows10 SDK - Windows app development</a>|

This project use **&lt;mutex&gt;**, so some MinGW without it will fail a build. In this case, you will need to install other MinGW with <mutex>. (select posix at item called Thread in MinGW-Installer.)
    
win-vind provides some scripts to prepare the environment for development. If you have already installed MinGW-w64 or Visual Studio 2019, these scripts install needed libraries (e.g. wxWidgets) by executing following commands at a Windows command prompt.  

- MinGW-w64 (64 bit only)  
```sh
$ ./scripts/setup_libs.bat -mingw  
```

- Visual Studio 2019 (Supported 32 bit and 64 bit)
```sh
$ ./scripts/setup_libs.bat -msvc 
```
<hr>

## Build Instruction
- Automatically **(Recommended)**
```bash
$./build.bat [-debug/-release] [-mingw/-msvc] [32/64]
```
Example
```bash
$ ./build.bat -debug -msvc 64
```

- Manually
```bash
$ cmake -B debug -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A x64 -DBIT_TYPE=64 .
$ cmake --build debug --config Debug
```
<br>

<hr>

## Information for Development  

**Please read its architecture at <a href="https://github.com/pit-ray/win-vind/blob/master/devdocs/README.md">devdocs</a>.**  

All binded functions of win-vind derive from <a href="https://github.com/pit-ray/win-vind/blob/master/core/include/bind/binded_func.hpp">**BindedFunc**</a>. However, these are based on polymorphism, so recommends to derive from <a href="https://github.com/pit-ray/win-vind/blob/master/core/include/bind/binded_func_creator.hpp">**BindedFuncCreator**</a> to have a factory function.

### New KeyBinding Example  
- Make a source file and a header file into [core/include/bind/dev/](https://github.com/pit-ray/win-vind/blob/master/core/include/bind/dev) and [core/src/bind/dev/](https://github.com/pit-ray/win-vind/blob/master/core/src/bind/dev).
- Add a path of source file into [core/CMakeLists.txt](https://github.com/pit-ray/win-vind/blob/master/core/include/bind/dev).
- Define a new derived class (e.g. **MyBinding**).  

**mybinding.hpp**

```cpp  
#ifndef MY_BINDING_HPP
#define MY_BINDING_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    struct MyBinding : public BindedFuncCreator<MyBinding> {
        explicit MyBinding() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
```  
**mybinding.cpp**

```cpp  
#include "bind/dev/mybindings.hpp"

#include "bind/base/ntype_logger.hpp"
#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/def.hpp"


namespace vind
{
    MyBinding::MyBinding()
    : BindedFuncCreator("my_binding") //Give the unique identifier.
    {}
    // A one-shot function to call inside win-vind
    void MyBinding::sprocess() {
        mouse::click(KEYCODE_MOUSE_LEFT) ; //left click

        keybrd::pushup(KEYCODE_LWIN, KEYCODE_D) ; //minimize all window

        VirtualCmdLine::msgout("Hello World !") ;
    }
    // A function called by sequence commands such as `23gg`
    void MyBinding::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    // A function called by the command line style commands like `:sample`
    void MyBinding::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
```

- Please register the class into [core/src/bindings_lists.cpp](https://github.com/pit-ray/win-vind/blob/master/core/src/bind/bindings_lists.cpp).

```cpp
    MyBinding::create(),
```
  
- Assign commands to **MyBinding** in [default_config/bindings.json](https://github.com/pit-ray/win-vind/blob/master/default_config/bindings.json).

```json
    {
        "name": "my_binding",
        "cdef": ["sample"],
        "endef": [],
        "evdef": [],
        "gndef": [],
        "gvdef": [],
        "idef": [],
        "rdef": [],
        "en": "Sample",
        "ja": "Sample"
    },
```
