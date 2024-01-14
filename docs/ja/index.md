---
layout: full
homepage: true
disable_anchors: true
title: ホーム - win-vind
nav: ホーム
show_in_menu: false
icon: home
translation: ja
translators:
  - pit-ray
version: 5.9.0
description: <img src="imgs/banner.gif" class="img-fluid">
---

## これは何ですか？
**win-vind** は、Windows のための軽量なハイブリッド UI システムを提供します。CUI と GUI の両方の特徴を持ち、このツールをインストールすることで、Vim と同じように Windows GUI を制御することができます。

### 1. Vim ユーザーフレンドリー。
すべての設定方法やモードの概念は Vim から派生しており、Vim のような UI 操作が可能です。Vim ユーザーは win-vind のマクロ機能と追加のモード概念を少しの学習コストで理解するだけで済みます。

### 2. 多くの便利な組み込みコマンドがあります。
従来のキーバインディングツールのような複雑なスクリプトや依存関係を気にする必要はありません。低レベルで最適化された組み込みコマンドを組み合わせて、自由にユーザー定義のコマンドを作成できます。

### 3. 非常に携帯性が高く完全にオープンソースです。
win-vind はユーザー権限で実行される依存関係のない小さな単一のバイナリです。また、コマンドラインからも利用でき、UI 操作のためのコマンドとして `$ win-vind -c "ggyyGp"` のように使用できます。


### トップ機能デモ

<video src="https://user-images.githubusercontent.com/42631247/215270933-3365065b-53db-4eca-9fc6-cd03d13e5ab0.mp4" controls class="img
-fluid"></video>



### 設定ファイル例

.vimrc スタイルで設定できます。.vindrc ではオプションの切り替え、パラメータの設定、低レベルのキーの再マップ、および関数のバインディングを行うことができます。

```vim
" {tiny, small, normal, big, huge} のバージョンを選択します。
version normal

" パラメータの変更
set shell = cmd
set cmd_fontsize = 14
set cmd_fontname = Consolas
set easyclick_bgcolor=E67E22
set easyclick_fontcolor=34495E

" capslock を ctrl にマップします。
imap <capslock> {<ctrl>}

" 便利なショートカットを定義します
inoremap <ctrl-shift-f> <easyclick><click_left>
inoremap <ctrl-shift-m> <gridmove><click_left>
inoremap <ctrl-shift-s> <switch_window><easyclick><click_left>

" アプリケーションの起動を登録します
noremap <ctrl-1> :! gvim<cr>
noremap <ctrl-2> :e http://example.com<cr>

" Vim のようなマクロを定義します
enoremap t ggyyGp

" オートコマンドの適用
autocmd AppLeave * <to_insert>
autocmd AppEnter,EdiNormalEnter vim.exe <to_resident>
```


## ライセンス
このソフトウェアは [MIT ライセンス](https://github.com/pit-ray/win-vind/blob/master/LICENSE.txt) によって提供されています。
