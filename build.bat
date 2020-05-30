chcp 65001

echo [cmake] _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _

if "%1" == "" (
    goto debug
)
if %1 == -debug (
    goto debug
)
if %1 == -release (
    goto release
)

:debug
    if not exist debug (
        mkdir debug
    )
    cd debug

    cmake -j 12 -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ..
    goto make

:release
    if not exist release (
        mkdir release
    )
    cd release

    cmake -j 12 -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ..
    goto make

:make
echo;
echo;
echo [mingw32-make] _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
mingw32-make -f Makefile -j 12

cd ..