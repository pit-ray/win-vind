type null > ".\\log\\error.log"
type null > ".\\log\\message.log"
echo no > ".\\default_config\\is_installer_used"

copy ".\\default_config\\bindings.json" ".\\config\\bindings.json"
copy ".\\default_config\\settings.json" ".\\config\\settings.json"

del /q ".\\release"
call build.bat -release

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

rmdir /q /s ".\\release"