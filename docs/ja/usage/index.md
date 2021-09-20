---
layout: page
title: 使い方 - win-vind
nav: 使い方
order: 0
icon: map-signs
translation: ja
translators: pit-ray
version: 4.2.0
disable_anchors: true
---

　このソフトウェアは、実機のWindows 10またはWindows 11のみをサポートしています。したがって、それ以前のWindowsやWineやVirtual Boxなどの仮想環境では動作しない可能性があります。問題や要望がありましたら、 [GitHub Issues](https://github.com/pit-ray/win-vind/issues)へお寄せください。日本語でも構いません。



## インストール方法
- インストーラ版かポータブル版をダウンロードします。インストーラ版はインストールするディレクトリとユーザディレクトリに依存関係を作りますが、ポータブル版はスタートアップファイルを除き、解凍後のルートディレクトリ以外に一切のファイルを生成しません。  
   
   [<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>32bit インストーラ (.exe)</span></span>]({{ site.dl_ins_32 }})
   [<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>32bit ポータブル (.zip)</span></span>]({{ site.dl_zip_32 }})  
   [<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>64bit インストーラ (.exe)</span></span>]({{site.dl_ins_64 }})
   [<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>64bit ポータブル (.zip)</span></span>]({{ site.dl_zip_64 }})
   <br>

- インストールし、win-vind.exeを実行します。

- タスクトレイにアイコンが表示されていれば適切に動作しています。
   <p align="center">
   <img src="{{ site.url }}/imgs/taskbar.jpg" width=500 >  
   <p align="center">Like this</p>
   </p>


## 備考
- `:exit`は推奨の終了コマンドです。
- `<F8> + <F9>`は安全な強制終了コマンドです。
- win-vindは自分より高い権限を持つウィンドウを操作できません。例えば、最も高位の権限を持つ**タスクマネージャ**を選択すると、win-vindによるマウスの移動、クリック、スクロールなどができなくなります。すべてのウィンドウを操作したい場合には、win-vindを管理者権限で実行することをお勧めします。その場合には、**タスクスケジューラ**をご利用ください。


## 簡単なチュートリアル

### 1. モード遷移

　基本的な概念はVimと同じですが、二つの**Normalモード**と二つの**Visualモード**と**Residentモード**があります。

<p align="center">
<img src="{{ site.url }}/imgs/mode_overview.png" width=600>  
<p align="center">デフォルトのモード構成の概要</p>
</p>

　GUIモードとEditorモードの二つのグループがあります。前者ではウィンドウやマウスの操作などを行うことができ、後者はWebページの入力フォームやMicrosoft Office WordなどでVimエミュレーションを行うことができます。  

<p align="center">
<img src="{{ site.url }}/imgs/GUIandEditor.jpg" width=700>
<p align="center">GUIモードとEditorモードの概略図</p>
</p>

　Residentモードは、Steamでのゲーム中やVimの使用中にそれらのショートカットキーと衝突するのを防ぐための退避モードです。例えば、忠実なVimエミュレーションのためにInsertモードのキーマップに`<Esc>`を追加した場合には、Residentモードに遷移することでVimの`<Esc>`によりwin-vindが呼び出されるのを防ぐことができます。

　**Insertモード**と**Residentモード**はすべてのに入力メッセージをWindowsへ伝えますが、**GUI Normalモード**、 **GUI Visualモード**、**Edi Normalモード**、**Edi Visualモード**、**Commandモード**はすべてブロックします。

<p align="center">
<img src="{{ site.url }}/imgs/mode_overview_3D.png" width=500 >  
<p align="center">モードの視覚的概念</p>
</p>

起動直後は**Insertモード**です。上の図を参考にモードの遷移を行ってみてください。


### 2. GUIの操作とウィンドウの操作

1. `<Esc-Left>`で**GUI Normalモード**に切り替えます。
1. `:!mspaint`と入力し、Microsoft ペイントを起動します。
1. `FF`コマンドにより、**EasyClick**を呼び出すことができます。  
   <p align="center">
   <img src="{{ site.url }}/imgs/EasyClickDemo.gif">
   <p align="center">EasyClickの動作例</p>
   </p>
1. 次に`<C-w>h`や`<C-w>l`でウィンドウを選択できることを確認します。
1. 最後にMicrosoft ペイントを選択し、`:close`でウィンドウを閉じます。


### 3. win-vindのカスタマイズ

　win-vindは**Run Commands**形式の設定方法を採用しています。`.vimrc`を書いたことがあれば、簡単にカスタマイズできます。

　キーマッピングには一般的に**key2key**、**keyset2keyset**、**cmd2cmd**の3つのレベルがあります。key2keyは一つのキーを他のキーに割り当てます。keyset2keysetは、`<c-s>`から`<m-h>`のように、いくつかのキーの組み合わせから、別の組み合わせに割り当てます。cmd2cmdは、`qq`から`<c-w>e`のように時系列的なコマンド入力に反応し、別のコマンドを生成するマッピング方式です。

　キーセットは、Vimと同様に`<`と`>`の内で`-`で繋ぐようにして表現します。ただし、`<Esc-b-c-a-d>`のように好きなだけ組み合わせることができます。

以下のコマンドがサポートされています。ただ、`{`と`}`自体は構文には含まれません。


|**構文**|**効果**|
|:---|:---|
|`set {option}`|オプションの値を**true**にします。|
|`set no{option}`|オプションの値を**false**にします。|
|`set {option} = {val}`|オプションのパラメータを設定します。値には文字列か浮動小数点を含む数値です。文字列には引用符は不要で、空白以外の文字以降が値として扱われます。ちなみに等号の両端の空白は無視されます。|
|`{mode}map {in-key} {out-key}`|低レベルなkey2keyマッピングを行います。定義された低レベルマップは、win-vindだけでなく、Windows全体に影響を与えます。|
|`{mode}noremap {in-cmd} {func-id}`|ファンクションを呼び出すためのマッピングを定義します。|
|`{mode}noremap {in-keyset} {out-keyset}`|win-vindのスコープでkeyset2keysetのマッピングを行います。ただし、`{func-id}`を利用した構文のほうが優先度が高いため、正確には1段階の再帰的なマッピングが生じる可能性があります。|
|`{mode}unmap {in-cmd}`|`{in-cmd}`に割り当てられたマップを削除します。|
|`{mode}mapclear`|すべてのマップを削除します。|
|`command {in-cmd} {func-id}`|ファンクションを呼び出すためのコマンドを定義します。`{in-cmd}`に`:`は不要です。|
|`delcommand {in-cmd}`|`{in-cmd}`に割り当てられたコマンドを削除します。|
|`comclear`|すべてのコマンドを削除します。|

  
`{mode}`は、[モード接頭辞]({{ site.url }}/ja/cheat_sheet/keywords/#モード接頭辞)です。また、`.vindrc`は**UTF-8**フォーマットのみがサポートされています。


それでは最後のチュートリアルを行いましょう。

1. まずは**Insertモード**に遷移します。
1. 今回は`<F8>`の**Instant GUI Normalモード**を試してみます。**Instant GUI Normalモード**は、一時的に**GUI Normalモード**へ遷移し、一度ファンクションが呼び出されたら元のモードに戻ります。
   <p align="center">
   <img src="{{ site.url }}/imgs/instant_gui_normal_mode.jpg" width=450 >  
   <p align="center">Instant GUI Normal Demo</p>
   </p>
1. `:e ~/.win-vind/.vindrc`で`.vindrc`を開きます。
   <p align="center">
   <img src="{{ site.url }}/imgs/edit_vindrc_demo.jpg" width=450 >  
   <p align="center">:e Demo</p>
   </p>
1. `.vindrc`に次のようなコマンドを書き込みます。
   ```vim
   set cmd_fontname = Times New Roman
   imap <Capslock> <ctrl>
   inoremap <Alt> easy_click_left
   ```
1. 次のように`:source`コマンドを実行すると`.vindrc`が読み込まれます。引数は不要です。
   <p align="center">
   <img src="{{ site.url }}/imgs/source_demo.jpg" width=450 >  
   <p align="center">:source Demo</p>
   </p>
1. **Insertモード**では、Ctrlの代わりにCapslockを利用でき、Altキー一つで**EasyClick**を起動できるようになります。

<br>
<br>
<br>
<br>
