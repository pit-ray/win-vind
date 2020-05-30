type null > ".\\log\\error.log"
type null > ".\\log\\message.log"
echo 0 > ".\\admin_config\\execution_mode.idx"

copy ".\\admin_config\\default_exapp.ini" ".\\config\\custom_exapp.ini"
copy ".\\admin_config\\default_icon.idx" ".\\config\\custom_icon.idx"
copy ".\\admin_config\\default_kbtype.pth" ".\\config\\custom_kbtype.pth"
copy ".\\admin_config\\default_opts_bool.ini" ".\\config\\custom_opts_bool.ini"
copy ".\\admin_config\\default_opts.ini" ".\\config\\custom_opts.ini"
copy ".\\admin_config\\default_propdlg_resolution.idx" ".\\config\\custom_propdlg_resolution.idx"
copy ".\\admin_config\\default_ui_language.idx" ".\\config\\custom_ui_language.idx"
copy ".\\admin_config\\default.ini" ".\\config\\custom.ini"
copy ".\\admin_config\\default.xml" ".\\config\\custom.xml"

del /q ".\\release"
call build.bat -release

rmdir /q /s ".\\bin\\win-vind"

mkdir ".\\bin\\win-vind"
mkdir ".\\bin\\win-vind\\config"
mkdir ".\\bin\\win-vind\\admin_config"
mkdir ".\\bin\\win-vind\\log"
mkdir ".\\bin\\win-vind\\resources"

copy ".\\release\\win-vind.exe" ".\\bin\\win-vind\\win-vind.exe"
xcopy /e ".\\config" ".\\bin\\win-vind\\config"
xcopy /e ".\\admin_config" ".\\bin\\win-vind\\admin_config"
xcopy /e ".\\log" ".\\bin\\win-vind\\log"
xcopy /e ".\\resources" ".\\bin\\win-vind\\resources"