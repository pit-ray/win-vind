---
layout: page
disable_anchors: true
title: よくある質問 - win-vind
nav: よくある質問
order: 3
translation: ja
translators:
  - pit-ray
version: 4.3.2
icon: question
---

### win-vindの名前の由来は何ですか。
win-vindは、<u>win</u>dowsのための<u>vi</u>m key b<u>ind</u>erに由来します。
<br>

### GUI Visual Modeとは何ですか。
マウスの左ボタンの長押しを常に行うモードです。
<br>

### ESCを使わずにモード遷移をするにはどうしたらいいですか。
　ESCはあくまでデフォルトの設定であり、すべてのコマンドは自由に変更することができます。この問題を解決する方法はいくつかありますが、ここでは`noremap`による内部スコープでのキーマッピングを利用するシンプルな方法をご紹介します。例えば、win-vindに`<Casplock>`が`<Esc>`であると認識させてみます。

```vim
" Insertモードでは、Windowsにキーメッセージが渡らないように、
" いったん別のキーに低レベルマッピングを行うことをお勧めします。
imap <capslock> <f20>
inoremap <f20> <esc>

" 内部スコープでのキーマッピング
gnnoremap <capslock> <esc>
gvnoremap <capslock> <esc>
ennoremap <capslock> <esc>
evnoremap <capslock> <esc>
```
