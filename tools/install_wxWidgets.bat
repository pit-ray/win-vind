@if "%1" == "" (
    @echo.
    @echo Error: Please pass your compiler type -mingw or -msvc as the first argument.
    @echo.
    exit
) else (
    @set compiler=%1
)

@if "%2" == "" (
   @set BUILD_32=TRUE
   @set BUILD_64=TRUE
) else (
    @if %2 == 32 (
        @set BUILD_32=TRUE
        @set BUILD_64=FALSE
    ) else if %2 == 64 (
        @set BUILD_32=FALSE
        @set BUILD_64=TRUE
    ) else (
        @set BUILD_32=FALSE
        @set BUILD_64=FALSE
    )
)

@echo Build Status: x86: %BUILD_32%, x64 %BUILD_64%

@if "%3" == "" (
    @set REQUIRE_VCVARS=TRUE
) else (
    if "%3" == "-without-vcvars" (
        @set REQUIRE_VCVARS=FALSE
    ) else (
        @set REQUIRE_VCVARS=TRUE
    )
)
@echo Setup vcvars in the script: %REQUIRE_VCVARS%

@rem @set wxWidgets_VERSION=3.0.5
set wxWidgets_VERSION=3.1.5

@if not exist libs (
    @mkdir libs
)
cd libs

@if not exist wxWidgets (
    git clone https://github.com/wxWidgets/wxWidgets.git -b v%wxWidgets_VERSION% -j %NUMBER_OF_PROCESSORS% --depth=1
) else (
    cd wxWidgets
    git pull
    cd ..
)

cd wxWidgets
git submodule update --init -j %NUMBER_OF_PROCESSORS%
cd ..

cd wxWidgets/build/msw

@if %compiler% == -mingw (
   @echo Compiler: MinGW
   goto mingw
) else (
   @echo Compiler: Microsoft Visual C++
   goto msvc
)

:mingw
    @if exist gcc_mswu (
        powershell rm -r gcc_mswu
    )

    @if not exist ../../lib/gcc_lib (
        @echo - Build for 64bit -------------------------------------------------------------
        mingw32-make -j %NUMBER_OF_PROCESSORS% -f makefile.gcc setup_h BUILD=release SHARED=0 UNICODE=1 CPPFLAGS="-m64 -w -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00"
        mingw32-make -j %NUMBER_OF_PROCESSORS% -f makefile.gcc BUILD=release SHARED=0 UNICODE=1 CPPFLAGS="-m64 -w -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00"
        @rem powershell mv "..\\..\\lib\\gcc_lib" "..\\..\\lib\\gcc_x64_lib"
        @if exist gcc_mswu (
            powershell rm -r gcc_mswu
        )
    )

    @rem @echo - Build for 32bit -------------------------------------------------------------
    @rem mingw32-make -j %NUMBER_OF_PROCESSORS% -f makefile.gcc setup_h BUILD=release SHARED=0 UNICODE=1 CPPFLAGS="-m32 -w -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00""
    @rem mingw32-make -j %NUMBER_OF_PROCESSORS% -f makefile.gcc BUILD=release SHARED=0 UNICODE=1 CPPFLAGS="-m32 -w -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00""
    @rem powershell rm -r gcc_mswu

    @if exist gcc_mswu (
       powershell rm -r gcc_mswu
    )

    @goto final

:msvc
    @if exist vc_mswu* (
        powershell rm -r vc_mswu*
    )

    @ if %BUILD_64% == FALSE (
        @goto msvc_64_end
    )

    @echo Building 64bit...
    @ if exist ../../lib/vc_x64_lib (
        @echo Already exists the directory for x64.
        @goto msvc_64_end
    )

    @if %REQUIRE_VCVARS% == TRUE (
        if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
            call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
        ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
            call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
        )
    )
    nmake /f makefile.vc BUILD=release SHARED=0 UNICODE=1 TARGET_CPU=X64 RUNTIME_LIBS=static

    :msvc_64_end

    @ if %BUILD_32% == FALSE (
        @goto msvc_32_end
    )

    @echo Building 32bit...
    @ if exist ../../lib/vc_lib (
        @echo Already exists the directory for x86.
        @goto msvc_32_end
    )
    @ if %REQUIRE_VCVARS% == TRUE (
        if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat" (
            call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
        ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat" (
            call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
        )
    )
    nmake /f makefile.vc BUILD=release SHARED=0 UNICODE=1 TARGET_CPU=X86 RUNTIME_LIBS=static

    :msvc_32_end

    @ if exist vc_mswu* (
        powershell rm -r vc_mswu*
    )

    @goto final

:final

cd ../../../..

@rem If you use wxWidgets-3.1.x, it solves linker error.
@rem if "%1" == "-mingw" (
@rem     cd ..
@rem call "tools/copy_mingw_libs.bat"
@rem )
