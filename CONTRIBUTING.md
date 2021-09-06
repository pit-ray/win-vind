# Contributing

Patches are welcome in whatever form. However, you must agree that your code will be provided by MIT License.

### To Documentation
You can contribute to the [homepage](https://pit-ray.github.io/win-vind/) by sending pull-request to the `gh-pages` branch if there is an error or a better way to describe the content. [Jekyll](https://jekyllrb.com/) is used as the framework, and can be written in markdown format. We also use [Just the Docs](https://pmarsceill.github.io/just-the-docs/) as a theme, which allows extended expressions.

### To Development
You can contribute to development by sending pull requests to the `master` branch to fix or add features, add test cases, modify typos and expressions, improve [security](https://github.com/pit-ray/win-vind/security/code-scanning). When adding new code, please consider adding test cases in the test directory to satisfy branch coverage. [Codacy](https://www.codacy.com/gh/pit-ray/win-vind/dashboard?utm_source=github.com&utm_medium=referral&utm_content=pit-ray/win-vind&utm_campaign=Badge_Grade) and [Travis](https://travis-ci.com/pit-ray/win-vind) and [Actions](https://github.com/pit-ray/win-vind/actions) run by pushing, and [Coverity](https://scan.coverity.com/projects/pit-ray-win-vind) checks programmatic resources at releasing as Continuous Integration. If you want to discuss development, please create a thread in [Discussion](https://github.com/pit-ray/win-vind/discussions) with **#Development** category.

You can see the documentation for the high-layers of the architecture in [devdocs](https://github.com/pit-ray/win-vind/blob/master/devdocs).


#### Quick Start for Build  
If you have already installed **MinGW-w64** or **Visual Studio 2019**, all you need is the next steps.  

##### 1. Install dependent libraries in the project root
  ```bash  
  $ ./tools/setup_libs.bat [-mingw/-msvc] [32/64] [-update (optional)]
  ```  
Since it builds all libraries with the best options for your system environment, it may take some minutes.

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

I recommend to install follow softwares or libraries.  

### Softwares
|Name|Recommended Version|Download Link|
|:---:|:---:|:---:|
|CMake|3.14.4|<a href="https://cmake.org/download/">Download - CMake</a>|
|NSIS|3.06.1|<a href="https://nsis.sourceforge.io/Download">Download - NSIS</a>|
|Windows10 SDK|10.0.19041.0|<a href="https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/">Microsoft Windows10 SDK - Windows app development</a>|

### Libraries (you can install with ./tools/setup_libs.bat)

|**Name**|**What is**|**Purpose**|**License**|
|:---:|:---:|:---:|:---:|
|[wxWidgets](https://github.com/wxWidgets/wxWidgets)|GUI framework|Create GUI for the system tray or popups.|[modified LGPL](https://github.com/wxWidgets/wxWidgets/blob/master/docs/licence.txt)|
|[nlohmann-json](https://github.com/nlohmann/json)|JSON parser|Parse json style settings|[MIT License](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT)|
|[maddy](https://github.com/progsource/maddy)|Markdown to HTML parser|Display the release note via **Check Update**|[MIT License](https://github.com/progsource/maddy/blob/master/LICENSE)|
|[doctest](https://github.com/onqtam/doctest)|Unit test framework|For basic unit test|[MIT License](https://github.com/onqtam/doctest/blob/master/LICENSE.txt)|
|[fff](https://github.com/meekrosoft/fff)|Macro-based fake function framework|To mock Windows API|[MIT License](https://github.com/meekrosoft/fff/blob/master/LICENSE)|


## Examples
All binded functions of win-vind derive from <a href="https://github.com/pit-ray/win-vind/blob/master/core/include/bind/binded_func.hpp">**BindedFunc**</a>. However, these are based on polymorphism, so recommends to derive from <a href="https://github.com/pit-ray/win-vind/blob/master/core/include/bind/binded_func_creator.hpp">**BindedFuncCreator**</a> to have a factory function.

### Making a new function
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
  
- Assign commands to **MyBinding** in [default_config/bindings.json](https://github.com/pit-ray/win-vind/blob/master/res/default_config/bindings.json).

```json
    {
        "name": "my_binding",
        "cdef": ["CallMyFunc"],
        "endef": [],
        "evdef": [],
        "gndef": ["<C-g>"],
        "gvdef": [],
        "idef": [],
        "rdef": [],
        "en": "Sample",
        "ja": "Sample"
    },
```

Once built, you can call it with `:CallMyFunc` or `<C-g>`.
