@chcp 65001
@if "%1" == "" (
    @echo Error: Please call this script with the version as a first argument.
    @exit
)
@if "%2" == "" (
    @echo Error: Please pass compiler type -msvc or -mingw.
    @exit
)

rmdir /s /q release_64
rmdir /s /q release_32

rmdir /s /q bin
mkdir bin

@echo Create Installer Version -------------------------------------------------------
echo 1> ".\\default_config\\instype"

call build.bat -release %2 64
cd release_64
cpack . -C Release
cd ..
copy /Y ".\\release_64\\setup*" ".\\bin\\*_%1_64bit.exe"

call build.bat -release %2 32
cd release_32
cpack . -C Release
cd ..
copy /Y ".\\release_32\\setup*" ".\\bin\\*_%1_32bit.exe"

@echo Create Zip Version ----------------------------------------------------------
echo 0> ".\\default_config\\instype"

mkdir ".\\bin\\win-vind"
mkdir ".\\bin\\win-vind\\default_config"
mkdir ".\\bin\\win-vind\\resources"

copy /Y ".\\release_64\\win-vind.exe" ".\\bin\\win-vind\\win-vind.exe"
xcopy /e /Y ".\\default_config" ".\\bin\\win-vind\\default_config"
xcopy /e /Y ".\\resources" ".\\bin\\win-vind\\resources"
powershell Compress-Archive -Path ".\\bin\\win-vind" -DestinationPath ".\\bin\\win-vind_%1_64bit".zip"

copy /Y ".\\release_32\\win-vind.exe" ".\\bin\\win-vind\\win-vind.exe"
powershell Compress-Archive -Path ".\\bin\\win-vind" -DestinationPath ".\\bin\\win-vind_%1_32bit.zip"

powershell Compress-Archive -Path ".\\bin\\setup_win-vind_%1_64bit.exe" -DestinationPath ".\\bin\\setup_win-vind_%1_64bit.exe.zip"
powershell Compress-Archive -Path ".\\bin\\setup_win-vind_%1_32bit.exe" -DestinationPath ".\\bin\\setup_win-vind_%1_32bit.exe.zip"
