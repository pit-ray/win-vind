@chcp 65001
@echo Usage: build.bat [-debug/-release] [-msvc/-mingw] [32/64]

@if %1 == -help (
    exit
)

@if "%1" == "" (
    @set build_type=-debug
) else (
    @set build_type=%1
)

@if "%2" == "" (
    @set compiler=-mingw
) else (
    @set compiler=%2
)

@if %build_type% == -release (
    @goto release
) else if %build_type% == -coverity (
    @goto coverity
) else if %build_type% == -test (
   @goto test 
) else if %build_type% == -coveralls (
    @goto coveralls
) else (
    @goto debug
)

:release
    @if %3 neq 64 (
        @if %3 neq 32 (
            @echo Error: Please pass bit type 32 or 64 as the third argument.
            exit
        )
    )

    @if %compiler% == -msvc (
        if %3 == 32 (
            cmake -B release_32 -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 16 2019" -A win32 -DBIT_TYPE=32 .
            cmake --build release_32 --config Release
            xcopy /e /Y ".\\release_32\\coregui\\Release\\*.exe" "release_32"
            @rem xcopy /e /Y ".\\release_32\\wxgui\\Release\\*.exe" "release"
        ) else (
            cmake -B release_64 -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 16 2019" -A x64 -DBIT_TYPE=64 .
            cmake --build release_64 --config Release
            xcopy /e /Y ".\\release_64\\coregui\\Release\\*.exe" "release_64"
            @rem xcopy /e /Y ".\\release_64\\wxgui\\Release\\*.exe" "release"
        )
    ) else (
        if %3 == 32 (
            @echo Error: Building 32bit win-vind using MinGW is not supported.
            @goto exit
        )
        cmake -B release -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" -DBIT_TYPE=%3 .
        cmake --build release --config Release
        xcopy /e /Y ".\\release\\coregui\\*.exe" "release"
        @rem xcopy /e /Y ".\\release\\wxgui\\*.exe" "release"
    )
    @goto exit

:debug
    @if %compiler% == -msvc (
        Del /q "debug/Debug"

        if "%3" == "32" (
            cmake -B debug -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A win32 -DBIT_TYPE=32 .
            cmake --build debug --config Debug
        ) else (
            cmake -B debug -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A x64 -DBIT_TYPE=64 .
            cmake --build debug --config Debug
        )

        xcopy /e /Y ".\\debug\\coregui\\Debug\\*.exe" "debug"
        @rem xcopy /e /Y ".\\debug\\wxgui\\Debug\\*.exe" "debug"

    ) else (
        cmake -B debug -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DBIT_TYPE=64 -DCCACHE_ENABLE=OFF .
        cmake --build debug --config Debug
        xcopy /e /Y ".\\debug\\coregui\\*.exe" "debug"
        @rem xcopy /e /Y ".\\debug\\wxgui\\*.exe" "debug"
    )
    @goto exit

:coverity
    cov-configure --config debug/covtest/cov.xml --comptype g++ --compiler g++ --template --xml-option=skip_file:".*/libs.*" --xml-option=skip_file:".*/mingw64.*"
    cmake -B debug -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DBIT_TYPE=64 -DCCACHE_ENABLE=OFF .
    cd debug
    cov-build --config ./covtest/cov.xml --dir cov-int cmake --build .
    tar -czvf cov-int.tgz cov-int
    cd ..
    @goto exit

:test
    cmake -B test/build -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" test
    cmake --build test/build

    cd test/build
    ctest -C Debug
    cd ../..
    @goto exit

:coveralls
    coveralls --include core --repo_token "%2"
    @goto exit

:exit
    @echo done.
