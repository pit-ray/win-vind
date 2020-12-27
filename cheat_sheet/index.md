---
layout: default
title: Cheat Sheet
nav_order: 5
descripption: "Command lists of win-vind."
has_children: true
---

<div id="jump_to_any">

<div id="jump_to_active_window">
    <p>It makes cursor jump center of most foreground window.</p>
</div>
<br>

<div id="switch_window">
    <p>It is equel to <b>Alt + Tab</b>.</p>
</div>
<br>

<div id="start_shell">
    <p>It starts command prompt and is equal to typing <span class="code">cmd</span> at Command Prompt. You can change started application by rewriting in <b>Preferences/Shortcut Apps/Console</b>.</p>
    <img src="https://github.com/pit-ray/pit-ray.github.io/raw/master/win-vind/imgs/config_shortapps.jpg?raw=true" width=512>
</div>
<br>
<br>
<div id="start_any_app">
    <p>It starts a registered application and you can register command to <b>Preferences/Shortcut Apps</b>.</p>
    <p>If you register <b>notepad.exe</b> as <b>notepad</b>, could launch it by typing <span class="code">:!notepad</span> like below.</p>
    <img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/cmd-demo.gif?raw=true" width="400">  
</div>

<br>
<br>
<div id="make_dir">
    <p>It has two calling ways.
    <ol>
        <li>Absolutely</li>
        If you want to make <b>TestDirectory</b> in <b>C:/ParentDirectory</b>, write like a bellow.
        <div class="code">:mkdir C:/ParentDirectory/TestDirectory</div>
        <br>
        <br>

        <li>Relatively</li>
        On the premise that the current directory are opened by <b>explorer.exe</b>, if you want to make <b>TestDirectory</b> in current directory, type as follow.
        <div class="code">:mkdir TestDirectory</div>
        By the way, It is same as <b>New folder</b> on explorer.exe.
        If no window of explorer.exe is opened, it regard the current directory as <b>~/Desktop</b>.
    </ol>
</div>
<br>
<div id="enable_targeting_of_dedicate_to_window">
    It regards a foreground window as the target for <b>Dedicate to Window</b>.
</div>
<br>
<div id="disable_targeting_of_dedicate_to_window">
    It cancels targeting for <b>Dedicate to Window</b>.
</div>
