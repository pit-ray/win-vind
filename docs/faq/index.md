---
layout: page
disable_anchors: true
title: FAQ - win-vind
nav: FAQ
order: 4
icon: question
---

Typical FAQs are listed here, but if you don't see it here, please search [GitHub Issues](https://github.com/pit-ray/win-vind/issues).

## Why is the name win-vind?  
win-vind means <u>vi</u>m key b<u>ind</u>er for <u>win</u>dows.  
<br>

## I often forget the current mode. Is there a workaround?  
Please extend the display time of the virtual command line as follows.

```vim
set cmd_fadeout = 3600 " one hour visible
```

## Can I disable everything and only use EasyClick?
Yes, you can only use EasyClick without other features.
To achieve this, please refer to the following .vindrc.

```vim
version tiny
set initmode=i  " Insert mode

" Overwrite default bindings into the dummy <f20> key
imap <esc-left> <f20>
imap <ctrl-]> <f20>
imap <f8> <f20>
imap <esc-down> <f20>

" Define with your favorite bindings.
imap <ctrl-shift-space> <easyclick><click_left>

" Scan UI objects asynchronously  (Optional)
set uiacachebuild

" Hold UI cache for 5 sec. (Optional)
set uiacachebuild_lifetime=5000

" Generate a cache between 0.5 and 2.0 seconds after the operation stops.  (Optional)
set uiacachebuild_staybegin=500
set uiacachebuild_stayend=2000
```
Firstly, you remove all mapping in the initial mode with map command, which is the same way as Vim.
In the sample .vindrc, the initial mode is set to the insert mode.
Next, define the mapping for EasyClick with your favorite keys (the example maps with ctrl + shift + space).
Optionally, you can use the uiacachebuild option to scan the UI objects asynchronously and efficiently reuse the cache.

<br>
