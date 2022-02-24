@if "%1" == "" (
    @echo.
    @echo Error: Please pass your compiler type -mingw or -msvc as the first argument.
    @echo.
    exit
)

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

@if "%1" == "-msvc" (
    @if "%2" == "" (
        @set BUILD_32=TRUE
        @set BUILD_64=TRUE
    ) else (
        @if "%2" == "32" (
            @set BUILD_32=TRUE

        ) else (
            @set BUILD_64=TRUE
        )
    )

    @if exist vc_mswu* (
        powershell rm -r vc_mswu*
    )

    @if not exist ../../lib/vc_x64_lib (
        @if defined BUILD_64 (
            call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
            nmake /f makefile.vc BUILD=release SHARED=0 UNICODE=1 TARGET_CPU=X64 RUNTIME_LIBS=static
        )
    )

    @if not exist ../../lib/vc_lib (
        @if defined BUILD_32 (
            call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
            nmake /f makefile.vc BUILD=release SHARED=0 UNICODE=1 TARGET_CPU=X86 RUNTIME_LIBS=static
        )
    )

    @if exist vc_mswu* (
        powershell rm -r vc_mswu*
    )
) else (
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
)

cd ../../../

@rem If you use wxWidgets-3.1.x, it solves linker error.
@rem if "%1" == "-mingw" (
@rem     cd ..
@rem call "tools/copy_mingw_libs.bat"
@rem )

@echo Installed wxWidgets successfully
