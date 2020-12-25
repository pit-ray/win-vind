---
layout: default
title: Cheat Sheet
nav_order: 5
descripption: "Command lists of win-vind."
has_children: true
---

<div id="jump_to_any">
    <p>This makes key coordinates on keyboard project coordinates of the desktop.</p>

    <br>
    <img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/Jump2Any-image.png?raw=true" width="512">
    <p>For example, it maps <span class="code">Space</span> key to the lower center. Thus, if you type <span class="code">f->Space</span>, jump the mouse cursor to lower center.</p>

    <p>However, the keyboard map is defferent according to languages, so you are able to choose the keyboard type at <b>Preferences/Settings/Common/Keyboard Type</b>.</p>
    <img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/kb_type.jpg?raw=true" width="512">
    <br>

    <p>Additionaly, positions of key are very hardware-dependent, so if you don't like default keyboard mapping, you can make optimized <b>custom.kmp</b> as follow ways.</p>
    <ol>
        <li>Choice <b>Custom</b> in <b>Preferences/Settings/Common/Keyboard Type</b>.</li>
        <li>Please open the <b>custom.kmp</b>. If you use the installer version, the file exists <b>~/.win-vind/custom.kmp</b>. If you use zip, it does <b>win-vind/config/custom.kmp</b></li>
        <li>Rewrite the kmp file.</li>
        <div class="sect3">Syntax</div>
        <div class="code">
         [Xposition]&nbsp;&nbsp;&nbsp;&nbsp;[Yposition]&nbsp;&nbsp;&nbsp;&nbsp;[KeyCode]
        </div>
        <ul>
            <li>the delimiter is some space. By the way, one space is same as multiple spaces.</li>
            <li>You must write a syntax per line.</li>
            <li><b>[Xposition]</b> and <b>[Yposition]</b> are positive values. Also, Left Upper at the real keyboard is (0, 0) and the max value of X means right-end, Y does bottom.</li>
            <li><b>[KeyCode]</b> is ASCII Code or KMP Key Code.</li>
            <div class='kmp_list'></div>
            <li>It is <b>not</b> supported <span class="code">Fn</span> Key. In addition, Toggle Key for example, <span class="code">CapsLock</span> or <span class="code">NumLock</span>, is sometimes <b>ignored</b> because kept the push-release-state of the key by hardware according to a product.</li>
            <li>Example</li>
            <!--<img src="../imgs/kmp_ex.jpg" width="512">-->
            <br>
        </ul>
    </ol>
    </p>
</div>

<br>
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
