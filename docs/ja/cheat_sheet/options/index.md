---
layout: page
title: オプションとパラメータ - win-vind
nav: オプションとパラメータ
disable_anchors: true
translation: ja
translators: pit-ray
version: 4.2.0
show_in_menu: false
---

## パラメータと初期値

|ID|型|初期値|備考|
|:---:|:---:|:---:|:---|
|`arrangewin_ignore`|str||ウィンドウの整列で無視する実行ファイル名のリスト。例えば、rainmeterとgvimを整列対象から外したい場合、`set arrangewin_ignore = rainmeter, gvim`のように書きます。名前は拡張子なしの実行ファイル名です。|
|`autofocus_textarea`|bool|false|**Editor Normalモード**に遷移したとき、自動的にマウスカーソルに最も近いテキストエリアを選択します。|
|`autotrack_popup`|bool|false|Windowsの標準オプションのうちの一つです。例えば、**このファイルをゴミ箱に移動しますか？**と表示されたときに、自動的にポップアップウィンドウへカーソルを移動させます。|
|`blockstylecaret_mode`|str|solid|ブロックスタイルのキャレットのモード。サイズ固定の`solid`モードと選択による疑似的な`flex`モードがあります。|
|`blockstylecaret_width`|num|15|solidモードのブロックスタイルキャレットの幅。|
|`blockstylecaret`|bool|false|ブロックスタイルのキャレット。|
|`charcache`|bool|false|`x`や`X`コマンドの時使われる1文字用の小さなキャッシュ。有効にすると、その都度クリップボードが開かれ、Vimと同じ動作になりますが、パフォーマンスが少し低下します。|
|`cmd_bgcolor`|str|323232|仮想コマンドラインの背景色(#は任意)。|
|`cmd_fadeout`|num|5|仮想コマンドラインのフェードアウト時間(秒)。コマンドラインを消えないようにするには、この値を十分大きなものにしてください。|
|`cmd_fontcolor`|str|c8c8c8|仮想コマンドラインの文字色(#は任意)。|
|`cmd_fontextra`|num|1|仮想コマンドラインの水平方向の文字間隔。|
|`cmd_fontname`|str|Consolas|仮想コマンドラインのフォント名。空の文字列の場合には、システムフォントが利用されます。|
|`cmd_fontsize`|num|23|仮想コマンドラインの文字サイズ。|
|`cmd_fontweight`|num|400|仮想コマンドラインの文字の太さ(最大値: 1000)。|
|`cmd_maxchar`|num|32|仮想コマンドラインの一行あたりの最大文字数。|
|`cmd_maxhist`|num|10|仮想コマンドラインで保持する履歴の最大数。|
|`cmd_roughpos`|str|LowerMid|仮想コマンドラインの大まかな位置。`UpperLeft`, `UpperMid`, `UpperRight`, `MidLeft`, `Center`, `MidRight`, `LowerLeft`, `LowerMid`, `LowerRight`のうちのどれかを指定してください。|
|`cmd_xmargin`|num|32|`cmd_roughpos`で大まかな位置を決め、ピクセル単位で水平方向の微調整を行います。|
|`cmd_ymargin`|num|64|`cmd_roughpos`で大まかな位置を決め、ピクセル単位で垂直方向の微調整を行います。|
|`cursor_accel`|num|95|マウスカーソルの等加速度運動におけるピクセルレベルの加速度。|
|`cursor_maxv`|num|12|マウスカーソルの最大速度。|
|`cursor_tweight`|num|250|マウスカーソルの等加速度運動における時間のスケーリング値。|
|`dedicate_to_window`|bool|false|このオプションを有効にすると、`enable_targeting_of_dedicate_to_window`で一つのウィンドウを選択できるようになります。その場合、そのウィンドウを選択するたびに自動的に**Editor Normalモード**遷移します。また、その他のウィンドウを選択すると、**Insertモード**に切り替わります。因みに、`disable_targeting_of_dedicate_to_window`で選択を無効化できます。言い換えれば、通常のエディタをFakeのVimとして扱うことができます。オーバヘッドはほぼありません。|
|`easyclick_bgcolor`|str|323232|EasyClickのヒントの背景色(#は任意)。|
|`easyclick_colordecay`|num|100|EasyClickでマッチングしているヒントの色減衰値 (0 ~ 255)。|
|`easyclick_fontcolor`|str|c8c8c8|EasyClickのヒントの文字色 (#は任意)。|
|`easyclick_fontname`|str|Arial|EasyClickのヒントのフォント名。|
|`easyclick_fontsize`|num|14|EasyClickのヒントの文字サイズ|
|`easyclick_fontweight`|num|500|EasyClickのヒントの文字の太さ(最大値: 1000)。|
|`gui_fontname`|str|Segoe UI|GUIのフォント名。空文字の場合には、システムフォントが利用されます。|
|`gui_fontsize`|num|11|GUIのフォントサイズ。|
|`hscroll_pageratio`|num|0.125|1ページの横のスクロール量を決めるための、画面の幅に対する1ページの比。|
|`hscroll_speed`|num|10|マウスの横スクロールの速度。|
|`icon_style`|str|resources/icon32_dark.ico|タスクバーに表示されるアイコンのスタイルです。デフォルトでは、**Dark**と**Light**があり、それぞれ`resources/icon32_dark.ico`と`resouces/icon32_light.ico`で利用できます。ちなみに、`ico`形式で**32x32**であれば、お好きな画像を利用できます。|
|`initmode`|str|i|win-vindの初期モード。値はモード接頭辞です。|
|`jump_margin`|num|10|`jump_cursor_to_left`などで画面端にジャンプしたときに、マウスカーソルが画面の端から飛び出さないようにするためのピクセル単位の余白です。|
|`keybrd_layout`|str||`jump_cursor_with_keybrd_layout`で参照されるキーボードのレイアウトkmpファイルのパスです。デフォルトでは、**US (101/102)配列**または**JP (106/109)配列**のみがサポートされています。この値を空にすると、現在のシステム設定に応じて、自動的にKMPファイルが選択されます。もし、お使いのキーボードが対応していない場合には、KMPファイルを作成し、そのパスを値として利用できます。|
|`shell`|str|cmd|`:!`コマンドに使用するシェルの名前。|
|`shell_startupdir`|str||`:shell`や`:!`などのコマンドにおけるシェルの現在ディレクトリです。これらのコマンドでは、エクスプローラウィンドウを選択していればそのディレクトリ、それ以外はユーザディレクトリが現在ディレクトリとなりますが、値を設定している場合にはそのディレクトリに固定されます。|
|`shellcmdflag`|str|-c|`:!`コマンドを実行する際にシェルに渡すフラグ。|
|`suppress_for_vim`|bool|false|実行ファイル名に**VIM**という文字列を含むアプリケーションを選択すると、自動的に**Residentモード**へ移行する機能です。これにより、Vimと同じキーバインディングを持つアプリケーションと干渉することなく、スムーズに移行することができます。|
|`uiacachebuild`|bool|false|EasyClickと`autofocus_textarea`は、呼び出された後にUIをスキャンするため、動作が遅くなります。このオプションを有効にすると、スキャンを非同期的に行い、キャッシュを用いた処理が行われます。キャッシュを利用することで、従来の線形なスキャンに比べ30倍以上高速化しますが、検出精度が低下します。|
|`uiacachebuild_lifetime`|num|1500|キャッシュの有効期限 (ms)。高い値にすると、計算コストが減少し、キャッシュの信頼度が低下します。低い値にすると、キャッシュを頻繁に作成し、信頼性が保証されます。|
|`uiacachebuild_staybegin`|num|500|マウスカーソルが停止してからキャッシュを作り始めるまでの時間(ms)です。キャッシュの生成における不必要な計算コストを削減するためには、操作がないときにはキャッシュを生成せず、マウスが停止した直後にのみキャッシュを更新する必要があります。言い換えれば、この値はマウスカーソルが停止したとみなされる時間(ms)です。[appendix](#overview-of-stay-range-in-uiacachebuild)をご参照ください。|
|`uiacachebuild_stayend`|num|2000|一定時間以上操作されていない場合には、キャッシュを作成すべきではありません。このオプションの値は、カーソルが停止してからキャッシュの作成を終えるまでの時間(ms)です。[appendix](#overview-of-stay-range-in-uiacachebuild)をご参照ください。|
|`vcmdline`|bool|true|仮想コマンドライン。|
|`vscroll_pageratio`|num|0.125|1ページの縦のスクロール量を決めるための、画面の高さに対する1ページの比。|
|`vscroll_speed`|num|30|マウスの縦スクロールの速度。|
|`window_accel`|num|95|winresizerにおけるウィンドウの等加速度運動の加速度 (ピクセル)。|
|`window_hdelta`|num|100|ウィンドウのリサイズにおける水平方向の間隔。|
|`window_maxv`|num|12|winresizerにおけるウィンドウの最大速度。|
|`window_tweight`|num|250|winresizerにおける等加速度運動の時間のスケーリング値。|
|`window_vdelta`|num|100|ウィンドウのリサイズにおける垂直方向の間隔。|
|`winresizer_initmode`|num|0|winresizerの初期モード([0]: リサイズ, [1]: 移動, [2]: フォーカス)。|


## Appendix

### Overview of stay range in `uiacachebuild`
<p align="center">
<img src="{{ site.url }}/imgs/uiacachebuild_stay.png" width=600>  
<p align="center">uiacachebuild_staybegin and uiacachebuild_stayend overview</p>
</p>


<br>
<br>
<br>
<br>
