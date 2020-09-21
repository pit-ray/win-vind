@if "%1" == "" (
    @echo [Error] Please call this script with the version as a first argument.
    @exit
)

type nul > ".\\log\\error.log"
type nul > ".\\log\\message.log"
echo n> ".\\default_config\\is_installer_used"

copy ".\\default_config\\bindings.json" ".\\config\\bindings.json"
copy ".\\default_config\\settings.json" ".\\config\\settings.json"

call build.bat --release

rmdir /q /s ".\\bin\\win-vind"

mkdir ".\\bin\\win-vind"
mkdir ".\\bin\\win-vind\\config"
mkdir ".\\bin\\win-vind\\default_config"
mkdir ".\\bin\\win-vind\\log"
mkdir ".\\bin\\win-vind\\resources"

copy ".\\release\\win-vind.exe" ".\\bin\\win-vind\\win-vind.exe"
xcopy /e ".\\config" ".\\bin\\win-vind\\config"
xcopy /e ".\\default_config" ".\\bin\\win-vind\\default_config"
xcopy /e ".\\log" ".\\bin\\win-vind\\log"
xcopy /e ".\\resources" ".\\bin\\win-vind\\resources"

del /q ".\\bin\\*.zip"
powershell Compress-Archive -Path ".\\bin\\win-vind" -DestinationPath ".\\bin\\win-vind_"%1".zip"
