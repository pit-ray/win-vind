# win-vind
Vim Key Binder for Windows
<img src="https://cdn-ak.f.st-hatena.com/images/fotolife/p/pit-ray/20200428/20200428042034.png" width="256" height="256" align="right">  

## Description
Vim, Vi is  a powerful light-weight awesome editor, has the idia of OSS. These editor key bind has been inherited from the begining. The reason why is it is very intuitive and allow us to type keeping the positon of hand over key board without moving it to mouse.
Therefore, I implemented key bind of Vim for manipulating Windows.
This software provides key bind like Vim and original useful shortcut system and is light-weight.

The most attractive things is that you can start any application with the "!" command according to the commands registered in "config/exapp.ini".

However, if you install by installer, it makes ".win-vind" directory at Home directory instead of "config" directory.

## How to use
Basically, key bind is based on Vim or Vi. Original functions are `highlighted`.

|Functions|binds|Windows Shortcut Key|
|---|---|---|
|Normal Mode|Left+ESC, Right+ESC, Up+Esc, F9|-|
|Insert Mode|i|-|
|Visual Mode|v|-|
|Select All|V|Ctrl+a|
|Move Cursor Left|h, Ctrl+h, BkSpace, Left|-|
|Move Cursor Right|l, Space, Right|-|
|Move Cursor Up|k, -, gk, Ctrl+p, Up|-|
|Jump Cursor Left End|0, ^, Home, g0, g^|-|
|Jump Cursor Right End|$, End, g$|-|
|Jump Cursor Top|gg|-|
|Jump Cursor Bottom|G|-|
|Jump Cursor Horizontal Center|gm, gM|-|
|Jump Cursor Vertical Center|M|-|
|`Jump Cursor to Any by Keyboard mapping`|f, F|-|
|`Jump Cursor to Center of Active Window`|t, T|-|
|Scroll Up|Ctrl+y|-|
|Scroll Down|Ctrl+e|-|
|Scroll Mid-Page Up|Ctrl+u|-|
|Scroll Mid-Page Down|Ctrl+f|-|
|Scroll Page Up|Ctrl+b|-|
|Scroll Page Down|Ctrl+f|-|
|Scroll Left|zh, z+h|-|
|Scroll Right|zl, z+l|-|
|Scroll Mid-Page Left|zH, z+H|-|
|Scroll Mid-Page Right|zL, z+L|-|
|`Click Left`|o, O|-|
|`Click Right`|a, A|-|
|Copy|y, yy, Y|Ctrl+c|
|Paste|p, P|Ctrl+v|
|Cut|dd, D|Ctrl+x|
|Delete|x, X, Delete|Delete|
|ReDo|Ctrl+r|Ctrl+y|
|Undo|u, U|Ctrl+z|
|`Switch Virtual Desktop to Left`|gT|Ctrl+Win+Left|
|`Switch Virtual Desktop to right`|gt|Ctrl+Win+Right|
|Search Pattern|/, ?|F3|
|`Close Current Window`|(Ctrl+w)q, (Ctrl+w)c|-|
|`Switch Window`|(Ctrl+w)s|Alt+Tab|
|`Maximize Current Window`|(Ctrl+w)k, (Ctrl+w)a|Win+Up|
|`Minimize Current Window`|(Ctrl+w)j, (Ctrl+w)i|Win+Down|
|`Snap Current Window to Left (resize half of screen)`|(Ctrl+w)h, (Ctrl+w)<|Win+Left|
|`Snap Current Window to Right (resize half of screen)`|(Ctrl+w)l, (Ctrl+w)>|Win+Right|
|`Move to Next Page`|>|Alt+Right|
|`Move to Previous Page`|<|Alt+Left|

|Functions|Commands|Windows Shortcut Key|
|---|---|---|
|Save Opened File|:w, :w_f, :w_a, :w_c, :up, :wall|Ctrl+s|
|Close Opened File(Tab)|:cl, :close|Ctrl+F4|
|`Switch WIndow`|:sw, :switch|Alt+Tab|
|`Maximize Current Window`|:max, :on, :only|Win+Up|
|`Minimize Current Window`|:min, :hi, :hide|Win+Down|
|`Snap Current Window to Left (resize half of screen)`|:lsplit|Win+Left|
|`Snap Current Window to Right (resize half of screen)`|:rsplit|Win+Right|
|`Start Command Prompt`|:sh, :shell|-|
|`Start Any Application`|!|-|


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
