---
layout: notoc
title: Downloads - win-vind
nav: downloads
nav: Downloads
icon: download
order: 1
disable_anchors: true
---


### Chocolatey
win-vind supports installation using [Chocolatey](https://chocolatey.org/). To install win-vind with Chocolatey, run the following command from the command line or from PowerShell.

```sh
$ choco install win-vind
```

### winget
[winget](https://github.com/microsoft/winget-cli) is also supported. To install the package, run the following command from your command line.

```sh
$ winget install win-vind
```

However, still no approval for [win-vind 4.3.2](https://community.chocolatey.org/packages/win-vind/4.3.2) on Chocolatey. Therefore, download the [Chocolatey package source](https://github.com/pit-ray/win-vind/releases/download/v4.3.2/win-vind_4.3.2_chocolatey_package_source.zip) manually and execute the following command in the extracted directory.

```sh
$ cd win-vind_4.3.2_chocolatey_package_source
$ choco install win-vind.4.3.2.nupkg
```

### Installer
The installer version creates dependencies to the installed directory and the user directory.  
[<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>32bit Installer</span></span>]({{ site.dl_ins_32 }})
[<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>64bit Installer</span></span>]({{site.dl_ins_64 }})

### Portable
The potable version does not create any files outside the unzipped root except for the startup file.  
[<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>32bit Portable</span></span>]({{ site.dl_zip_32 }})
[<span class="site-masthead__button"><i class="fas fa-download"></i>&nbsp;<span>64bit Portable</span></span>]({{ site.dl_zip_64 }})

<br>
