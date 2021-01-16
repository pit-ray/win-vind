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
        @mkdir release_%3
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
        @mkdir debug
    )
    cd debug

    @if %compiler% == -msvc (
        cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A x64 -DBIT_TYPE=64 ..
        cmake --build . --config Debug
        xcopy /e /Y ".\\Debug" .
    ) else (
        cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DBIT_TYPE=64 ..
        cmake --build . --config Debug
    )
    cd ..
    @goto exit

:exit
    @echo done.
