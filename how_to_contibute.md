---
layout: default
title: How to contribute
nav_order: 4
descripption: "How to contibute win-vind."
has_children: true
---
<span class="sect1">Contibute Procedure</span><hr>
<ol>
    <li>Clone the repository from <a href="https://github.com/pit-ray/win-vind">pit-ray/win-vind</a>. For example, you do a follow command.</li>
    <div class="code"><p>$git clone https://github.com/pit-ray/win-vind.git</p></div>

    <li>Make a unique branch for pull requests.</li>
    <div class="code"><p>$git checkout -b UNIQUE_BRANCH_NAME</p></div>
    <li>You can do something for this project.</li>
    <li>Build project for release in root directory.</li>
    <div class="code"><p>$./push_preproc.bat 0.0.0</p></div>
    <li>Please add, commit and push.</li>
    <div class="code"><p>$git add .</p></div>
    <div class="code"><p>$git commit -m "Please write a summary of your changes."</p></div>
    <div class="code"><p>$git push origin UNIQUE_BRANCH_NAME</p></div>
    <li>Please send pull requests at <a href="https://github.com/pit-ray/win-vind/pulls">Pull requests pit-ray/win-vind</a></li>
</ol>

<br>
<span class="sect1">Note</span><hr>
<ul>
    <li><span class="important">Your written codes are provided as <b>MIT License</b></span></li>
    <li><span class="important">If you used third party libraries, you must add some essential texts in <a href="https://github.com/pit-ray/win-vind/blob/master/THIRD_PARTY_LICENSES.txt"><b>THIRD_PARTY_LICENSES.TXT</b></a> before pull requests.</span></li>
    <li>You're most welcomed! All you need is some passions for win-vind.</li>
</ul>

<br>
<span class="sect1">Development Environment</span><hr>
<p>You must install follow softwares or libraries.</p>
<table class="long">
    <tbody>
        <tr>
            <th>Name</th>
            <th>Minimum Version</th>
            <th>Download Link</th>
        </tr>
        <tr>
            <th>MinGW-w64</th>
            <td>GCC-8.1.0-x86_64-posix-seh</td>
            <td><a href="https://sourceforge.net/projects/mingw-w64/files/mingw-w64/">MinGW-w64 SourceForge.net</a></td>
        </tr>
        <tr>
            <th>CMake</th>
            <td>3.14.4</td>
            <td><a href="https://cmake.org/download/">Download - CMake</a></td>
        </tr>
        <tr>
            <th>wxWidgets</th>
            <td>3.0.5</td>
            <td><a href="https://www.wxwidgets.org/downloads/">Downloads - wxWidgets</a></td>
        </tr>
        <tr>
            <th>NSIS</th>
            <td>3.06.1</td>
            <td><a href="https://nsis.sourceforge.io/Download">Download - NSIS</a></td>
        </tr>
    </tbody>
</table>
<p>This project use <b>&lt;mutex&gt;</b>, so some MinGW without it will fail a build. In this case, you will need to install other MinGW with <mutex>. (select posix at item called Thread in MinGW-Installer.)</p>

<br>
<span class="sect1">Build Instruction</span><hr>
<ul>
    <li>Automatically <span class="important">(Recommended)</span></li>
    <div class="code"><p>$./build.bat [-debug/-release]</p></div>
    <li>Manually</li>
    <div class="code">
        <p>$mkdir debug</p>
        <p>$cd debug</p>
        <p>$cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ..</p>
        <p>$mingw32-make -f Makefile</p>
        <p>$cd ..</p>
    </div>
</ul>

<br>
<span class="sect1">Class Information</span><hr>
<p>There are six base classes in win-vind as follow.</p>
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
