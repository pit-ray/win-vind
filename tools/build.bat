@chcp 65001
@echo Usage: build.bat [-debug/-release] [-msvc/-mingw] [32/64]

@if "%1" == "" (
    @set build_type=-debug
) else if "%1" == "-help" (
    @ exit
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
            cmake -B release_32 -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A win32 .
            cmake --build release_32 --config Release
            xcopy /E /Y release_32\Release\*.exe release_32
        ) else (
            cmake -B release_64 -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64 .
            cmake --build release_64 --config Release
            xcopy /E /Y release_64\Release\*.exe release_64
        )
    ) else (
        if %3 == 32 (
            @echo Error: Building 32bit win-vind using MinGW is not supported.
            @goto exit
        )
        cmake -B release -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" .
        cmake --build release --config Release
    )
    @goto exit

:debug
    @if %compiler% == -msvc (
        Del /q "debug/Debug"

        if "%3" == "32" (
            cmake -B debug -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" -A win32 .
            cmake --build debug --config Debug
        ) else (
            INTERNALcmake -B debug -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" -A x64 .
            cmake --build debug --config Debug
        )

        xcopy /e /Y debug\Debug\*.exe debug
    ) else (
        cmake -B debug -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCCACHE_ENABLE=OFF .
        cmake --build debug --config Debug
    )
    @goto exit

:coverity
    @set covdir=build_cov
    @if %compiler% == -msvc (
        "cov_tools/bin/cov-configure" --config %covdir%/covtest/cov.xml --msvc --template --xml-option=skip_file:".*/libs.*"
        cmake -B %covdir% -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" -A x64 .
        xcopy /e /Y %covdir%\Debug\*.exe %covdir%

    ) else (
        "cov_tools/bin/cov-configure" --config %covdir%/covtest/cov.xml --comptype g++ --compiler g++ --template --xml-option=skip_file:".*/libs.*" --xml-option=skip_file:".*/mingw64.*"
        cmake -B %covdir% -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCCACHE_ENABLE=OFF .
    )

    "cov_tools/bin/cov-build" --config %covdir%/covtest/cov.xml --dir %covdir%/cov-int cmake --build %covdir%
    cd %covdir%
    tar -czvf cov-int.tgz cov-int
    cd ..
    @goto exit

:test
    cd tests
    @if %compiler% == -msvc (
        cmake -B build_msvc -G "Visual Studio 17 2022" unit
        cmake --build build_msvc
        ctest -C Debug --test-dir build_msvc --output-on-failure
    ) else (
        cmake -B build_mingw -G "MinGW Makefiles" unit
        cmake --build build_mingw
        ctest -C Debug --test-dir build_mingw --output-on-failure
    )
    cd ..
    @goto exit

:coveralls
    coveralls --include core --repo_token "%2"
    @goto exit

:exit
    @echo done.
