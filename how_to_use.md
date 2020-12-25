---
layout: default
title: How to use
nav_order: 3
descripption: "Usage of win-vind"
---
<span class="sect1" id="installation">Installation</span><hr>
<ol>
    <li>Please download latest version win-vind from <a href="https://pit-ray.github.io/win-vind/downloads">Downloads</a>. </li>
    <li>Execute the installer or unzip.</li>
    <li>Start win-vind.</li>
    <li>In default, the mode is <b>GUI Nomral</b>. You can setup <span class="code">:set</span> or <span class="code">:config</span> or right-click on the system tray.</li>
    <li>In the end, you can terminate win-vind by <span class="code">:exit</span> or right-click on the system tray.</li>
</ol>

<br>
<br>
<span class="sect1" id="overview">Overview</span><hr>
<!--<img src="../imgs/mode_overview_2.jpg">-->
<p>win-vind has plenty of hotkeys and commands, but they are based on the original Vim. Some little differences are that its mode has two layers and its unique functions for GUI. Concretely, they are <b>GUI Mode</b> and <b>Editor Mode</b>. </p>

<!--<img src="../imgs/GUIandEditor.jpg">-->
<ul>
    <li>GUI Mode</li>
    <p>The target is the mouse cursor. For instance, if you input h, moves it to left. However, it does not have a concept called rows and columns, so I implemented a wealth of alternative functions.</p>


<li>Editor Mode</li>
<p>It enables to emulate Vim when using general text editors, for instance, notepad or Microsoft Office Word or some Web forms. The target is each caret of text controls. In other words, you can operate a text editor that mainly control by a mouse with key binds of the original Vim.</p>
</ul>
<br>
<br>
<span class="sect1" id="note">Note</span><hr>
<ul>
    <li><span class="code">F8 + F9</span> is safe forced termination.</li>
    <li>win-vind <b>cannot</b> operate some windows given high-rank authorization than itself. For example, if you start Task Manager having the highest authorization and select its window, you cannot move, click or scroll the mouse cursor by win-vind. If you want to operate all windows, I recommend giving win-vind the administrator authorization. (Please use <b>Task Scheduler</b>.)</li>
</ul>
<br>
<br>

<span class="sect1" id="bindings">Bindings</span><hr>
<p>You can customize all key bindings by GUI based settings or rewriting JSON settings file easily.</p>
<ul>
    <li>GUI based</li>
    <br>
    <img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/bind_lits.jpg?raw=true">
    <p>There are settings in <b>Preferences</b> in the system tray.<p>


    <li>Rewriting JSON</li>
    If you use the installer, the file is located in <b>~/.win-vind/bindings.json</b>.<br>
    If you get zip-version, it is existed in <b>win-vind/config/bindings.json</b>.
</ul>
<br>
<br>

<span class="sect2">Syntax</span>
<ul>
    <li><span class="code">[key1](+)[key2]</span> means typing key1 and key2 with same timing.</li>
    <li><span class="code">[key1]-&gt;[key2]</span> means typing key2 after key1. </li>
    <li>It distinguished between upper and lowercase letters. For example, <span class="code">a</span> means <span class="code">a</span>, <span class="code">A</span> does <span class="code">Shift+a</span>.</li>
    <li>If you want to use system keys like Shift or Ctrl, please write KMP key code.</li>
    <br>
    <div class='kmp_list'></div>
</ul>
<br>
<span class="sect1" id="details_of_funcs">Details of Functions</span><hr>
<ul>
    <li id="jump_to_any"></li>
    <li id="jump_to_active_window"></li>
    <li id="start_shell"></li>
    <li id="start_any_app"></li>
    <li id="make_dir"></li>
    <li id="enable_targeting_of_dedicate_to_window"></li>
    <li id="disable_targeting_of_dedicate_to_window"></li>
</ul>

<span class="sect1" id="options">Options</span><hr>
<div class="sect2" id="autotrack_popup">Autotrack Popup</div>
<p>For example, if shown <b>Are you sure you want to move this file to the Recycle Bin?</b>, it automatically moves the cursor to popup.</p>
<br>  
<div class="sect2" id="dedicate_to_window">Dedicate To One Window</div>
<p>If you focus on a selected window, win-vind is only validated on its window. Concretely, if you select a target window, it changes mode to <b>Editor Normal Mode</b>. Also, selected other windows, it changes to <b>GUI Insert Mode</b>.  You can switch targeting by <a href="https://pit-ray.github.io/win-vind/cheat_sheet/#enable_targeting_of_dedicate_to_window">Enable Targeting</a> and <a href="https://pit-ray.github.io/win-vind/cheat_sheet/#disable_targeting_of_dedicate_to_window">Disable Targeting</a>.</p>
<br>
<div class="sect2" id="virtual_cmd_line">Virtual Command Line</div>
<p>It makes inputed commands to display and the current mode on the screen.</p>
