# Contributing

Patches are welcome in whatever form. However, you must agree that your code will be provided by MIT License.

### To Documentation
You can contribute to the [homepage](https://pit-ray.github.io/win-vind/) by sending pull-request to the `docs` directory if there is an error, a better way to describe the content, or writing a translation. [Jekyll](https://jekyllrb.com/) is used as the framework, and can be written in markdown format. We also use [jekyll-docs-theme](https://github.com/allejo/jekyll-docs-theme) as a theme, which allows extended expressions.  

#### Add translation of document.
The win-vind documentation has the potential to easily add manual translation and manage continuously. A Japanese translation is available as a sample in [here](docs/ja). Please use it as a reference when translating. The following are the steps for translation.  

1. Create a translation directory (e.g. `fr`). 
   ```sh
   $ ./tools/create_trans.bat fr
   ```

1. Add the following to the headers for each file.
   |key|Notes|
   |:---:|:---|
   |translation|Specify the name of the directory you just created. This will be used for language selection.|
   |translators|Write your GitHub id. It will appear at the bottom of the page. If more than one person translates the document due to revisions, give them in list form.|
   |version|Write the version that corresponds to the document. If a newer version than this is released, it will indicate that the content may not be up to date.|

   Like this.

   ```md
   ---
   translation: fr
   translators:
     - foo
     - bar
   version: 4.2.1
   ---

   ```

1. Add a label to [_config.yml](docs/_config.yml) to be displayed in the language selection.
   ```yml
   - translations:
     - id: fr
       label: Français
   ```

1. All that is left to do is to translate it, and use DeepL to check the content during the pull-request.  


### To Development
You can contribute to development by sending pull requests to the `master` branch to fix or add features, add test cases, modify typos and expressions, improve [security](https://github.com/pit-ray/win-vind/security/code-scanning). When adding new code, please consider adding test cases in the test directory to satisfy branch coverage. [Codacy](https://www.codacy.com/gh/pit-ray/win-vind/dashboard?utm_source=github.com&utm_medium=referral&utm_content=pit-ray/win-vind&utm_campaign=Badge_Grade) and [Travis](https://travis-ci.com/pit-ray/win-vind) and [Actions](https://github.com/pit-ray/win-vind/actions) run by pushing, and [Coverity](https://scan.coverity.com/projects/pit-ray-win-vind) checks programmatic resources at releasing as Continuous Integration. If you want to discuss development, please create a thread in [Discussion](https://github.com/pit-ray/win-vind/discussions) with **#Development** category.

You can see the documentation for the high-layers of the architecture in [devdocs](https://github.com/pit-ray/win-vind/blob/master/src/README.md).


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
  $ cmake -B build -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A x64 .
  $ cmake --build build --config Debug
  $ ./debug/Debug/win-vind.exe
  ```

###### Manually (MinGW-w64 >= 8.2.0)
  ```bash
  $ cmake -B debug -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" .
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
  $ cmake -B tests/build -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" tests
  $ cmake --build tests/build
  $ cd tests/build ; ctest -C Debug ; cd ../..
  ```

###### Manually (MinGW-w64 >= GCC 11.2.0)
  ```bash
  $ cmake -B tests/build -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" tests
  $ cmake --build tests/build
  $ cd tests/build ; ctest -C Debug ; cd ../..
  ```

#### Make Installer
```bash
$ ./tools/create_assets.bat 1.0.0 -msvc 64
```

You can refer to ToDo at <a href="https://github.com/pit-ray/win-vind/projects/2">Projects/win-vind</a> and its architecture at <a href="src/README.md">devdocs</a>.  


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
|[pydirectinput](https://github.com/learncodebygaming/pydirectinput)|Mouse and keyboard automation for Windows|To emulate inputs for runtime tests|[MIT License](https://github.com/learncodebygaming/pydirectinput/blob/master/LICENSE.txt)|
