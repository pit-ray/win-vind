type null > ".\\log\\error.log"
type null > ".\\log\\message.log"

rem Japanese Edition
del ".\\bin\\Japanese\\*"
del ".\\config\\enable.kmp"
copy ".\\config\\JP.kmp" ".\\config\\enable.kmp"
del ".\\release"
call build.bat -release

mkdir ".\\bin\\Japanese\\win-vind"
del ".\\bin\\Japanese\\win-vind"
mkdir ".\\bin\\Japanese\\win-vind\\config"
del ".\\bin\\Japanese\\win-vind\\config"
mkdir ".\\bin\\Japanese\\win-vind\\admin_config"
del ".\\bin\\Japanese\\win-vind\\admin_config"
mkdir ".\\bin\\Japanese\\win-vind\\log"
del ".\\bin\\Japanese\\win-vind\\log"
mkdir ".\\bin\\Japanese\\win-vind\\resources"
del ".\\bin\\Japanese\\win-vind\\resources"

copy ".\\release\\win-vind.exe" ".\\bin\\Japanese\\win-vind\\win-vind.exe"
xcopy /e ".\\config" ".\\bin\\Japanese\\win-vind\\config"
xcopy /e ".\\admin_config" ".\\bin\\Japanese\\win-vind\\admin_config"
xcopy /e ".\\log" ".\\bin\\Japanese\\win-vind\\log"
xcopy /e ".\\resources" ".\\bin\\Japanese\\win-vind\\resources"

rem English Edition
del ".\\bin\\English\\*"
del ".\\config\\enable.kmp"
copy ".\\config\\US.kmp" ".\\config\\enable.kmp"

mkdir ".\\bin\\English\\win-vind"
del ".\\bin\\English\\win-vind"
mkdir ".\\bin\\English\\win-vind\\config"
del ".\\bin\\English\\win-vind\\config"
mkdir ".\\bin\\English\\win-vind\\admin_config"
del ".\\bin\\English\\win-vind\\admin_config"
mkdir ".\\bin\\English\\win-vind\\log"
del ".\\bin\\English\\win-vind\\log"
mkdir ".\\bin\\English\\win-vind\\resources"
del ".\\bin\\English\\win-vind\\resources"

copy ".\\release\\win-vind.exe" ".\\bin\\English\\win-vind\\win-vind.exe"
xcopy /e ".\\config" ".\\bin\\English\\win-vind\\config"
xcopy /e ".\\admin_config" ".\\bin\\English\\win-vind\\admin_config"
xcopy /e ".\\log" ".\\bin\\English\\win-vind\\log"
xcopy /e ".\\resources" ".\\bin\\English\\win-vind\\resources"

rem postprocess
del ".\\config\\enable.kmp"
copy ".\\config\\JP.kmp" ".\\config\\enable.kmp"