for /f "usebackq" %%A in (`where.exe mingw32-make`) do set MINGW_MAKE_PATH=%%A

cd libs
@if not exist tmp (
    mkdir tmp
)
powershell cp "%MINGW_MAKE_PATH:~0,-21%\\*-mingw*\\lib\\liboleacc.a" "tmp\\liboleacc.a"
powershell cp "%MINGW_MAKE_PATH:~0,-21%\\*-mingw*\\lib\\libuxtheme.a" "tmp\\libuxtheme.a"


copy "tmp\\liboleacc.a" "wxWidgets\\lib\\gcc_lib\\liboleacc.a"
copy "tmp\\liboleacc.a" "wxWidgets\\lib\\gcc_x64_lib\\liboleacc.a"

copy "tmp\\libuxtheme.a" "wxWidgets\\lib\\gcc_lib\\libuxtheme.a"
copy "tmp\\libuxtheme.a" "wxWidgets\\lib\\gcc_x64_lib\\libuxtheme.a"

cd ..
