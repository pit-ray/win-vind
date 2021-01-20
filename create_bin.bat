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

type nul > ".\\log\\error.log"
type nul > ".\\log\\message.log"
copy ".\\default_config\\bindings.json" ".\\config\\bindings.json"
copy ".\\default_config\\settings.json" ".\\config\\settings.json"

echo y> ".\\default_config\\is_installer_used"

call build.bat -release %2 64
cd release_64
cpack . -C Release
cd ..

call build.bat -release %2 32
cd release_32
cpack . -C Release
cd ..

@echo Create Installer Version -------------------------------------------------------

copy /Y ".\\release_64\\setup*" ".\\bin\\*_%1_64bit.exe"
copy /Y ".\\release_32\\setup*" ".\\bin\\*_%1_32bit.exe"
powershell Compress-Archive -Path ".\\bin\\setup_win-vind_%1_64bit.exe" -DestinationPath ".\\bin\\setup_win-vind_%1_64bit.exe.zip"
powershell Compress-Archive -Path ".\\bin\\setup_win-vind_%1_32bit.exe" -DestinationPath ".\\bin\\setup_win-vind_%1_32bit.exe.zip"

@echo Create Zip Version ----------------------------------------------------------
echo n> ".\\default_config\\is_installer_used"

mkdir ".\\bin\\win-vind"
mkdir ".\\bin\\win-vind\\config"
mkdir ".\\bin\\win-vind\\default_config"
mkdir ".\\bin\\win-vind\\log"
mkdir ".\\bin\\win-vind\\resources"

copy /Y ".\\release_64\\win-vind.exe" ".\\bin\\win-vind\\win-vind.exe"
xcopy /e /Y ".\\config" ".\\bin\\win-vind\\config"
xcopy /e /Y ".\\default_config" ".\\bin\\win-vind\\default_config"
xcopy /e /Y ".\\log" ".\\bin\\win-vind\\log"
xcopy /e /Y ".\\resources" ".\\bin\\win-vind\\resources"
powershell Compress-Archive -Path ".\\bin\\win-vind" -DestinationPath ".\\bin\\win-vind_%1_64bit".zip"

copy /Y ".\\release_32\\win-vind.exe" ".\\bin\\win-vind\\win-vind.exe"
powershell Compress-Archive -Path ".\\bin\\win-vind" -DestinationPath ".\\bin\\win-vind_%1_32bit.zip"
