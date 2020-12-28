---
layout: default
title: Cheat Sheet
nav_order: 5
descripption: "Command lists of win-vind."
has_children: true
---
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
