# win-vind
Vim Key Binder for Windows
<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/win-vind-icon.png?raw=true" width="256" height="256" align="right">  

GitHub wiki pages is <a href="https://github.com/pit-ray/win-vind/wiki">this</a>.  
<br>  


## Description
Vim, Vi is a powerful light-weight awesome editor, has the idia of OSS. These editor key bind has been inherited from the begining. The reason why is it is very intuitive and allow us to type keeping the positon of hand over key board without moving it to mouse. Therefore, I implemented key bind of Vim for manipulating Windows. This software provides key bind like Vim and original useful shortcut system and is light-weight.

Of course,  you can disable this application, by typing `i` in order to change mode to `Insert Mode`. Thus, It's not only the idea of Mouse-less, but it can also be used as an auxiliary tool for GUI operation.

The most attractive things are that you can start any application with the `!` command according to the commands registered in `exapp.ini`.  
In addition, it makes cursor to jump any position with key-mapping of your keyboard as following image.  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/Jump2Any-image.png?raw=true" width="512">
This makes key coordinates on keyboard project coordinates of the desktop.  
<br>

## How to use  
Please visit <a href="https://github.com/pit-ray/win-vind/wiki/For-Users">GitHub wiki pages</a>.  
<br>  
  
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
Please visit <a href="https://github.com/pit-ray/win-vind/wiki/For-Developers">GitHub wiki pages</a>.  
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
