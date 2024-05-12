<h1></h1>
<p align="center">
  <img src="docs/imgs/banner.gif?raw=true" />
  <p align="center">
    <a href="https://scan.coverity.com/projects/pit-ray-win-vind"><img alt="Coverity Scan Build Status" src="https://img.shields.io/coverity/scan/pit-ray-win-vind?style=flat-square" /></a>
    <a href="https://www.codacy.com/gh/pit-ray/win-vind/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=pit-ray/win-vind&amp;utm_campaign=Badge_Grade"><img src="https://img.shields.io/codacy/grade/8f2e6f2826904efd82019f5888574327?style=flat-square" /></a>
    <a href="https://github.com/pit-ray/win-vind/actions/workflows/codeql-analysis.yml"><img src="https://img.shields.io/github/actions/workflow/status/pit-ray/win-vind/codeql-analysis.yml?branch=master&label=CodeQL&logo=github&style=flat-square" /></a>
    <a href="https://github.com/pit-ray/win-vind/actions/workflows/test.yml"><img src="https://img.shields.io/github/actions/workflow/status/pit-ray/win-vind/test.yml?branch=master&label=test&logo=github&style=flat-square" /></a>
    <br>  
    <a href="https://github.com/pit-ray/win-vind/actions/workflows/coverity.yml"><img src="https://img.shields.io/github/actions/workflow/status/pit-ray/win-vind/coverity.yml?branch=master&label=cov-build&logo=github&style=flat-square" /></a>
    <a href="https://github.com/pit-ray/win-vind/actions/workflows/mingw.yml"><img src="https://img.shields.io/github/actions/workflow/status/pit-ray/win-vind/mingw.yml?branch=master&label=MinGW%20build&logo=github&style=flat-square" /></a>
    <a href="https://github.com/pit-ray/win-vind/actions/workflows/msvc.yml"><img src="https://img.shields.io/github/actions/workflow/status/pit-ray/win-vind/msvc.yml?branch=master&label=MSVC%20build&logo=github&style=flat-square" /></a>
    <br>
    <a href="https://community.chocolatey.org/packages/win-vind"><img src="https://img.shields.io/badge/chocolatey-supported-blue?style=flat-square&color=607D8B" /></a>
    <a href="https://github.com/microsoft/winget-pkgs/tree/master/manifests/p/pit-ray/win-vind"><img src="https://img.shields.io/badge/winget-supported-blue?style=flat-square&color=607D8B" /></a>
    <a href="https://github.com/ScoopInstaller/Extras/commits/master/bucket/win-vind.json"><img src="https://img.shields.io/badge/scoop-supported-blue?style=flat-square&color=607D8B" /></a>
  </p>
</p>  

## What is this?
**win-vind** provides a lightweight hybrid UI system of CUI and GUI for Windows.  
By installing this tool, you will be able to control the Windows GUI in the same way as Vim.

### 1. Vim-user friendly.
All configuration methods and mode concepts are derived from Vim, allowing for Vim-like UI operation.  
Vim users only need to understand win-vind's macro features and additional mode concepts with little learning cost.  

### 2. There are many useful built-in commands.
You don't have to worry about complicated scripts and dependencies like with traditional key binding tools.  
You can freely create user-defined commands by combining low-level optimized built-in commands.

### 3. Very portable and fully open source.
win-vind is a small single binary with no dependencies that runs with user permissions.   
It is also available from the command line as a command for UI operations like `$ win-vind -c "ggyyGp"`.


### Top Feature Demo

<div><video controls src="https://user-images.githubusercontent.com/42631247/215270933-3365065b-53db-4eca-9fc6-cd03d13e5ab0.mp4" muted="false"></video></div>

<br>  

## Installation
win-vind supports a variety of installation methods.

### [Chocolatey](https://chocolatey.org/)

```sh
$ choco install win-vind
```

### [winget](https://github.com/microsoft/winget-cli)

```sh
$ winget install win-vind
```

### [Scoop](https://scoop.sh/)
This has been added to Scoop Extras thanks to contributors and is automatically updated by the autoupdate feature of scoop.

```
$ scoop bucket add extras
$ scoop install win-vind
```

