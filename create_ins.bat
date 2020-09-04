type null > ".\\log\\error.log"
type null > ".\\log\\message.log"
echo yes > ".\\default_config\\is_installer_used"

copy ".\\default_config\\bindings.json" ".\\config\\bindings.json"
copy ".\\default_config\\settings.json" ".\\config\\settings.json"

del /q ".\\bin\\setup*.exe"
del /q ".\\release"
call build.bat -release
cd release
cpack .
cd ..
copy ".\\release\\setup*.exe" ".\\bin\\"

rmdir /q /s ".\\release"