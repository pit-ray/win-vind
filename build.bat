@chcp 65001
@echo Usage: build.bat [-debug/-release] [-msvc/-mingw] [32/64]

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

    @if not exist release_%3 (
        mkdir release_%3
    ) 
    cd release_%3

    @if %compiler% == -msvc (
        if %3 == 32 (
            cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 16 2019" -A Win32 -DBIT_TYPE=32 ..
        ) else (
            cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 16 2019" -A x64 -DBIT_TYPE=64 ..
        )
        cmake --build . --config Release
        xcopy /e /Y ".\\Release" .
    ) else (
        if %3 == 32 (
            @echo Error: Building 32bit win-vind using MinGW is not supported.
            @goto exit
        )
        cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" -DBIT_TYPE=%3 ..
        cmake --build . --config Release
    )
    cd ..
    @goto exit

:debug
    @if not exist debug (
        mkdir debug
    )
    cd debug

    @if %compiler% == -msvc (
        Del /q "Debug"
        cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A x64 -DBIT_TYPE=64 ..
        cmake --build . --config Debug
        xcopy /e /Y ".\\Debug" .
    ) else (
        cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DBIT_TYPE=64 -DCCACHE_ENABLE=OFF ..
        cmake --build . --config Debug
    )
    cd ..
    @goto exit

:coverity
    rmdir /s /q debug
    mkdir debug
    cd debug

    cov-configure --config ./covtest/cov.xml --comptype g++ --compiler g++ --template --xml-option=skip_file:".*/libs.*" --xml-option=skip_file:".*/mingw64.*"
    cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DBIT_TYPE=64 -DCCACHE_ENABLE=OFF ..
    cov-build --config ./covtest/cov.xml --dir cov-int cmake --build .
    tar -czvf cov-int.tgz cov-int
    cd ..
    @goto exit

:test
    @if not exist debug (
        mkdir debug
    )
    cd debug
    ctest
    cd ..
    @goto exit

:coveralls
    @if not exist debug (
        mkdir debug
    )
    cd debug
    ctest
    cd ..
    coveralls --include core --repo_token "%4" --verbose --gcov-options '\-lp'

    @goto exit

:exit
    @echo done.
