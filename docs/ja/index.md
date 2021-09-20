---
layout: full
homepage: true
disable_anchors: true
title: ホーム - win-vind
nav: ホーム
show_in_menu: false
icon: home
translation: ja
translators: pit-ray
version: 4.2.0
description: Vim Key Binder for Windows
---


## このソフトウェアについて  

**win-vind**は、Vimとそのプラグインの影響を強く受けた、Windowsを操作するためのツールボックスです。


### 機能  
- 軽量なバインディング
- Vimライクなモード管理
- マウスレスのGUI操作
- どこでもVimエミュレーション
- 低レベルなキーマッピング (例: CapsLockをCtrl)
- ユーザ権限内での実行
- VimやAHKからのワンショット利用 (例: `$ win-vind -f easy_click_left`)
- タイルウィンドウマネージャ
- GUIのためのVimium/EasyMotionライクな操作


##### 動作例

<img src="{{ site.url }}/imgs/4xxdemo.gif?raw=true" title="Demo" width="624">  


##### 設定ファイルの例

.vimrcスタイルで設定できます。.vindrcではオプションの切り替え、パラメータの設定、低レベルなキーマッピング、バインディングの定義を行うことができます。


```vim
" ------------ Example ------------
" Virtual command line options
set shell = cmd
set cmd_fontsize = 14
set cmd_roughpos = LowerLeft
set cmd_maxchar = 100

" Enable block style caret
set blockstylecaret
set blockstylecaret_mode = solid

" Low-level key mapping in resident mode
rmap <capslock> <ctrl>

" Define bindings in GUI normal mode
gnnoremap <c-h> select_left_window
gnnoremap <c-l> select_right_window
gnnoremap <c-k> select_upper_window
gnnoremap <c-j> select_lower_window

" Define bindings in insert mode
imap <capslock> <f16>
inoremap <f16> to_edi_normal

imap <ralt> <f17>
inoremap <f17> easy_click_left

imap <app> <f18>
inoremap <f18> window_resizer
```

## ライセンス
このソフトウェアは[MIT License](https://github.com/pit-ray/win-vind/blob/master/LICENSE.txt)を採用しています。
