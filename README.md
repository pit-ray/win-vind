# win-vind
Vim Key Binder for Windows
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/win-vind-icon.png?raw=true" width="256" height="256" align="right">
<br>

**win-vind** provides the light weight **Hybrid UI System of CUI and GUI**.  
<br>
GitHub wiki pages is <a href="https://github.com/pit-ray/win-vind/wiki">this</a>.  
日本語のGitHub wikiページは <a href="https://github.com/pit-ray/win-vind/wiki/%E3%83%9B%E3%83%BC%E3%83%A0-(Home-in-Japanese)">こちら</a>です。  
<br>  


## Description
Vim, Vi is a powerful light-weight awesome editor, has the idia of OSS. These editor key bind has been inherited from the begining. The reason why is it is very intuitive and allow us to type keeping the positon of hand over key board without moving it to mouse. I like this idia, so I implemented key bind of Vim for manipulating Windows. This software provides key bind like Vim and useful shortcut system and is light-weight.

Of course,  you can disable this application, by typing `i` in order to change mode to `Insert Mode`. Thus, It's not only the idea of Mouse-less, but it can also be used as an auxiliary tool for GUI operation.

The most of attractive things are that you can start any application with the `!` command according to the commands registered in `exapp.ini`.  
In addition, it makes cursor to jump any position with key-mapping of your keyboard as following image.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/Jump2Any-image.png?raw=true" width="512">
This makes key coordinates on keyboard project coordinates of the desktop.  
<br>

## How to use  
Please visit <a href="https://github.com/pit-ray/win-vind/wiki/For-Users">GitHub wiki pages</a>.  
<a href="https://github.com/pit-ray/win-vind/wiki/%E4%BD%BF%E3%81%84%E6%96%B9-(For-Users-in-Japanese)">GitHub wikiページ</a>に日本語で詳細を載せました。  
<br>  
 
## Build instructions  
### How to build Project  
**This project use `<mutex>`**, so some MinGW without it will fail a build.  
In this case, you will need to install other MinGW with `<mutex>`. (select `posix` at item called **Thread** in MinGW-Installer.)


- Automatically Build  
I recommend executing `build.bat`  
``` shell
./build.bat [-debug/-release]
```

- Manualy Build  
If you want to manualy build, use `cmake` and `make` commands as follows.
```shell
mkdir debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ..
mingw32-make -f Makefile
cd ..
```  
<br>  

### How to build Installer 
If you want to make installer, please use `cpack` command of **CMake**.  
However, the installer generator of `cpack` use **Nullsoft Scriptable Install System(NSIS)**.  
Therefore, you will need to install its software to your Windows. (<a href="https://nsis.sourceforge.io/Download">Download Link of NSIS</a>)  

**Please change `_USE_INSTALLER` to `true` in `include/path.hpp` before building the installer.**  

- Automatically Build  
I recommend executing `create_ins.bat`.    
```
./create_ins.bat
```

- Manually Build  
Move directory existed a builded binary.   
```shell
cd debug
cpack .
cd ..
```
<br>  

## For Developer
Please visit <a href="https://github.com/pit-ray/win-vind/wiki/For-Developers">GitHub wiki pages</a>.  
<a href="https://github.com/pit-ray/win-vind/wiki/%E9%96%8B%E7%99%BA%E8%80%85%E3%81%B8(For-Developers-in-Japanese)">GitHub wikiページ</a>に日本語で詳細を載せました。  
<br>  

## Development Enviroment
||Name and Version|
|---|---|
|OS|Windows 10 Home 64bit 1909|
|Framework|MinGW-W64-builds-4.3.5|
|Compiler|GNU GCC 8.20|
|Library|Boost 1.72.0, Win32API, STL|
|CPU|AMD Ryzen 2600|

## Author
- pit-ray  
[E-mail] contact(at)pit-ray.com
