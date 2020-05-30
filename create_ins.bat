type null > ".\\log\\error.log"
type null > ".\\log\\message.log"
echo 1 > ".\\admin_config\\execution_mode.idx"

copy ".\\admin_config\\default_exapp.ini" ".\\config\\custom_exapp.ini"
copy ".\\admin_config\\default_icon.idx" ".\\config\\custom_icon.idx"
copy ".\\admin_config\\default_kbtype.pth" ".\\config\\custom_kbtype.pth"
copy ".\\admin_config\\default_opts_bool.ini" ".\\config\\custom_opts_bool.ini"
copy ".\\admin_config\\default_opts.ini" ".\\config\\custom_opts.ini"
copy ".\\admin_config\\default_propdlg_resolution.idx" ".\\config\\custom_propdlg_resolution.idx"
copy ".\\admin_config\\default_ui_language.idx" ".\\config\\custom_ui_language.idx"
copy ".\\admin_config\\default.ini" ".\\config\\custom.ini"
copy ".\\admin_config\\default.xml" ".\\config\\custom.xml"

del /q ".\\bin\\setup*.exe"
del /q ".\\release"
call build.bat -release
cd release
cpack .
cd ..
copy ".\\release\\setup*.exe" ".\\bin\\"

del /q ".\\release"