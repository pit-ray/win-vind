@chcp 65001

@if "%1" == "" (
    @echo Error: Please call this script with the version as a first argument.
    @exit
)

if exist choco (
    powershell Remove-Item -Recurse -Force choco/* -Exclude *.nuspec
)
mkdir choco

@if not exist choco\tools (
    mkdir choco\tools
)

@set OUTDIR=choco\tools
@set OUTFILE=%OUTDIR%\chocolateyinstall.ps1

@type nul > %OUTFILE%

@echo $ErrorActionPreference = 'Stop';>> %OUTFILE%
@echo $toolsDir = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)">> %OUTFILE%

@echo $url = 'https://github.com/pit-ray/win-vind/releases/download/v%1/win-vind_%1_32bit_chocolatey.zip'>> %OUTFILE%
@echo $url64 = 'https://github.com/pit-ray/win-vind/releases/download/v%1/win-vind_%1_64bit_chocolatey.zip'>> %OUTFILE%

@echo $packageArgs = @{>> %OUTFILE%
@echo packageName = $env:ChocolateyPackageName>> %OUTFILE%
@echo unzipLocation = $toolsDir>> %OUTFILE%
@echo url = $url>> %OUTFILE%
@echo url64bit = $url64>> %OUTFILE%

@echo softwareName = 'win-vind*'>> %OUTFILE%


for /f "usebackq" %%A in (`checksum -t sha256 bin_32/win-vind_%1_32bit_chocolatey.zip`) do (
    set CHECKSUM32=%%A
)
@echo checksum = '%CHECKSUM32%'>> %OUTFILE%
@echo checksumType = 'sha256'>> %OUTFILE%

for /f "usebackq" %%B in (`checksum -t sha256 bin_64/win-vind_%1_64bit_chocolatey.zip`) do (
    set CHECKSUM64=%%B
)
@echo checksum64 = '%CHECKSUM64%'>> %OUTFILE%
@echo checksumType64 = 'sha256'>> %OUTFILE%

@echo }>> %OUTFILE%

@echo Install-ChocolateyZipPackage @packageArgs>> %OUTFILE%
