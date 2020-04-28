# win-vind
Vim Key Binder for Windows
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/win-vind-icon.png?raw=true" width="256" height="256" align="right">  

## Description
Vim, Vi is  a powerful light-weight awesome editor, has the idia of OSS. These editor key bind has been inherited from the begining. The reason why is it is very intuitive and allow us to type keeping the positon of hand over key board without moving it to mouse.
Therefore, I implemented key bind of Vim for manipulating Windows.
This software provides key bind like Vim and original useful shortcut system and is light-weight.

The most attractive things is that you can start any application with the "!" command according to the commands registered in "config/exapp.ini".

However, if you install by installer, it makes ".win-vind" directory at Home directory instead of "config" directory.


## Build instructions
recommend executing build.bat  
``` shell
./build.bat [-debug/-release]
```

If you want to manualy build, do as follows.
```shell
mkdir debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ..
mingw32-make -f Makefile
cd ..
```
If you want to make installer, please as follows.
```shell
cd debug
cpack .
cd ..
```

## For Developer
- This licence is MIT.
- This GUI is a temporary and simply by Win32API, therefore will be replace by a GUI(For example, Qt...) in the future.
- It is designed by Polymorphism and Pimpl idiom. Base classes are BindedFunction or BindedFunctionWithCreator, Command or CommandWithCreator, DynamicOption. First class is called by inputting short key command shown in config/default.xml. For example, gg is jumping to top. Next class is done by :command, for example, :w is saving file. In the end, Dynamic Option Class use in order to change on running process, use dynamically resource need to refresh resource. For example, display inputed command. Please register BindedFunction and Command to KeyBinder, Option to OptionLoader.


## Development Enviroment
||Name and Version|
|---|---|
|OS|Windows 10 Home 64bit 1909|
|Framework|MinGW-W64-builds-4.3.5|
|Compiler|GNU GCC 8.20|
|Library|Boost 1.72.0, Win32API, STL|
|CPU|AMD Ryzen 2600|

## Author
**pit-ray**  
**[E-mail]** contact(at)pit-ray.com
