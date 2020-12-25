---
layout: default
title: How to contribute
nav_order: 4
descripption: "How to contibute win-vind."
---
# How to contribute

<hr>

## Contibute Procedure
1. Make a discussion at <a href="https://github.com/pit-ray/win-vind/discussions">github.com/discussions</a> in order to prevent conflicts. In addition, post the what you will do, just in a few words.  

1. Clone the repository from <a href="https://github.com/pit-ray/win-vind">pit-ray/win-vind</a>. For example, you do a follow command.
    <div class="code"><p>$git clone https://github.com/pit-ray/win-vind.git</p></div>

1. Make a unique branch for pull requests.
```bash
$ git checkout -b UNIQUE_BRANCH_NAME
```
1. You can do something for this project.  
1. Build project for release in root directory.
``` bash
$ ./push_preproc.bat 0.0.0</p></div>
```
1. If you used third party libraries, you must add some essential texts in <a href="https://github.com/pit-ray/win-vind/blob/master/THIRD_PARTY_LICENSES.txt">**THIRD_PARTY_LICENSES.TXT**</a>.  
1. Please add, commit and push.
```bash
$ git add .
$ git commit -m "Please write a summary of your changes."
$ git push -u origin UNIQUE_BRANCH_NAME
```
1. Please send pull requests at <a href="https://github.com/pit-ray/win-vind/pulls">Pull requests pit-ray/win-vind</a></li>

<br>
<hr>

## Note  
- Your written codes are provided as **MIT License**.


<br>
<hr>

## Development Environment
I recommend to install follow softwares or libraries.  

|Name|Minimum Version|Download Link|
|:---:|:---:|:---:|
|MinGW-w64|GCC-8.1.0-x86_64-posix-seh|<a href="https://sourceforge.net/projects/mingw-w64/files/mingw-w64/">MinGW-w64 SourceForge.net</a>|
|CMake|3.14.4|<a href="https://cmake.org/download/">Download - CMake</a>|
|wxWidgets|3.0.5|<a href="https://www.wxwidgets.org/downloads/">Downloads - wxWidgets</a>|
|NSIS|3.06.1|<a href="https://nsis.sourceforge.io/Download">Download - NSIS</a>|

This project use **&lt;mutex&gt;**, so some MinGW without it will fail a build. In this case, you will need to install other MinGW with <mutex>. (select posix at item called Thread in MinGW-Installer.)

<hr>

## Build Instruction
- Automatically **(Recommended)**
```bash
$./build.bat [-debug/-release]
```
- Manually
```bash
$ mkdir debug
$ cd debug
$ cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ..
$ cmake --build . --config Debug
$ cd ..
```
<br>

<hr>

## Information for Development
All binded functions of win-vind derive from <a href="https://github.com/pit-ray/win-vind/blob/master/core/include/common/binded_func.hpp">**BindedFunc**</a>. However, these are based on polymorphism, so recommends to derive from <a href="https://github.com/pit-ray/win-vind/blob/master/core/include/common/binded_func_with_creator.hpp">**BindedFuncWithCreator**</a> to have a factory function. In addition, you can use some utilities in <a href="https://github.com/pit-ray/win-vind/tree/master/core/include/common">**core/include/common**</a> for developments. 

### New KeyBinding Example  
- Make a new derived class (e.g. **MyBinding**) and add files into **core/??_bindings/**.
- Add a path of source file into **core/CMakeLists.txt**.   
- Define the class.  

**mybinding.hpp**

```cpp  
//
#ifndef MY_BINDING_HPP
#define MY_BINDING_HPP
#include "binded_func_with_creator.hpp"

struct MyBinding : public BindedFuncWithCreator<MyBinding>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

#endif
```  
**mybinding.cpp**

```cpp  
#include "mybinding.hpp"
#include "virtual_cmd_line.hpp"

const std::string MyBinding::sname() noexcept
{
    return "my_binding" ; //Give the unique identifier.
}

void MyBinding::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* parent_vkclgr,
        const KeyLogger* const parent_charlgr)
{
    if(first_call) {
        MouseEventer::click(VKC_MOUSE_LEFT) ; //left click
    
        KeybrdEventer::pushup(VKC_LWIN, VKC_D) ; //minimize all window
    
        VirtualCmdLine::msgout("Hello World !") ;
    }
}
```

- Please register the class into <a href="https://github.com/pit-ray/win-vind/blob/master/core/include/bindings_lists.hpp">**core/include/bindings_lists.hpp**</a>.  

```cpp
    MyBinding::create(),
```
  
- Assign commands to **MyBinding** in <a href="https://github.com/pit-ray/win-vind/blob/master/config/bindings.json">**config/bindings.json**</a>.  

```json
    {
        "name": "my_binding",
        "guin": ["Q"],
        "guii": ["<guin>"],
        "guiv": ["<guin>"],
        "edin": ["<guin>"],
        "edii": ["<guin>"],
        "ediv": ["<guin>"],
        "edivl": ["<guin>"],
        "cmd": ["mybinding"],
        "en": "My Binding",
        "ja": "Sample"
    },
```