### Executable Installer
- [win-vind_5.12.0_32bit_installer.zip](https://github.com/pit-ray/win-vind/releases/download/v5.12.0/win-vind_5.12.0_32bit_installer.zip)
- [win-vind_5.12.0_64bit_installer.zip](https://github.com/pit-ray/win-vind/releases/download/v5.12.0/win-vind_5.12.0_64bit_installer.zip)

### Portable Zip
- [win-vind_5.12.0_32bit_portable.zip](https://github.com/pit-ray/win-vind/releases/download/v5.12.0/win-vind_5.12.0_32bit_portable.zip)
- [win-vind_5.12.0_64bit_portable.zip](https://github.com/pit-ray/win-vind/releases/download/v5.12.0/win-vind_5.12.0_64bit_portable.zip)


## Usage

Usage is mainly described in easy-to-read [documentation pages](https://pit-ray.github.io/win-vind/usage/) with plenty of diagrams. Here, only a sample .vindrc is shown below. As you can see, it is Vim user-friendly.  

### .vindrc sample  

```vim
" Choose the version of {tiny, small, normal, big, huge}.
version normal

" Change parameters
set shell = cmd
set cmd_fontsize = 14
set cmd_fontname = Consolas
set easyclick_bgcolor=E67E22
set easyclick_fontcolor=34495E

" Map capslock to ctrl.
imap <capslock> {<ctrl>}

" Define useful shortcuts
inoremap <ctrl-shift-f> <easyclick><click_left>
inoremap <ctrl-shift-m> <gridmove><click_left>
inoremap <ctrl-shift-s> <switch_window><easyclick><click_left>

" Register application launchers
noremap <ctrl-1> :! gvim<cr>
noremap <ctrl-2> :e http://example.com<cr>

" Define macros like Vim
enoremap t ggyyGp

" Apply auto-commands
autocmd AppLeave * <to_insert>
autocmd AppEnter,EdiNormalEnter vim.exe <to_resident>
```

### Default Mappings
win-vind has many features, but you can use a simpler one for your purposes by putting the `version` command at the beginning of your .vindrc.
If you want to use `tiny` version, write the following. It is important to note that **nothing but comments can be written before the `version` command**.
If you do not write the `version` command, `huge` will be loaded.

```vim
" Only comments can be written in here.
version tiny
" Any command can be written from.
" For example
set shell = cmd
```

|**Tier**|**Supported Features**|
|:---|:---|
|[tiny](https://pit-ray.github.io/win-vind/cheat_sheet/defaults/tiny)|+mouse +syscmd|
|[small](https://pit-ray.github.io/win-vind/cheat_sheet/defaults/small)|+mouse +syscmd +window +process|
|[normal](https://pit-ray.github.io/win-vind/cheat_sheet/defaults/normal)|+mouse +syscmd +window +process +vimemu|
|[big](https://pit-ray.github.io/win-vind/cheat_sheet/defaults/big)|+mouse +syscmd +window +process +vimemu +hotkey +gvmode|
|[huge](https://pit-ray.github.io/win-vind/cheat_sheet/defaults/huge)|+mouse +syscmd +window +process +vimemu +hotkey +gvmode +experimental|

Following Vim, there are five tiers. `tiny` has minimal commands for mouse moving and clicking, including EasyClick and GridMove. `small` allows more flexible handling of window controls and process launches, etc. `normal` has Vim emulation mappings and allows text editing in text areas, etc. `big` adds several hotkeys that redefine some of the shortcut keys in Windows to operate Windows with more Vim-like ways. It also provides GUI Visual Mode (+gvmode), which allows for holding down the mouse. `huge` allows win-vind to have experimental features for more complex operations.

For more information on default mappings, please visit our [website](https://pit-ray.github.io/win-vind/cheat_sheet/defaults/).


## Known Issues
- EasyClick does not seem to work properly for some applications on older Windows 10 before 1803. The cause is not known, but we have confirmed that it works after 1909. ([#11](https://github.com/pit-ray/win-vind/issues/11))
- Windows 10/11 Single Language does not seem to be able to map toggle keys such as `<Capslock>`. ([#40](https://github.com/pit-ray/win-vind/issues/40))

- If you want to use word motion (e.g. `w`, `B`, `e`) in MS Office Word, it is recommended to disable `Use smart paragraph selection`.  

  <img src="docs/imgs/opt_in_ms_word.png" title="Option in MS Word" width=600>

## Build
All you have to do is install [cmake](https://cmake.org/download/), [Visual Sudio](https://visualstudio.microsoft.com/ja/downloads/) and [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/), then do the following.

```bash
$ cmake -B build
$ cmake --build build
$ ./build/Debug/win-vind.exe
```

If you want to build with MinGW or create an installer, see [here](https://github.com/pit-ray/win-vind/blob/master/CONTRIBUTING.md#to-development).

## Test
Tests can be run using ctest and python.

### Unit Test
Run source code level tests.

```bash
$ cd tests
$ cmake -B build unit
$ cmake --build build
$ ctest -C Debug --test-dir build --output-on-failure
$ cd ..
```

### Runtime Test
Runtime tests check the behavior of the built binaries.

```bash
$ cd tests
$ python runtime/test.py "../bin_64/win-vind/win-vind.exe"  # Specifies the binary for release.
$ cd ..
```

See [this document](tests/README.md) for details.

## Contribute
If you would like to contribute to win-vind, see [CONTRIBUTING.md](https://github.com/pit-ray/win-vind/blob/master/CONTRIBUTING.md).

The project is managed with <a href="https://github.com/users/pit-ray/projects/2">GitHub Projects</a>. 

## Similar Projects
- [rcmdnk/vim_ahk](https://github.com/rcmdnk/vim_ahk)
- [microsoft/PowerToys](https://github.com/microsoft/PowerToys)
- [zsims/hunt-and-peck](https://github.com/zsims/hunt-and-peck)

## License  

This software is provided by pit-ray under the **MIT License**.  
