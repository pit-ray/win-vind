type nul > ".\\log\\error.log"
type nul > ".\\log\\message.log"
echo y> ".\\default_config\\is_installer_used"

copy ".\\default_config\\bindings.json" ".\\config\\bindings.json"
copy ".\\default_config\\settings.json" ".\\config\\settings.json"

del /q ".\\bin\\setup*.exe"
call build.bat -release
cd release
cpack .
cd ..
copy ".\\release\\setup*.exe" ".\\bin\\"
