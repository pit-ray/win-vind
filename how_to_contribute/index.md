---
layout: default
title: How to contribute
nav_order: 4
descripption: "How to contibute win-vind."
---
# How to contribute

## Contibute Procedure
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
- **Your written codes are provided as MIT License.
- If you used third party libraries, you must add some essential texts in <a href="https://github.com/pit-ray/win-vind/blob/master/THIRD_PARTY_LICENSES.txt">**THIRD_PARTY_LICENSES.TXT**</a> before pull requests.
- You're most welcomed! All you need is some passions for win-vind.

<br>
<hr>

## Development Environment
You must install follow softwares or libraries.  

|Name|Minimum Version|Download Link|
|:---:|:---:|:---:|
|MinGW-w64|GCC-8.1.0-x86_64-posix-seh|<a href="https://sourceforge.net/projects/mingw-w64/files/mingw-w64/">MinGW-w64 SourceForge.net</a>|
|CMake|3.14.4|<a href="https://cmake.org/download/">Download - CMake</a>|
|wxWidgets|3.0.5|<a href="https://www.wxwidgets.org/downloads/">Downloads - wxWidgets</a>|
|NSIS|3.06.1|<a href="https://nsis.sourceforge.io/Download">Download - NSIS</a>|

This project use **&lt;mutex&gt;**, so some MinGW without it will fail a build. In this case, you will need to install other MinGW with <mutex>. (select posix at item called Thread in MinGW-Installer.)

<hr>

## Build Instruction
- Automatically *(Recommended)*
```bash
$./build.bat [-debug/-release]
```
- Manually
```bash
$ mkdir debug
$ cd debug
$ cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ..
$ mingw32-make -f Makefile
$ cd ..
```
<br>

## Class Information
There are six base classes in win-vind as follow.
<table class="long">
    <tbody>
        <tr>
            <th>Pure Base Class</th>
            <th><div>Base Class With Creator</div></th>
            <th>Function Examples</th>
        </tr>
        <tr>
            <td>KeyBinding</td>
            <td><div>KeyBindingWithCreator</div></td>
            <td><span class="code">d-&gt;d</span>, <span class="code">h</span>, <span class="code">j</span>, <span class="code">k</span>, <span class="code">l</span></td>
        </tr>
        <tr>
            <td>Command</td>
            <td><div>CommandWithCreator</div></td>
            <td><span class="code">:exit</span>, <span class="code">:close</span>, <span class="code">:mkdir</span></td>
        </tr>
        <tr>
            <td>DynamicOption</td>
            <td><div>DynamicOptionWithCreator</div></td>
            <td>Autotrack Popup</td>
        </tr>
    </tbody>
</table>

<p><b>With Creator</b> means having some factory functions. If you make some new functions, you must register to <b>include/system/key_binder_list.hpp</b>, so I recommend to use <b>With Creator</b> base classes in order to register easily.</p>

<div class="sect2">New KeyBinding Example</div>
<ol>
    <li>Make a new derived class (e.g. <b>MyBinding</b>) and add files into <b>system/***_bindings/</b>.</li>
    <p><b>my_binding.hpp</b></p>
<pre class="prettyprint"><code class="lang-cpp">#ifndef _TEST_BINDING_HPP
#define _TEST_BINDING_HPP
#include "key_binding_with_creator.hpp"

struct MyBinding : public KeyBindingWithCreator&lt;MyBinding&gt;
{
static bool sprocess(const bool first_call) ;
static const std::string sname() noexcept ;
} ;
#endif
</code></pre>

    <p><b>my_binding.cpp</b></p>
<pre class="prettyprint"><code class="lang-cpp">#include "my_binding.hpp"

const std::string MyBinding::sname() noexcept
{
return "my_binding" ;
}
bool MyBinding::sprocess(const bool first_call)
{
//Do something
return true ;
}
</code></pre>
<li>You must register <b>MyBinding</b> to <b>include/system/key_binder_list.hpp</b></li>
<li>Please add the unique identifier to <b>default_config/bindings.json</b>.</li>
<pre class="prettyprint"><code class="lang-json">{
"name": "my_binding",
"key": ["q-&gt;c"],
"cmd": [],
"en": "My Binding"
}
</code></pre>
</ol>
<br>
<br>
<br>
