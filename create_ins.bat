type null > "log\\error.log"
type null > "log\\message.log"

rem Japanese Edition
del ".\\bin\\Japanese\\setup*.exe"
del ".\\config\\enable.kmp"
copy ".\\config\\JP.kmp" ".\\config\\enable.kmp"

del ".\\release"
call build.bat -release
cd release
cpack .
cd ..
copy ".\\release\\setup*.exe" ".\\bin\\Japanese\\"

rem English Edition
del ".\\bin\\English\\setup*.exe"
del ".\\config\\enable.kmp"
copy ".\\config\\US.kmp" ".\\config\\enable.kmp"

del ".\\release"
call build.bat -release
cd release
cpack .
cd ..
copy ".\\release\\setup*.exe" ".\\bin\\English\\"

rem postprocess
del ".\\config\\enable.kmp"
copy ".\\config\\JP.kmp" ".\\config\\enable.kmp"