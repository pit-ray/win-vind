@chcp 65001

@if "%1" == "" (
    @echo Error: Please call this script with the version as a first argument.
    @exit
)
@if "%2" == "" (
    @echo Error: Please pass compiler type -msvc or -mingw.
    @exit
)
@if "%3" == "" (
    @echo Error: Please pass bit type
    @exit
)

if exist release_%3 (
    @rem rmdir /s /q release_%3
)

if exist bin_%3 (
    rmdir /s /q bin_%3
)

mkdir bin_%3

@echo Create Installer Version -------------------------------------------------------
echo 1 > res\default_config\instype

call build.bat -release %2 %3
cd release_%3
cpack . -C Release
cd ..
copy /Y release_%3\setup* bin_%3\*_%1_%3bit.exe

@echo Create Zip Version ----------------------------------------------------------
echo 0 > res\default_config\instype

mkdir bin_%3\win-vind
copy /Y release_%3\win-vind.exe bin_%3\win-vind\

xcopy /I /E /Y res\default_config bin_%3\win-vind\default_config
xcopy /I /E /Y res\resources bin_%3\win-vind\resources

powershell Compress-Archive -Path "bin_%3\win-vind" -DestinationPath "bin_%3\win-vind_%1_%3bit.zip"
powershell Compress-Archive -Path "bin_%3\setup_win-vind_%1_%3bit.exe" -DestinationPath "bin_%3\setup_win-vind_%1_%3bit.exe.zip"
