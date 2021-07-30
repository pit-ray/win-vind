# Contributing

Patches are welcome in whatever form. However, you must agree that your code will be provided by MIT License.

### To Documentation
You can contribute to the [homepage](https://pit-ray.github.io/win-vind/) by sending pull-request to the `gh-pages` branch if there is an error or a better way to describe the content. [Jekyll](https://jekyllrb.com/) is used as the framework, and can be written in markdown format. We also use [Just the Docs](https://pmarsceill.github.io/just-the-docs/) as a theme, which allows extended expressions.

### To Development
You can contribute to development by sending pull requests to the `master` branch to fix or add features, add test cases, modify typos and expressions, improve [security](https://github.com/pit-ray/win-vind/security/code-scanning). When adding new code, please consider adding test cases in the test directory to satisfy branch coverage. [Codacy](https://www.codacy.com/gh/pit-ray/win-vind/dashboard?utm_source=github.com&utm_medium=referral&utm_content=pit-ray/win-vind&utm_campaign=Badge_Grade) and [Travis](https://travis-ci.com/pit-ray/win-vind) and [Actions](https://github.com/pit-ray/win-vind/actions) run by pushing, and [Coverity](https://scan.coverity.com/projects/pit-ray-win-vind) checks programmatic resources at releasing as Continuous Integration. If you want to discuss development, please create a thread in [Discussion](https://github.com/pit-ray/win-vind/discussions) with **#Development** category.


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
