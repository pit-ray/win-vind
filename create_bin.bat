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
@if "%1" == "" (
    @echo Error: Please pass the compiler type -msvc or -mingw as a first argument.
    @exit
)
@if "%2" == "" (
    @echo Error: Please pass the bit type 64 or 32 as a second argument.
    @exit
)

del /q ".\\bin\\setup*.exe"

copy /Y ".\\release_64\\setup*.exe" ".\\bin\\*_%1_64bit.exe"
copy /Y ".\\release_32\\setup*.exe" ".\\bin\\*_%1_32bit.exe"

@echo Create Zip Version ----------------------------------------------------------
echo n> ".\\default_config\\is_installer_used"

rmdir /q /s ".\\bin\\win-vind_64bit"
del /q ".\\bin\\*.zip"

mkdir ".\\bin\\win-vind_64bit"
mkdir ".\\bin\\win-vind_64bit\\config"
mkdir ".\\bin\\win-vind_64bit\\default_config"
mkdir ".\\bin\\win-vind_64bit\\log"
mkdir ".\\bin\\win-vind_64bit\\resources"

copy ".\\release_64\\win-vind.exe" ".\\bin\\win-vind_64bit\\win-vind.exe"
xcopy /e /Y ".\\config" ".\\bin\\win-vind_64bit\\config"
xcopy /e /Y ".\\default_config" ".\\bin\\win-vind_64bit\\default_config"
xcopy /e /Y ".\\log" ".\\bin\\win-vind_64bit\\log"
xcopy /e /Y ".\\resources" ".\\bin\\win-vind_64bit\\resources"

powershell Compress-Archive -Path ".\\bin\\win-vind_64bit" -DestinationPath ".\\bin\\win-vind_%1_64bit".zip"

mkdir ".\\bin\\win-vind_32bit"
mkdir ".\\bin\\win-vind_32bit\\config"
mkdir ".\\bin\\win-vind_32bit\\default_config"
mkdir ".\\bin\\win-vind_32bit\\log"
mkdir ".\\bin\\win-vind_32bit\\resources"

copy ".\\release_32\\win-vind.exe" ".\\bin\\win-vind_32bit\\win-vind.exe"
xcopy /e /Y ".\\config" ".\\bin\\win-vind_32bit\\config"
xcopy /e /Y ".\\default_config" ".\\bin\\win-vind_32bit\\default_config"
xcopy /e /Y ".\\log" ".\\bin\\win-vind_32bit\\log"
xcopy /e /Y ".\\resources" ".\\bin\\win-vind_32bit\\resources"

powershell Compress-Archive -Path ".\\bin\\win-vind_32bit" -DestinationPath ".\\bin\\win-vind_%1_32bit.zip"
